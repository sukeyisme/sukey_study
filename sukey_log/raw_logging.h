#ifndef BASE_RAW_LOGGING_H_
#define BASE_RAW_LOGGING_H_

#include "config.h"
_START_SUKEY_NAMESPACE_
#include "log_severity.h"

SUKEY_LOG_DLL_DECL void RawLog__SetLastTime(const struct tm& t, int usecs);

_END_SUKEY_NAMESPACE_
#endif