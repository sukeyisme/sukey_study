#define SUKEY_NAMESPACE sukey
#define _START_SUKEY_NAMESPACE_ namespace sukey {

#define _END_SUKEY_NAMESPACE_ }

#define _START_BASE_NAMESPACE_ namespace base{
#define _END_BASE_NAMESPACE_ }

#define _START_LOG_INTERNAL_NAMESPACE_ namespace log_internal_ {
#define _END_LOG_INTERNAL_NAMESPACE_ }

#ifndef SUKEY_LOG_DLL_DECL
# define SUKEY_LOG_IS_A_DLL  1  //如果是静态编译 不是DLL就不需要  
# define SUKEY_LOG_DLL_DECL  __declspec(dllexport)
# define SUKEY_LOG_DLL_DECL_FOR_UNITTESTS  __declspec(dllimport)
#endif