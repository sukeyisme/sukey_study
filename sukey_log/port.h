#ifndef PORT_H_
#define PORT_H_
#include "config.h"

#include <windows.h>
#include <winsock.h>         /* for gethostname */
#include <io.h>              /* because we so often use open/close/etc */
#include <direct.h>          /* for _getcwd() */
#include <process.h>         /* for _getpid() */
#include <stdio.h>           /* read in vsnprintf decl. before redifining it */
#include <stdarg.h>          /* template_dictionary.cc uses va_copy */
#include <string.h>          /* for _strnicmp(), strerror_s() */
#include <time.h>  

//4244: 类型
//4355: this 指针不能在构造函数里
//4251: 指出一个私有结构没有DLLexport
//4996: unsafe 函数
#pragma warning (disable:4996 4251 4355 4244)

typedef int pid_t;
#define getpid  _getpid
inline struct tm* localtime_r(const time_t* timep, struct tm* result)
{
  localtime_s(result, timep);
  return result;
}
extern int snprintf(char *str, size_t size,const char *format, ...);
#endif