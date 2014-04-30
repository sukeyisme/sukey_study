#ifndef _LOG_SEVERITY_H_
#define _LOG_SEVERITY_H_

typedef int LogSeverity;
const int LOG_INFO =0,LOG_WARNING =1,LOG_ERROR =2,LOG_FATAL =3,NUM_SEVERITIES  =4;
#ifndef LOG_NO_ABBREVIATED_SEVERITIES
#ifdef ERROR
# error ERROR macro is defined. Define LOG_NO_ABBREVIATED_SEVERITIES before including logging.h. See the document for detail.
#endif
const int INFO = LOG_INFO, WARNING = LOG_WARNING,ERROR = LOG_ERROR, FATAL = LOG_FATAL;
#endif

extern SUKEY_LOG_DLL_DECL const char* const LogSeverityNames[NUM_SEVERITIES];
#endif