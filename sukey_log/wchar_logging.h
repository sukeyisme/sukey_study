#ifndef WCHAR_LOGGING_H_
#define WCHAR_LOGGING_H_

#include <wchar.h>
#include <stdarg.h>
#include <iostream>
#include <string>
inline std::ostream& operator<<(std::ostream& out, const wchar_t* str)
{
  size_t len = wcsrtombs(NULL, &str, 0, NULL);
  char* buf = (char*)malloc(len + 1);
  buf[len] = 0;
  wcsrtombs(buf, &str, len, NULL);
  out << buf;
  free(buf);
  return out;
}

inline void LogPrintf(char* out,size_t size,const wchar_t* format,...)
{
	va_list args;
	va_start(args,format);

	wchar_t *buf= (wchar_t*)malloc(size*sizeof(wchar_t));
	buf[size-1]=0;
	vswprintf_s(buf,size,format,args);
	const wchar_t *buf_in = buf;
	wcsrtombs(out, &buf_in, size, NULL);
	free(buf);

	va_end(args);
}

//inline std::ostream& operator<<(std::ostream& out, const std::wstring& str) 
//{
//  return operator<<(out, str.c_str());
//}

#endif