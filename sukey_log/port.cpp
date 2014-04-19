#include "port.h"
int snprintf(char *str, size_t size, const char *format, ...) {
  va_list ap;
  va_start(ap, format);
  const int r = vsnprintf(str, size, format, ap);
  va_end(ap);
  return r;
}