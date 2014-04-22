#ifndef BASE_COMMANDLINEFLAGS_H__
#define BASE_COMMANDLINEFLAGS_H__

#include "config.h"
#include "logging.h"
#include <stdlib.h>
#include <string.h>

#define DECLARE_VARIABLE(type,shorttype,name,tn)\
	namespace fL##shorttype{\
	extern SUKEY_LOG_DLL_DECL type FLAGS_##name;\
}

#define DEFINE_VARIABLE(type,shorttype,name,value,meaning,tn)\
	namespace fL##shorttype\
	{\
	SUKEY_LOG_DLL_DECL type FLAGS_##name(value);\
	char FLAGS_no##name;\
	}\
	using fL##shorttype::FLAGS_##name

#define DECLARE_bool(name)\
	DECLARE_VARIABLE(bool,B,name,bool)
#define DEFINE_bool(name,value,meaning)\
	DEFINE_VARIABLE(bool,B,name,value,meaning,bool)

#define DECLARE_int32(name) \
  DECLARE_VARIABLE(SUKEY_NAMESPACE::int32, I, name, int32)
#define DEFINE_int32(name, value, meaning) \
  DEFINE_VARIABLE(SUKEY_NAMESPACE::int32, I, name, value, meaning, int32)

#define DECLARE_string(name)                                            \
  namespace fLS {                                                       \
    extern SUKEY_LOG_DLL_DECL std::string& FLAGS_##name;              \
  }                                                                     \
  using fLS::FLAGS_##name
#define DEFINE_string(name, value, meaning)                             \
  namespace fLS {                                                       \
    std::string FLAGS_##name##_buf(value);                              \
    SUKEY_LOG_DLL_DECL std::string& FLAGS_##name = FLAGS_##name##_buf; \
    char FLAGS_no##name;                                                \
  }                                                                     \
  using fLS::FLAGS_##name

#define LOG_DEFINE_bool(name,value,meaning)\
	DEFINE_bool(name,EnvToBool("LOG_" #name,value),meaning)

#define LOG_DEFINE_int32(name,value,meaning)\
	DEFINE_int32(name,EnvToInt("LOG_" #name,value),meaning)

#define LOG_DEFINE_string(name, value, meaning)\
	DEFINE_string(name, EnvToString("LOG_"#name, value), meaning)



#define EnvToBool(envname,default_value)\
	(!getenv(envname)?(default_value):memchr("tTyY1\0",getenv(envname)[0],6)!=NULL)

#define EnvToString(envname, default_value)   \
  (!getenv(envname) ? (default_value) : getenv(envname))

#define EnvToInt(envname,default_value)\
	(!getenv(envname) ? (default_value) : strtol(getenv(envname), NULL, 10))

#endif