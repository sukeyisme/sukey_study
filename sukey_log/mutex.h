#ifndef SUKEY_MUTEX_H_
#define SUKEY_MUTEX_H_

#include "config.h"

#if defined(NO_THREADS)

#elif defined(_WIN32) || defined(__CYGWIN32__) || defined(__CYGWIN64__)
# ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
# endif

#ifndef NOGDI
#define NOGDI
#endif

#include <windows.h>
typedef CRITICAL_SECTION MutexType;

#elif defined(HAVE_PTHREAD) && defined(HAVE_RWLOCK)

#elif defined(HAVE_PTHREAD)

#else

#endif

#include <assert.h>
#include <stdlib.h> 

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

		inline void SetIsSafe() {is_safe_ = true;}

		Mutex(Mutex*){}
		Mutex(const Mutex&);
		void operator=(const Mutex&);
	};


#if defined(NO_THREADS)

#elif defined(_WIN32) || defined(__CYGWIN32__) || defined(__CYGWIN64__)
Mutex::Mutex()				{ InitializeCriticalSection(&mutex_); SetIsSafe(); }
Mutex::~Mutex()       { DeleteCriticalSection(&mutex_); }
void Mutex::Lock()    { if (is_safe_) EnterCriticalSection(&mutex_); }
void Mutex::Unlock()  { if (is_safe_) LeaveCriticalSection(&mutex_); }
void Mutex::ReaderLock() {Lock();}
void Mutex::ReaderUnlock() { Unlock(); }
#elif defined(HAVE_PTHREAD) && defined(HAVE_RWLOCK)

#elif defined(HAVE_PTHREAD)

#endif

//��д��
class MutexLock {
 public:
  explicit MutexLock(Mutex *mu) : mu_(mu) { mu_->Lock(); }
  ~MutexLock() { mu_->Unlock(); }
 private:
  Mutex * const mu_;
  
  MutexLock(const MutexLock&);
  void operator=(const MutexLock&);
};

class ReaderMutexLock {
 public:
  explicit ReaderMutexLock(Mutex *mu) : mu_(mu) { mu_->ReaderLock(); }
  ~ReaderMutexLock() { mu_->ReaderUnlock(); }
 private:
  Mutex * const mu_;

  ReaderMutexLock(const ReaderMutexLock&);
  void operator=(const ReaderMutexLock&);
};

class WriterMutexLock {
 public:
  explicit WriterMutexLock(Mutex *mu) : mu_(mu) { mu_->WriterLock(); }
  ~WriterMutexLock() { mu_->WriterUnlock(); }
 private:
  Mutex * const mu_;
  
  WriterMutexLock(const WriterMutexLock&);
  void operator=(const WriterMutexLock&);
};

//�ñ���ʱ���� ��ֹ�ֲ���������û�г����� ��������
//COMPILE_ASSERT ��ʵ��
//template <bool>
//struct CompileAssert {
//};
//
//#undef COMPILE_ASSERT
//#define COMPILE_ASSERT(expr, msg) /
//  typedef CompileAssert<(bool(expr))> msg[bool(expr) ? 1 : -1]
//���õ��������±겻��Ϊ����

#define MutexLock(x)				COMPILE_ASSERT(0, mutex_lock_decl_missing_var_name)
#define ReaderMutexLock(x)	COMPILE_ASSERT(0, rmutex_lock_decl_missing_var_name)
#define WriterMutexLock(x)	COMPILE_ASSERT(0, wmutex_lock_decl_missing_var_name)

}

using namespace log_internal_;

#endif