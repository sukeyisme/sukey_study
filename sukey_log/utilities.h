#ifndef UTILITIES_H__
#define UTILITIES_H__

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
# define OS_WINDOWS
#elif defined(__CYGWIN__) || defined(__CYGWIN32__)
# define OS_CYGWIN
#elif defined(linux) || defined(__linux) || defined(__linux__)
# define OS_LINUX
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
# define OS_MACOSX
#elif defined(__FreeBSD__)
# define OS_FREEBSD
#elif defined(__NetBSD__)
# define OS_NETBSD
#elif defined(__OpenBSD__)
# define OS_OPENBSD
#else
// TODO(hamaji): Add other platforms.
#endif

#include "mutex.h"
#include <string>

#if defined(OS_WINDOWS)
#include "port.h"
#endif

#include "config.h"
#include "logging.h"

namespace SUKEY_NAMESPACE
{

namespace LOG_INTERNAL_NAMESPACE
{
	const char* ProgramInvocationShortName();

	int64 CycleClock_Now();

	int64 UsecToCycles(int64 usec);

	typedef double WallTime;
	WallTime WallTime_Now();

	int32 GetMainThreadPid();
	bool PidHasChanged();

	pid_t GetTID();

	const std::string& MyUserName();

	const char* const_basename(const char* filepath);

	struct CrashReason {
  CrashReason() : filename(0), line_number(0), message(0), depth(0) {}

  const char* filename;
  int line_number;
  const char* message;

  // We'll also store a bit of stack trace context at the time of crash as
  // it may not be available later on.
  void* stack[32];
  int depth;
};

}

}

using namespace SUKEY_NAMESPACE::LOG_INTERNAL_NAMESPACE;
#endif