#include "utilities.h"

#include <stdio.h>
#include <stdlib.h>

#include <signal.h>
#ifdef HAVE_SYS_TIME_H
# include <sys/time.h>
#endif
#include <time.h>
#if defined(HAVE_SYSCALL_H)
#include <syscall.h>                 // for syscall()
#elif defined(HAVE_SYS_SYSCALL_H)
#include <sys/syscall.h>                 // for syscall()
#endif
#ifdef HAVE_SYSLOG_H
# include <syslog.h>
#endif

using std::string;

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

_START_SUKEY_NAMESPACE_

static const char* g_program_invocation_short_name = NULL;

static pthread_t g_main_thread_id;

_START_LOG_INTERNAL_NAMESPACE_

const char* ProgramInvocationShortName() {
if (g_program_invocation_short_name != NULL) {
  return g_program_invocation_short_name;
} else {
  // TODO(hamaji): Use /proc/self/cmdline and so?
  return "UNKNOWN";
}
}

#ifdef OS_WINDOWS
static int gettimeofday(struct timeval *tv, void* tz) {
#define EPOCHFILETIME (116444736000000000ULL)
  FILETIME ft;
  LARGE_INTEGER li;
  uint64 tt;

  GetSystemTimeAsFileTime(&ft);
  li.LowPart = ft.dwLowDateTime;
  li.HighPart = ft.dwHighDateTime;
  tt = (li.QuadPart - EPOCHFILETIME) / 10;
  tv->tv_sec = tt / 1000000;
  tv->tv_usec = tt % 1000000;

  return 0;
}
#endif

int64 CycleClock_Now() {
  // TODO(hamaji): temporary impementation - it might be too slow.
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return static_cast<int64>(tv.tv_sec) * 1000000 + tv.tv_usec;
}

int64 UsecToCycles(int64 usec) {
  return usec;
}

WallTime WallTime_Now()
{
	return CycleClock_Now() * 0.000001;
}
static int32 g_main_thread_pid = getpid();
int32 GetMainThreadPid() {
  return g_main_thread_pid;
}

bool PidHasChanged() {
  int32 pid = getpid();
  if (g_main_thread_pid == pid) {
    return false;
  }
  g_main_thread_pid = pid;
  return true;
}

pid_t GetTID() {
  // On Linux and MacOSX, we try to use gettid().
#if defined OS_LINUX || defined OS_MACOSX
#ifndef __NR_gettid
#ifdef OS_MACOSX
#define __NR_gettid SYS_gettid
#elif ! defined __i386__
#error "Must define __NR_gettid for non-x86 platforms"
#else
#define __NR_gettid 224
#endif
#endif
  static bool lacks_gettid = false;
  if (!lacks_gettid) {
    pid_t tid = syscall(__NR_gettid);
    if (tid != -1) {
      return tid;
    }
    // Technically, this variable has to be volatile, but there is a small
    // performance penalty in accessing volatile variables and there should
    // not be any serious adverse effect if a thread does not immediately see
    // the value change to "true".
    lacks_gettid = true;
  }
#endif  // OS_LINUX || OS_MACOSX

  // If gettid() could not be used, we use one of the following.
#if defined OS_LINUX
  return getpid();  // Linux:  getpid returns thread ID when gettid is absent
#elif defined OS_WINDOWS || defined OS_CYGWIN
  return GetCurrentThreadId();
#else
  // If none of the techniques above worked, we use pthread_self().
  return (pid_t)(uintptr_t)pthread_self();
#endif
}

static string g_my_user_name;
const string& MyUserName() {
  return g_my_user_name;
}

const char* const_basename(const char* filepath) {
  const char* base = strrchr(filepath, '/');
#ifdef OS_WINDOWS  // Look for either path separator in Windows
  if (!base)
    base = strrchr(filepath, '\\');
#endif
  return base ? (base+1) : filepath;
}

_END_LOG_INTERNAL_NAMESPACE_
_END_SUKEY_NAMESPACE_