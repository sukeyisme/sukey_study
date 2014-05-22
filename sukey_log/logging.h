/*!
*	\brief			log主要头文件
* \details		各种LOG宏
*	\author			Sukey
* \version		0.1
* \date				2013-2014
* \pre				暂时没有
*	\bug				暂时没有发现BUG
* \warning		随便用
* \copyright	sukey's code
*/
#ifndef _LOGGING_H_
#define _LOGGING_H_

#include "config.h"
#include <errno.h>
#include <string.h>
#include <time.h>
#include <iosfwd>
#include <ostream>
#include <sstream>
#include <string>
#include <stdarg.h>
#if 0
# include <unistd.h>
#endif
#include <vector>

namespace SUKEY_NAMESPACE
{
#if 0
#elif 0
#elif 1 //VC7 格式
typedef __int32 int32;
typedef unsigned __int32 uint32;
typedef __int64 int64;
typedef unsigned __int64 uint64;
#else
#endif
}

#ifndef SUKEY_STRIP_LOG
#define SUKEY_STRIP_LOG	0
#endif

#ifndef SUKEY_PREDICT_BRANCH_NOT_TAKEN
#if 0
#define SUKEY_PREDICT_BRANCH_NOT_TAKEN(x) (__builtin_expect(x, 0))
#define SUKEY_PREDICT_FALSE(x) (__builtin_expect(x, 0))
#define SUKEY_PREDICT_TRUE(x) (__builtin_expect(!!(x), 1))
#else
#define SUKEY_PREDICT_BRANCH_NOT_TAKEN(x) x
#define SUKEY_PREDICT_FALSE(x) x
#define SUKEY_PREDICT_TRUE(x) x
#endif
#endif

//根据配置来定义需要的LOG宏
#if SUKEY_STRIP_LOG ==0
#define COMPACT_SUKEY_LOG_INFO sukey::LogMessage(\
	__FILE__,__LINE__)
#else
#define COMPACT_SUKEY_LOG_INFO google::NullStream()
#endif

#if SUKEY_STRIP_LOG <=1
#define COMPACT_SUKEY_LOG_WARNING sukey::LogMessage(\
	__FILE__,__LINE__,sukey::LOG_WARNING)
#else
#define COMPACT_SUKEY_LOG_WARNING google::NullStream()
#endif

#if SUKEY_STRIP_LOG <=2
#define COMPACT_SUKEY_LOG_ERROR sukey::LogMessage( \
      __FILE__, __LINE__, sukey::LOG_ERROR)
#else
#define COMPACT_SUKEY_LOG_ERROR sukey::NullStream()
#endif

#if SUKEY_STRIP_LOG <=3
#define COMPACT_SUKEY_LOG_FATAL sukey::LogMessageFatal( \
      __FILE__, __LINE__)
#else
#define COMPACT_SUKEY_LOG_FATAL sukey::NullStreamFatal()
#endif

#ifndef DECLARE_VARIABLE
#define MUST_UNDEF_FLAGS_DECLARE_MACROS
#define DECLARE_VARIABLE(type, shorttype, name, tn)\
	namespace fL##shorttype{\
    extern SUKEY_LOG_DLL_DECL type FLAGS_##name;\
  }\
  using fL##shorttype::FLAGS_##name
#define DECLARE_bool(name)\
  DECLARE_VARIABLE(bool, B, name, bool)
	
#define DECLARE_int32(name) \
  DECLARE_VARIABLE(sukey::int32, I, name, int32)

#define DECLARE_string(name)\
	namespace fLS\
	{\
		extern SUKEY_LOG_DLL_DECL std::string&  FLAGS_##name;\
	}\
	using fLS::FLAGS_##name
#endif

DECLARE_bool(log_prefix);

DECLARE_int32(minloglevel);

DECLARE_string(log_dir);

DECLARE_int32(logbuflevel);

DECLARE_int32(max_log_size);

DECLARE_bool(stop_logging_if_full_disk);

DECLARE_int32(logbufsecs);

DECLARE_bool(log_open);

#ifdef MUST_UNDEF_FLAGS_DECLARE_MACROS
#undef MUST_UNDEF_GFLAGS_DECLARE_MACROS
#undef DECLARE_VARIABLE
#undef DECLARE_bool
#undef DECLARE_int32
#endif

inline void LogPrintf(char* out,size_t size,const char* format,...)
{
	va_list args;
	va_start(args,format);
	vsnprintf(out,size,format,args);
	va_end(args);
}

//普通LOG宏
#define LOG(severity) if(FLAGS_log_open) COMPACT_SUKEY_LOG_##severity.stream()
#define LOG_PRINTF(severity,format,...) \
	do{\
		char buf[600]={0};\
		LogPrintf(buf,sizeof(buf)/sizeof(char)-1,format,##__VA_ARGS__);\
		LOG(severity)<<buf;\
	}while(0)\

//sukey 空间内的各种类
namespace SUKEY_NAMESPACE
{
#include "log_severity.h"

}

namespace SUKEY_NAMESPACE
{
#ifdef LOG_NO_ABBREVIATED_SEVERITIES
#else
#define LOG_ERROR_MSG ERROR_macro_is_defined_Define_LOG_NO_ABBREVIATED_SEVERITIES_before_including_logging_h_See_the_document_for_detail
#define COMPACT_SUKEY_LOG_0 LOG_ERROR_MSG
#define SYSLOG_0 LOG_ERROR_MSG
#define LOG_TO_STRING_0 LOG_ERROR_MSG
#define LOG_0 LOG_ERROR_MSG
#endif

#ifndef NDEBUG

#else
#define DLOG(severity)\
	(true) ? (void)0 : sukey::LogMessageVoidify() & LOG(severity)
#endif


namespace BASE_LOGGING_NAMESPACE
{
	class LogStreamBuf:public std::streambuf
	{
	public:
		LogStreamBuf(char *buf, int len) 
		{
			setp(buf, buf + len - 2);
		}

		virtual int_type overflow(int_type ch) {
    return ch;
		}
		
		size_t pcount() const { return pptr() - pbase(); }
		char* pbase() const { return std::streambuf::pbase(); }
	};
}

struct DummyClassToDefineOperator {};


// Define global operator<< to declare using ::operator<<.
// This declaration will allow use to use CHECK macros for user
// defined classes which have operator<< (e.g., stl_logging.h).
inline std::ostream& operator<<(
    std::ostream& out, const sukey::DummyClassToDefineOperator&) {
  return out;
}


class SUKEY_LOG_DLL_DECL LogMessage
{
public:
	LogMessage(const char* file, int line);
	LogMessage(const char* file, int line, LogSeverity severity);
  ~LogMessage();

	enum
	{
		kNoLogPrefix = -1
	};

//LogStream继承自std::ostream 是一个没有DLL接口的类 所以VC会报警告 然而 MSDN说可以忽略这个问题
//http://msdn.microsoft.com/en-us/library/3tdb471s(VS.80).aspx
//#ifdef _MSC_VER
//#pragma warning(disable: 4275)
//#endif
	class SUKEY_LOG_DLL_DECL LogStream:public std::ostream
	{
	public:
		LogStream(char *buf, int len, int ctr):std::ostream(NULL),streambuf_(buf, len), ctr_(ctr),self_(this)
		{
			rdbuf(&streambuf_);
		}
		
		int ctr() const { return ctr_; }
    void set_ctr(int ctr) { ctr_ = ctr; }
    LogStream* self() const { return self_; }

    size_t pcount() const { return streambuf_.pcount(); }
    char* pbase() const { return streambuf_.pbase(); }
    char* str() const { return pbase(); }

	private:
		base_logging::LogStreamBuf streambuf_;
		int ctr_;
		LogStream *self_;
	};

	void Flush();

	static const size_t kMaxLogMessageLen;

	//不能在logging.*之外直接调用
	void SendToLog();

	void Init(const char* file, int line, LogSeverity severity,
            void (LogMessage::*send_method)());

	std::ostream& stream();

	static void Fail() ;

	struct LogMessageData;
private:

	static int64 num_messages_[NUM_SEVERITIES];

	//Log 数据存放在分配出来的结构体中减少堆消耗
	LogMessageData* allocated_;
	LogMessageData* data_;

	friend class LogDestination;
};

struct CheckOpString {
  CheckOpString(std::string* str) : str_(str) { }
  operator bool() const {
    return SUKEY_PREDICT_BRANCH_NOT_TAKEN(str_ != NULL);
  }
  std::string* str_;
};


class SUKEY_LOG_DLL_DECL LogMessageFatal:public LogMessage
{
public:
	LogMessageFatal(const char* file, int line);
  LogMessageFatal(const char* file, int line, const CheckOpString& result);
  ~LogMessageFatal() ;
};

class SUKEY_LOG_DLL_DECL LogMessageVoidify
{
public:
	LogMessageVoidify()
	{

	}
	//&取地址符 比<<优先级 低 比?: 优先级高
	void operator&(std::ostream&)
	{
	
	}
};

//输出到别的地方 继承它的必须是线程安全的
class SUKEY_LOG_DLL_DECL LogSink
{
public:
	virtual ~LogSink();

	virtual void send(LogSeverity severity, const char* full_filename,
                    const char* base_filename, int line,
                    const struct ::tm* tm_time,
                    const char* message, size_t message_len) = 0;
	virtual void WaitTillSent();
	//给send用的
	static std::string ToString(LogSeverity severity, const char* file, int line,
                              const struct ::tm* tm_time,
                              const char* message, size_t message_len);
};

SUKEY_LOG_DLL_DECL const std::vector<std::string>& GetLoggingDirectories();

namespace BASE_NAMESPACE
{

class SUKEY_LOG_DLL_DECL Logger
{
public:
	virtual ~Logger(){}
	virtual void Write(bool force_flush,time_t timestamp,const char* message,int message_len)=0;
	virtual void Flush()=0;
	virtual uint32 LogSize() = 0;
};

class SUKEY_LOG_DLL_DECL NullStream : public LogMessage::LogStream {
 public:
  // Initialize the LogStream so the messages can be written somewhere
  // (they'll never be actually displayed). This will be needed if a
  // NullStream& is implicitly converted to LogStream&, in which case
  // the overloaded NullStream::operator<< will not be invoked.
  NullStream() : LogMessage::LogStream(message_buffer_, 1, 0) { }
  NullStream(const char* /*file*/, int /*line*/,
             const CheckOpString& /*result*/) :
      LogMessage::LogStream(message_buffer_, 1, 0) { }
  NullStream &stream() { return *this; }
 private:
  // A very short buffer for messages (which we discard anyway). This
  // will be needed if NullStream& converted to LogStream& (e.g. as a
  // result of a conditional expression).
  char message_buffer_[2];
};

}

}

#endif