#ifndef BASE_RAW_LOGGING_H_
#define BASE_RAW_LOGGING_H_

#include "config.h"
namespace SUKEY_NAMESPACE
{
#include "log_severity.h"

SUKEY_LOG_DLL_DECL void RawLog__SetLastTime(const struct tm& t, int usecs);

}
#endif