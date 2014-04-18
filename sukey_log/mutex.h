#ifndef SUKEY_MUTEX_H_
#define SUKEY_MUTEX_H_

#include "config.h"
#include <windows.h>

namespace log_internal_
{
	class Mutex
	{
	public:
		inline Mutex();
		inline ~Mutex();

		inline void Lock();
		inline void Unlock();
		
		inline void ReaderLock();
		inline void ReaderUnlock();
		inline void WriterLock() { Lock(); }
		inline void WriterUnlock() { Unlock(); }
	
	private:
		MutexType mutex_;	//��ͬ����ϵͳ��Ӧ��ͬ��������
		volatile bool is_safe_;


	}
}

#endif