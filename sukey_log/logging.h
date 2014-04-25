#ifndef _LOGGING_H_
#define _LOGGING_H_

#include "config.h"
#include "log_severity.h"
#include <errno.h>
#include <string.h>
#include <time.h>
#include <iosfwd>
#include <ostream>
#include <sstream>
#include <string>
#if 0
# include <unistd.h>
#endif
#include <vector>

namespace SUKEY_NAMESPACE
{
#if 0
#elif 0
#elif 1 //VC7 ��ʽ
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

//����������������Ҫ��LOG��
#if SUKEY_STRIP_LOG ==0
#define COMPACT_SUKEY_LOG_INFO sukey::LogMessage(\
	__FILE__,__LINE__)
#else
#define COMPACT_SUKEY_LOG_INFO google::NullStream()
#endif

#if SUKEY_STRIP_LOG <=1
#define COMPACT_SUKEY_LOG_WARNING sukey::LogMessage(\
	__FILE__,__LINE__,sukey::GLOG_WARNING)
#else
#define COMPACT_SUKEY_LOG_WARNING google::NullStream()
#endif

#if SUKEY_STRIP_LOG <=2
#define COMPACT_SUKEY_LOG_ERROR sukey::LogMessage( \
      __FILE__, __LINE__, sukey::GLOG_ERROR)
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

#endif

DECLARE_bool(log_prefix);

DECLARE_int32(minloglevel);

#ifdef MUST_UNDEF_FLAGS_DECLARE_MACROS
#undef MUST_UNDEF_GFLAGS_DECLARE_MACROS
#undef DECLARE_VARIABLE
#undef DECLARE_bool
#undef DECLARE_int32
#endif


//��ͨLOG��
#define LOG(severity) COMPACT_SUKEY_LOG_##severity.stream()

#ifndef NDEBUG
#define DLOG(severity)\
	(true) ? (void)0 : sukey::LogMessageVoidify() & LOG(severity)
#else

#endif

//sukey �ռ��ڵĸ�����
_START_SUKEY_NAMESPACE_
namespace base_logging
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

//LogStream�̳���std::ostream ��һ��û��DLL�ӿڵ��� ����VC�ᱨ���� Ȼ�� MSDN˵���Ժ����������
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

	//������logging.*֮��ֱ�ӵ���
	void SendToLog();

	void Init(const char* file, int line, LogSeverity severity,
            void (LogMessage::*send_method)());

	std::ostream& stream();

	struct LogMessageData;
private:

	static int64 num_messages_[NUM_SEVERITIES];

	//Log ���ݴ���ڷ�������Ľṹ���м��ٶ�����
	LogMessageData* allocated_;
	LogMessageData* data_;

	friend class LogDestination;

};

class SUKEY_LOG_DLL_DECL LogMessageVoidify
{
public:
	LogMessageVoidify()
	{

	}
	//&ȡ��ַ�� ��<<���ȼ� �� ��?: ���ȼ���
	void operator&(std::ostream&)
	{
	
	}
};

//�������ĵط� �̳����ı������̰߳�ȫ��
class SUKEY_LOG_DLL_DECL LogSink
{
public:
	virtual ~LogSink();

	virtual void send(LogSeverity severity, const char* full_filename,
                    const char* base_filename, int line,
                    const struct ::tm* tm_time,
                    const char* message, size_t message_len) = 0;
	virtual void WaitTillSent();
	//��send�õ�
	static std::string ToString(LogSeverity severity, const char* file, int line,
                              const struct ::tm* tm_time,
                              const char* message, size_t message_len);
};
_END_SUKEY_NAMESPACE_

#endif