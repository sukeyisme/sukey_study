#include "utilities.h"
#include <iomanip>
#include <string>
#include "logging.h"

using std::setw;
using std::setfill;
using std::string;
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

//全局变量
const size_t LogMessage::kMaxLogMessageLen = 1000;
const char* const LogSeverityNames[NUM_SEVERITIES] =
{
	"INFO","WARNING","ERROR","FATAL"
};

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
	
}

void LogMessage::Init(const char* file, int line, LogSeverity severity,void (LogMessage::*send_method)())
{
	  allocated_ = NULL;
  if (severity != LOG_FATAL || !exit_on_dfatal) {
    allocated_ = new LogMessageData();
    data_ = allocated_;
    data_->buf_ = new char[kMaxLogMessageLen+1];
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

void LogMessage::SendToLog()
{

}

std::ostream& LogMessage::stream()
{
	return *(data_->stream_);
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



_END_SUKEY_NAMESPACE_