#define SUKEY_NAMESPACE sukey
#define _START_SUKEY_NAMESPACE_ namespace sukey {

#define _END_SUKEY_NAMESPACE_ }

#ifndef SUKEY_LOG_DLL_DECL
# define SUKEY_LOG_IS_A_DLL  1  //����Ǿ�̬���� ����DLL�Ͳ���Ҫ  
# define SUKEY_LOG_DLL_DECL  __declspec(dllexport)
# define SUKEY_LOG_DLL_DECL_FOR_UNITTESTS  __declspec(dllimport)
#endif