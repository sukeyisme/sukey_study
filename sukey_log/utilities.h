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

_START_SUKEY_NAMESPACE_

namespace log_internal_
{
	pid_t GetTID();
}
_END_SUKEY_NAMESPACE_

using namespace SUKEY_NAMESPACE::log_internal_;
#endif