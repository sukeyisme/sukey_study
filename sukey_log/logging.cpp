#include "utilities.h"
#include <iomanip>
#include <string>
#include <fcntl.h>
#include "commandlineflags.h"
#include "logging.h"
#include "raw_logging.h"

using std::setw;
using std::setfill;
using std::string;
using std::ostringstream;
using std::vector;

#define EXCLUSIVE_LOCKS_REQUIRED(mu)

LOG_DEFINE_bool(log_prefix,true,"准备每行Log开始的前缀 ");
LOG_DEFINE_int32(minloglevel, 0, "输出最低等级的Log");
LOG_DEFINE_string(log_backtrace_at,"","当logging到file:linenum.发出回溯");
LOG_DEFINE_bool(logtostderr,false,"除了输出到log文件输出到标准错误中去");
LOG_DEFINE_int32(logbuflevel,0,"缓存级别（只缓存不输出），在这个指定等级以下包括这个等级的LOG缓存，比如0表示什么都不缓存，1表示只缓存 INFO");
LOG_DEFINE_int32(max_log_size,1800,"log文件的最大尺寸(MB)。如果是0那么就成改写成1");
LOG_DEFINE_bool(stop_logging_if_full_disk,false,"如果盘符是空的就停止LOG");
LOG_DEFINE_int32(logbufsecs,30,"LOG日志最多这些时间");
#define PATH_SEPARATOR '/' //为windows考虑

static void GetHostName(string* hostname) {
#if defined(HAVE_SYS_UTSNAME_H)
  struct utsname buf;
  if (0 != uname(&buf)) {
    // ensure null termination on failure
    *buf.nodename = '\0';
  }
  *hostname = buf.nodename;
#elif defined(OS_WINDOWS)
  char buf[MAX_COMPUTERNAME_LENGTH + 1];
  DWORD len = MAX_COMPUTERNAME_LENGTH + 1;
  if (GetComputerNameA(buf, &len)) {
    *hostname = buf;
  } else {
    hostname->clear();
  }
#else
# warning There is no way to retrieve the host name.
  *hostname = "(unknown)";
#endif
}

static const char* DefaultLogDir()
{
	const char* env;
	env = getenv("SUKEY_LOG_DIR");
	if(env!=NULL && env[0]!='\0')
		return env;
	env = getenv("TEST_TMPDIR");
	if(env!=NULL && env[0]!='\0')
		return env;
	return "";
}
LOG_DEFINE_string(log_dir,DefaultLogDir(),"如果有指定就用指定的，如果没有就用默认的");

_START_SUKEY_NAMESPACE_

struct LogMessage::LogMessageData
{
	LogMessageData() {};

	int preserved_errno_;
	char* buf_;
	char* message_text_;
	LogStream* stream_alloc_;
  LogStream* stream_;
	char severity_;
	int line_;
	void (LogMessage::*send_method_)();
	union
	{
		LogSink* sink_;
		std::vector<std::string>* outvec_;
		std::string* message_;
	};
	time_t timestamp_;
	struct ::tm tm_time_;
	size_t num_prefix_chars_;
	size_t num_chars_to_log_;
	size_t num_chars_to_syslog_;
	const char* basename_;
	const char* fullname_;
	bool has_been_flushed_;
	bool first_fatal_;

	~LogMessageData();
private:
	LogMessageData(const LogMessageData&);
  void operator=(const LogMessageData&);
};

static bool stop_writing = false;

const char*const LogSeverityNames[NUM_SEVERITIES] = {
  "INFO", "WARNING", "ERROR", "FATAL"
};

class LogFileObject:public base::Logger
{
public:
  LogFileObject(LogSeverity severity, const char* base_filename);
  ~LogFileObject();
	virtual void Write(bool force_flush,time_t timestamp,const char* message,int message_len);
	virtual void Flush();
	virtual uint32 LogSize() 
	{
    MutexLock l(&lock_);
    return file_length_;
  }

	void FlushUnlocked();
private:
	static const uint32 kRolloverAttemptFrequency = 0x20;

	Mutex lock_;
	bool base_filename_selected_;
  string base_filename_;
  string symlink_basename_;
  string filename_extension_;
  FILE* file_;
  LogSeverity severity_;
  uint32 bytes_since_flush_;
	uint32 file_length_;
	unsigned int rollover_attempt_;
  int64 next_flush_time_; 

	bool CreateLogfile(const string& time_pid_string);
};

static int32 MaxLogSize() 
{
  return (FLAGS_max_log_size > 0 ? FLAGS_max_log_size : 1);
}

LogFileObject::LogFileObject(LogSeverity severity,
                             const char* base_filename)
  : base_filename_selected_(base_filename != NULL),
    base_filename_((base_filename != NULL) ? base_filename : ""),
    symlink_basename_(log_internal_::ProgramInvocationShortName()),
    filename_extension_(),
    file_(NULL),
    severity_(severity),
    bytes_since_flush_(0),
    file_length_(0),
    rollover_attempt_(kRolloverAttemptFrequency-1),
    next_flush_time_(0) {
  assert(severity >= 0);
  assert(severity < NUM_SEVERITIES);
}

LogFileObject::~LogFileObject() {
  MutexLock l(&lock_);
  if (file_ != NULL) {
    fclose(file_);
    file_ = NULL;
  }
}

void LogFileObject::Flush() {
  MutexLock l(&lock_);
  FlushUnlocked();
}

bool LogFileObject::CreateLogfile(const string& time_pid_string) 
{
  string string_filename = base_filename_+filename_extension_+
                           time_pid_string;
  const char* filename = string_filename.c_str();
  int fd = open(filename, O_WRONLY | O_CREAT | O_EXCL, 0664);
  if (fd == -1) return false;
#ifdef HAVE_FCNTL
  // Mark the file close-on-exec. We don't really care if this fails
  fcntl(fd, F_SETFD, FD_CLOEXEC);
#endif

  file_ = fdopen(fd, "a");  // Make a FILE*.
  if (file_ == NULL) {  // Man, we're screwed!
    close(fd);
    unlink(filename);  // Erase the half-baked evidence: an unusable log file
    return false;
  }

  // We try to create a symlink called <program_name>.<severity>,
  // which is easier to use.  (Every time we create a new logfile,
  // we destroy the old symlink and create a new one, so it always
  // points to the latest logfile.)  If it fails, we're sad but it's
  // no error.
  if (!symlink_basename_.empty()) {
    // take directory from filename
    const char* slash = strrchr(filename, PATH_SEPARATOR);
    const string linkname =
      symlink_basename_ + '.' + LogSeverityNames[severity_];
    string linkpath;
    if ( slash ) linkpath = string(filename, slash-filename+1);  // get dirname
    linkpath += linkname;
    unlink(linkpath.c_str());                    // delete old one if it exists

    // We must have unistd.h.
#ifdef HAVE_UNISTD_H
    // Make the symlink be relative (in the same dir) so that if the
    // entire log directory gets relocated the link is still valid.
    const char *linkdest = slash ? (slash + 1) : filename;
    if (symlink(linkdest, linkpath.c_str()) != 0) {
      // silently ignore failures
    }

    // Make an additional link to the log file in a place specified by
    // FLAGS_log_link, if indicated
    if (!FLAGS_log_link.empty()) {
      linkpath = FLAGS_log_link + "/" + linkname;
      unlink(linkpath.c_str());                  // delete old one if it exists
      if (symlink(filename, linkpath.c_str()) != 0) {
        // silently ignore failures
      }
    }
#endif
  }

  return true;  // Everything worked
}

class LogDestination
{
public:	
	friend class LogMessage;

	static const string& hostname();
private:
	LogDestination(LogSeverity severity, const char* base_filename);
	~LogDestination(){}
	static void WaitForSinks(LogMessage::LogMessageData* data);
	static void MaybeLogToLogfile(LogSeverity severity,time_t timestamp,const char* message, size_t len);
	static void LogToAllLogfiles(LogSeverity severity,time_t timestamp,const char* message, size_t len);
	static LogDestination* log_destination(LogSeverity severity);

	LogFileObject fileobject_;

	static LogDestination* log_destinations_[NUM_SEVERITIES];
	static Mutex sink_mutex_;
	base::Logger* logger_;

	static string hostname_;

	// 不允许拷贝
  LogDestination(const LogDestination&);
  LogDestination& operator=(const LogDestination&);
};

string LogDestination::hostname_;

const string& LogDestination::hostname()
{
  if (hostname_.empty()) {
    GetHostName(&hostname_);
    if (hostname_.empty()) {
      hostname_ = "(unknown)";
    }
  }
  return hostname_;
}

void LogFileObject::FlushUnlocked(){
  if (file_ != NULL) {
    fflush(file_);
    bytes_since_flush_ = 0;
  }
  // Figure out when we are due for another flush.
  const int64 next = (FLAGS_logbufsecs
                      * static_cast<int64>(1000000));  // in usec
  next_flush_time_ = CycleClock_Now() + UsecToCycles(next);
}

void LogFileObject::Write(bool force_flush,time_t timestamp,const char* message,int message_len)
{
	MutexLock l(&lock_);

  // We don't log if the base_name_ is "" (which means "don't write")
  if (base_filename_selected_ && base_filename_.empty()) {
    return;
  }

  if (static_cast<int>(file_length_ >> 20) >= MaxLogSize() ||
      PidHasChanged()) {
    if (file_ != NULL) fclose(file_);
    file_ = NULL;
    file_length_ = bytes_since_flush_ = 0;
    rollover_attempt_ = kRolloverAttemptFrequency-1;
  }

  // If there's no destination file, make one before outputting
  if (file_ == NULL) {
    // Try to rollover the log file every 32 log messages.  The only time
    // this could matter would be when we have trouble creating the log
    // file.  If that happens, we'll lose lots of log messages, of course!
    if (++rollover_attempt_ != kRolloverAttemptFrequency) return;
    rollover_attempt_ = 0;

    struct ::tm tm_time;
    localtime_r(&timestamp, &tm_time);

    // The logfile's filename will have the date/time & pid in it
    ostringstream time_pid_stream;
    time_pid_stream.fill('0');
    time_pid_stream << 1900+tm_time.tm_year
                    << setw(2) << 1+tm_time.tm_mon
                    << setw(2) << tm_time.tm_mday
                    << '-'
                    << setw(2) << tm_time.tm_hour
                    << setw(2) << tm_time.tm_min
                    << setw(2) << tm_time.tm_sec
                    << '.'
                    << GetMainThreadPid();
    const string& time_pid_string = time_pid_stream.str();

    if (base_filename_selected_) {
      if (!CreateLogfile(time_pid_string)) {
        perror("Could not create log file");
        fprintf(stderr, "COULD NOT CREATE LOGFILE '%s'!\n",
                time_pid_string.c_str());
        return;
      }
    } else {
      // If no base filename for logs of this severity has been set, use a
      // default base filename of
      // "<program name>.<hostname>.<user name>.log.<severity level>.".  So
      // logfiles will have names like
      // webserver.examplehost.root.log.INFO.19990817-150000.4354, where
      // 19990817 is a date (1999 August 17), 150000 is a time (15:00:00),
      // and 4354 is the pid of the logging process.  The date & time reflect
      // when the file was created for output.
      //
      // Where does the file get put?  Successively try the directories
      // "/tmp", and "."
      string stripped_filename(
				log_internal_::ProgramInvocationShortName());
      string hostname;
      GetHostName(&hostname);

      string uidname = MyUserName();
      // We should not call CHECK() here because this function can be
      // called after holding on to log_mutex. We don't want to
      // attempt to hold on to the same mutex, and get into a
      // deadlock. Simply use a name like invalid-user.
      if (uidname.empty()) uidname = "invalid-user";

      stripped_filename = stripped_filename+'.'+hostname+'.'
                          +uidname+".log."
                          +LogSeverityNames[severity_]+'.';
      // We're going to (potentially) try to put logs in several different dirs
      const vector<string> & log_dirs = GetLoggingDirectories();

      // Go through the list of dirs, and try to create the log file in each
      // until we succeed or run out of options
      bool success = false;
      for (vector<string>::const_iterator dir = log_dirs.begin();
           dir != log_dirs.end();
           ++dir) {
        base_filename_ = *dir + "/" + stripped_filename;
        if ( CreateLogfile(time_pid_string) ) {
          success = true;
          break;
        }
      }
      // If we never succeeded, we have to give up
      if ( success == false ) {
        perror("Could not create logging file");
        fprintf(stderr, "COULD NOT CREATE A LOGGINGFILE %s!",
                time_pid_string.c_str());
        return;
      }
    }

    // Write a header message into the log file
    ostringstream file_header_stream;
    file_header_stream.fill('0');
    file_header_stream << "Log file created at: "
                       << 1900+tm_time.tm_year << '/'
                       << setw(2) << 1+tm_time.tm_mon << '/'
                       << setw(2) << tm_time.tm_mday
                       << ' '
                       << setw(2) << tm_time.tm_hour << ':'
                       << setw(2) << tm_time.tm_min << ':'
                       << setw(2) << tm_time.tm_sec << '\n'
                       << "Running on machine: "
                       << LogDestination::hostname() << '\n'
                       << "Log line format: [IWEF]mmdd hh:mm:ss.uuuuuu "
                       << "threadid file:line] msg" << '\n';
    const string& file_header_string = file_header_stream.str();

    const int header_len = file_header_string.size();
    fwrite(file_header_string.data(), 1, header_len, file_);
    file_length_ += header_len;
    bytes_since_flush_ += header_len;
  }

  // Write to LOG file
  if ( !stop_writing ) {
    // fwrite() doesn't return an error when the disk is full, for
    // messages that are less than 4096 bytes. When the disk is full,
    // it returns the message length for messages that are less than
    // 4096 bytes. fwrite() returns 4096 for message lengths that are
    // greater than 4096, thereby indicating an error.
    errno = 0;
    fwrite(message, 1, message_len, file_);
    if ( FLAGS_stop_logging_if_full_disk &&
         errno == ENOSPC ) {  // disk full, stop writing to disk
      stop_writing = true;  // until the disk is
      return;
    } else {
      file_length_ += message_len;
      bytes_since_flush_ += message_len;
    }
  } else {
    if ( CycleClock_Now() >= next_flush_time_ )
      stop_writing = false;  // check to see if disk has free space.
    return;  // no need to flush
  }

  // See important msgs *now*.  Also, flush logs at least every 10^6 chars,
  // or every "FLAGS_logbufsecs" seconds.
  if ( force_flush ||
       (bytes_since_flush_ >= 1000000) ||
       (CycleClock_Now() >= next_flush_time_) ) {
    FlushUnlocked();
#ifdef OS_LINUX
    if (FLAGS_drop_log_memory) {
      if (file_length_ >= logging::kPageSize) {
        // don't evict the most recent page
        uint32 len = file_length_ & ~(logging::kPageSize - 1);
        posix_fadvise(fileno(file_), 0, len, POSIX_FADV_DONTNEED);
      }
    }
#endif
  }
}

Mutex LogDestination::sink_mutex_;


LogDestination::LogDestination(LogSeverity severity,
                               const char* base_filename)
  : fileobject_(severity, base_filename),
    logger_(&fileobject_) {
}

inline void LogDestination::WaitForSinks(sukey::LogMessage::LogMessageData *data)
{
	ReaderMutexLock l(&sink_mutex_);
}

LogDestination* LogDestination::log_destinations_[NUM_SEVERITIES];

inline LogDestination* LogDestination::log_destination(LogSeverity severity) 
{
  assert(severity >=0 && severity < NUM_SEVERITIES);
  if (!log_destinations_[severity]) {
    log_destinations_[severity] = new LogDestination(severity, NULL);
  }
  return log_destinations_[severity];
}

inline void LogDestination::MaybeLogToLogfile(LogSeverity severity,
                                              time_t timestamp,
																							const char* message,
																							size_t len)
{
	const bool should_flush = severity > FLAGS_logbuflevel;
	LogDestination* destination = log_destination(severity);
	destination->logger_->Write(should_flush,timestamp,message,len);
}

inline void LogDestination::LogToAllLogfiles(LogSeverity severity,
                                             time_t timestamp,
                                             const char* message,
                                             size_t len) {

  //if ( FLAGS_logtostderr ) {           // global flag: never log to file
  //ColoredWriteToStderr(severity, message, len);
  //} else {
   for (int i = severity; i >= 0; --i)
			LogDestination::MaybeLogToLogfile(i, timestamp, message, len);
  //}
}

//全局变量
static Mutex log_mutex;
int64 LogMessage::num_messages_[NUM_SEVERITIES] = {0, 0, 0, 0};//每个等级的Log数 在log_mutex下
const size_t LogMessage::kMaxLogMessageLen = 1000;

static bool exit_on_dfatal = true;
static Mutex fatal_msg_lock;
static CrashReason crash_reason;
static bool fatal_msg_exclusive = true;
static char fatal_msg_buf_exclusive[LogMessage::kMaxLogMessageLen+1];
static char fatal_msg_buf_shared[LogMessage::kMaxLogMessageLen+1];
static LogMessage::LogStream fatal_msg_stream_exclusive(
    fatal_msg_buf_exclusive, LogMessage::kMaxLogMessageLen, 0);
static LogMessage::LogStream fatal_msg_stream_shared(
    fatal_msg_buf_shared, LogMessage::kMaxLogMessageLen, 0);
static LogMessage::LogMessageData fatal_msg_data_exclusive;
static LogMessage::LogMessageData fatal_msg_data_shared;


LogMessage::LogMessageData::~LogMessageData()
{
	delete[] buf_;
  delete stream_alloc_;
}

LogMessage::LogMessage(const char* file, int line):allocated_(NULL) 
{
  Init(file, line, LOG_INFO, &LogMessage::SendToLog);
}

LogMessage::LogMessage(const char* file, int line, LogSeverity severity): allocated_(NULL)
{
  Init(file, line, severity, &LogMessage::SendToLog);
}

LogMessage::~LogMessage() 
{
  Flush();
  delete allocated_;
}

void LogMessage::Flush()
{
	if(data_->has_been_flushed_ || data_->severity_<FLAGS_minloglevel)
		return;

	data_->num_chars_to_log_ = data_->stream_->pcount();
	data_->num_chars_to_syslog_ = data_->num_chars_to_log_ - data_->num_prefix_chars_;
	
	//在末尾加换行符
	bool append_newline = (data_->message_text_[data_->num_chars_to_log_-1] != '\n');
	char original_final_char = '\0';

	if (append_newline) {
    original_final_char = data_->message_text_[data_->num_chars_to_log_];
    data_->message_text_[data_->num_chars_to_log_++] = '\n';
  }

	{
    MutexLock l(&log_mutex);
    (this->*(data_->send_method_))();
    ++num_messages_[static_cast<int>(data_->severity_)];
  }
	// LogDestination::WaitForSinks(data_); TODO:添加实现

	if(append_newline)
	{
		 data_->message_text_[data_->num_chars_to_log_-1] = original_final_char;
	}

	 if (data_->preserved_errno_ != 0) 
	{
    errno = data_->preserved_errno_;
  }

	 data_->has_been_flushed_ = true;
}

void LogMessage::Init(const char* file, int line, LogSeverity severity,void (LogMessage::*send_method)())
{
	  allocated_ = NULL;
  if (severity != LOG_FATAL || !exit_on_dfatal) {
    allocated_ = new LogMessageData();
    data_ = allocated_;
    data_->buf_ = new char[kMaxLogMessageLen+1];
		memset(data_->buf_,0,kMaxLogMessageLen+1); //TODO:不知道是否合适
    data_->message_text_ = data_->buf_;
    data_->stream_alloc_ =
        new LogStream(data_->message_text_, kMaxLogMessageLen, 0);
    data_->stream_ = data_->stream_alloc_;
    data_->first_fatal_ = false;
  } else {
    MutexLock l(&fatal_msg_lock);
    if (fatal_msg_exclusive) {
      fatal_msg_exclusive = false;
      data_ = &fatal_msg_data_exclusive;
      data_->message_text_ = fatal_msg_buf_exclusive;
      data_->stream_ = &fatal_msg_stream_exclusive;
      data_->first_fatal_ = true;
    } else {
      data_ = &fatal_msg_data_shared;
      data_->message_text_ = fatal_msg_buf_shared;
      data_->stream_ = &fatal_msg_stream_shared;
      data_->first_fatal_ = false;
    }
    data_->stream_alloc_ = NULL;
  }

  stream().fill('0');
  data_->preserved_errno_ = errno;
  data_->severity_ = severity;
  data_->line_ = line;
  data_->send_method_ = send_method;
  data_->sink_ = NULL;
  data_->outvec_ = NULL;
  WallTime now = WallTime_Now();
  data_->timestamp_ = static_cast<time_t>(now);
  localtime_r(&data_->timestamp_, &data_->tm_time_);
  int usecs = static_cast<int>((now - data_->timestamp_) * 1000000);
  RawLog__SetLastTime(data_->tm_time_, usecs);

  data_->num_chars_to_log_ = 0;
  data_->num_chars_to_syslog_ = 0;
  data_->basename_ = const_basename(file);
  data_->fullname_ = file;
  data_->has_been_flushed_ = false;

  // If specified, prepend a prefix to each line.  For example:
  //    I1018 160715 f5d4fbb0 logging.cc:1153]
  //    (log level, GMT month, date, time, thread_id, file basename, line)
  // We exclude the thread_id for the default thread.
  if (FLAGS_log_prefix && (line != kNoLogPrefix)) {
    stream() << LogSeverityNames[severity][0]
             << setw(2) << 1+data_->tm_time_.tm_mon
             << setw(2) << data_->tm_time_.tm_mday
             << ' '
             << setw(2) << data_->tm_time_.tm_hour  << ':'
             << setw(2) << data_->tm_time_.tm_min   << ':'
             << setw(2) << data_->tm_time_.tm_sec   << "."
             << setw(6) << usecs
             << ' '
             << setfill(' ') << setw(5)
             << static_cast<unsigned int>(GetTID()) << setfill('0')
             << ' '
             << data_->basename_ << ':' << data_->line_ << "] ";
  }
  data_->num_prefix_chars_ = data_->stream_->pcount();

  if (!FLAGS_log_backtrace_at.empty()) {
    char fileline[128];
    snprintf(fileline, sizeof(fileline), "%s:%d", data_->basename_, line);
#ifdef HAVE_STACKTRACE
    if (!strcmp(FLAGS_log_backtrace_at.c_str(), fileline)) {
      string stacktrace;
      DumpStackTraceToString(&stacktrace);
      stream() << " (stacktrace:\n" << stacktrace << ") ";
    }
#endif
  }
}



void LogMessage::SendToLog() EXCLUSIVE_LOCKS_REQUIRED(log_mutex)
{
	LogDestination::LogToAllLogfiles(data_->severity_, data_->timestamp_,
                                     data_->message_text_,
                                     data_->num_chars_to_log_);
	//LogDestination::MaybeLogToStderr(data_->severity_, data_->message_text_,
                   //                  data_->num_chars_to_log_);
	//需要用户自己实现
	/*LogDestination::LogToSinks(data_->severity_,
                               data_->fullname_, data_->basename_,
                               data_->line_, &data_->tm_time_,
                               data_->message_text_ + data_->num_prefix_chars_,
                               (data_->num_chars_to_log_
                                - data_->num_prefix_chars_ - 1));*/
}

std::ostream& LogMessage::stream()
{
	return *(data_->stream_);
}

#ifdef HAVE___ATTRIBUTE__
# define ATTRIBUTE_NORETURN __attribute__((noreturn))
#else
# define ATTRIBUTE_NORETURN
#endif

static void logging_fail() ATTRIBUTE_NORETURN;

static void logging_fail() {
#if defined(_DEBUG) && defined(_MSC_VER)
  _asm int 3
#else
  abort();
#endif
}

typedef void (*logging_fail_func_t)() ATTRIBUTE_NORETURN;

SUKEY_LOG_DLL_DECL logging_fail_func_t g_logging_fail_func = &logging_fail;

void LogMessage::Fail()
{
  g_logging_fail_func();
}

LogMessageFatal::LogMessageFatal(const char* file, int line) :
    LogMessage(file, line, LOG_FATAL) {}

LogMessageFatal::LogMessageFatal(const char* file, int line,
                                 const CheckOpString& result) :
    LogMessage(file, line, result) {}

LogMessageFatal::~LogMessageFatal() {
    Flush();
    LogMessage::Fail();
}

LogSink::~LogSink()
{

}
void LogSink::WaitTillSent() {
 
}

std::string LogSink::ToString(LogSeverity severity, const char* file, int line,
                         const struct ::tm* tm_time,
                         const char* message, size_t message_len) {
	std::ostringstream stream(std::string(message, message_len));
  stream.fill('0');
  int usecs = 0;

  stream << LogSeverityNames[severity][0]
         << setw(2) << 1+tm_time->tm_mon
         << setw(2) << tm_time->tm_mday
         << ' '
         << setw(2) << tm_time->tm_hour << ':'
         << setw(2) << tm_time->tm_min << ':'
         << setw(2) << tm_time->tm_sec << '.'
         << setw(6) << usecs
         << ' '
         << setfill(' ') << setw(5) << GetTID() << setfill('0')
         << ' '
         << file << ':' << line << "] ";

  stream << string(message, message_len);
  return stream.str();
}

static void GetTempDirectories(vector<string>* list) {
  list->clear();
#ifdef OS_WINDOWS
  // On windows we'll try to find a directory in this order:
  //   C:/Documents & Settings/whomever/TEMP (or whatever GetTempPath() is)
  //   C:/TMP/
  //   C:/TEMP/
  //   C:/WINDOWS/ or C:/WINNT/
  //   .
  char tmp[MAX_PATH];
  if (GetTempPathA(MAX_PATH, tmp))
    list->push_back(tmp);
  list->push_back("C:\\tmp\\");
  list->push_back("C:\\temp\\");
#else
  // Directories, in order of preference. If we find a dir that
  // exists, we stop adding other less-preferred dirs
  const char * candidates[] = {
    // Non-null only during unittest/regtest
    getenv("TEST_TMPDIR"),

    // Explicitly-supplied temp dirs
    getenv("TMPDIR"), getenv("TMP"),

    // If all else fails
    "/tmp",
  };

  for (size_t i = 0; i < ARRAYSIZE(candidates); i++) {
    const char *d = candidates[i];
    if (!d) continue;  // Empty env var

    // Make sure we don't surprise anyone who's expecting a '/'
    string dstr = d;
    if (dstr[dstr.size() - 1] != '/') {
      dstr += "/";
    }
    list->push_back(dstr);

    struct stat statbuf;
    if (!stat(d, &statbuf) && S_ISDIR(statbuf.st_mode)) {
      // We found a dir that exists - we're done.
      return;
    }
  }

#endif
}

vector<string>* logging_directories_list;

const vector<string>& GetLoggingDirectories() {
  // Not strictly thread-safe but we're called early in InitGoogle().
  if (logging_directories_list == NULL) {
    logging_directories_list = new vector<string>;

    if ( !FLAGS_log_dir.empty() ) {
      // A dir was specified, we should use it
      logging_directories_list->push_back(FLAGS_log_dir.c_str());
    } else {
      GetTempDirectories(logging_directories_list);
#ifdef OS_WINDOWS
      char tmp[MAX_PATH];
      if (GetWindowsDirectoryA(tmp, MAX_PATH))
        logging_directories_list->push_back(tmp);
      logging_directories_list->push_back(".\\");
#else
      logging_directories_list->push_back("./");
#endif
    }
  }
  return *logging_directories_list;
}


_END_SUKEY_NAMESPACE_