/*
 * CopyRight:	HangZhou Hikvision System Technology Co., Ltd. All	Right Reserved.
 * FileName:	HRP_Mutex.h
 * Desc:		OS Layer Component.
 * Author:		schina
 * Date:		2009-03-02
 * Contact:		zhaoyiji@hikvision.com.cn
 * History:		Created By Zhaoyiji 2009-03-02
 * */

/*! 
	\file HPR_Mutex.h
	\brief HPR Mutex Manager.

	Details.
*/

#ifndef __HPR_MUTEX_H__
#define __HPR_MUTEX_H__

#include "HPR_Config.h"
#include "HPR_Types.h"

#if defined(OS_WINDOWS)
	typedef CRITICAL_SECTION HPR_MUTEX_T;
	#define HPR_MUTEX_FAST			0
	#define HPR_MUTEX_RECURSIVE		0
	#define HPR_MUTEX_ERRORCHECK	0
#elif defined(OS_POSIX)
#include <pthread.h>
	typedef pthread_mutex_t HPR_MUTEX_T;
	///ª•≥‚À¯¿‡–Õª•≥‚
#if defined (__linux__)
	#define HPR_MUTEX_FAST			PTHREAD_MUTEX_FAST_NP
	#define HPR_MUTEX_RECURSIVE		PTHREAD_MUTEX_RECURSIVE_NP
	#define HPR_MUTEX_ERRORCHECK	PTHREAD_MUTEX_ERRORCHECK_NP
#elif (defined(OS_APPLE) || defined(OS_S60) || defined(OS_ANDROID))
	#define HPR_MUTEX_FAST			PTHREAD_MUTEX_NORMAL
	#define HPR_MUTEX_RECURSIVE		PTHREAD_MUTEX_RECURSIVE
	#define HPR_MUTEX_ERRORCHECK	PTHREAD_MUTEX_ERRORCHECK
#endif
#else
	#error OS Not Implement yet.
#endif

/**
 * HPR_MutexCreate create mutex
 * @param pMutex (OUT) mutex to create
 * @param iFlag (IN) mutex flag.
 * @return 0 success, -1 fail.
 * @sa HPR_MutexDestroy
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_MutexCreate(HPR_MUTEX_T* pMutex, HPR_INT32 iFlag = -1); 

/**
 * HPR_MutexDestroy destroy mutex created by HPR_MutexCreate.
 * @param pMutex (IN) mutex created by HPR_MutexCreate.
 * @return 0 success, -1 fail.
 * @sa HPR_MutexCreate
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_MutexDestroy(HPR_MUTEX_T* pMutex); 

/**
 * HPR_MutexLock lock mutex
 * @param pMutex (IN) mutex created by HPR_MutexCreate.
 * @return 0 success, -1 fail.
 * @sa HPR_MutexCreate,HPR_MutexTryLock(),HPR_MutexUnlock().
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_MutexLock(HPR_MUTEX_T* pMutex); 

/**
 * HPR_MutexTryLock try lock mutex
 * @param pMutex (IN) mutex created by HPR_MutexCreate.
 * @return 0 success, -1 fail.
 * @sa HPR_MutexCreate(),HPR_MutexUnlock(),HPR_MutexLock().
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_MutexTryLock(HPR_MUTEX_T* pMutex);

/**
 * HPR_MutexUnlock unlock mutex
 * @param pMutex (IN) mutex created by HPR_MutexCreate.
 * @return 0 success, -1 fail.
 * @sa HPR_MutexCreate(),HPR_MutexTryLock(),HPR_MutexLock()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_MutexUnlock(HPR_MUTEX_T* pMutex); 

class HPR_DECLARE_CLASS HPR_Mutex
{
public:
	HPR_Mutex();
	HPR_Mutex(HPR_UINT32 nFlag);
	~HPR_Mutex();

	HPR_INT32 Lock();
	HPR_INT32 Unlock();
	HPR_INT32 TryLock();

private:
	HPR_MUTEX_T m_mutex;
};

#endif


