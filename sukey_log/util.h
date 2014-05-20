#ifndef FLAGS_UTIL_H_
#define FLAGS_UTIL_H_

namespace FLAGS_NAMESPACE
{
	extern FLAGS_DLL_DECL void (*gflags_exitfunc)(int);
	inline int SafeFOpen(FILE **fp, const char* fname, const char *mode)
	{
	#if defined(_MSC_VER) && _MSC_VER >= 1400
		return fopen_s(fp, fname, mode);
	#else
		assert(fp != NULL);
		*fp = fopen(fname, mode);
			// errno only guaranteed to be set on failure
		return ((*fp == NULL) ? errno : 0);
	#endif
	}
}
#endif