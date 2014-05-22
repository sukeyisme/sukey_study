#define SUKEY_NAMESPACE sukey
#define BASE_NAMESPACE base
#define LOG_INTERNAL_NAMESPACE log_internal
#define	BASE_LOGGING_NAMESPACE base_logging
#define FLAGS_NAMESPACE	flags

#ifndef SUKEY_STRIP_LOG
#define SUKEY_STRIP_LOG	0
#endif

#ifndef SUKEY_LOG_DLL_DECL
#define SUKEY_LOG_IS_A_DLL  1  //如果是静态编译 不是DLL就不需要 
#ifdef SUKEY_LOG_SOURCE 
#define SUKEY_LOG_DLL_DECL  __declspec(dllexport)
#else
#define SUKEY_LOG_DLL_DECL  __declspec(dllimport)
#endif
#ifdef SUKEY_FLAGS_SOURCE
#define FLAGS_DLL_DECL __declspec(dllexport)
#else
#define FLAGS_DLL_DECL __declspec(dllimport)
#endif
#endif