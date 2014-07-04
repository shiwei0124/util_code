/*
 * CopyRight:	HangZhou Hikvision System Technology Co., Ltd. All	Right Reserved.
 * FileName:	HRP_Cond.h
 * Desc:		OS Layer Component.
 * Author:		schina
 * Date:		2009-03-02
 * Contact:		zhaoyiji@hikvision.com.cn
 * History:		Created By Zhaoyiji 2009-03-02
 * */

/*! 
	\file HPR_Cond.h
	\brief HPR Cond Manager.

	Details.
*/

#ifndef __HPR_COND_T__
#define __HPR_COND_T__

#include "HPR_Config.h"
#include "HPR_Types.h"
#include "HPR_Mutex.h"

#if defined(OS_WINDOWS)
	typedef struct HPR_COND* HPR_COND_T;
#elif defined(OS_POSIX)
	typedef pthread_cond_t HPR_COND_T;
#else
	#error HPR_OS_NOT_SUPPORT
#endif

/**
 * HPR_CondCreate create var cond 
 * @param pCond (OUT) var cond to create.
 * @return 0 success, -1 fail.
 * @sa HPR_CondDestroy(),HPR_CondWait(),HPR_CondTimedWait(),HPR_CondSignal(),HPR_CondBroadCast()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_CondCreate(HPR_COND_T* pCond);

/**
 * HPR_CondDestroy destroy var cond 
 * @param pCond (IN) var cond to destroy.
 * @return 0 success, -1 fail.
 * @sa HPR_CondCreate(),HPR_CondWait(),HPR_CondTimedWait(),HPR_CondSignal(),HPR_CondBroadCast()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_CondDestroy(HPR_COND_T* pCond);

/**
 * HPR_CondWait create var cond 
 * @param pCond (IN) var cond to wait.
 * @param pMutex (IN) mutex used in cond.
 * @return 0 success, -1 fail.
 * @sa HPR_CondDestroy(),HPR_CondCreate(),HPR_CondTimedWait(),HPR_CondSignal(),HPR_CondBroadCast()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_CondWait(HPR_COND_T* pCond, HPR_MUTEX_T* pMutex);

/**
 * HPR_CondTimedWait create var cond 
 * @param pCond (IN) var cond to wait.
 * @param pMutex (IN) mutex used in cond.
 * @return 0 success, -1 fail.
 * @sa HPR_CondDestroy(),HPR_CondWait(),HPR_CondTimedCreate(),HPR_CondSignal(),HPR_CondBroadCast()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_CondTimedWait(HPR_COND_T* pCond, HPR_MUTEX_T* pMutex, HPR_UINT32 nTimeOut = HPR_INFINITE);

/**
 * HPR_CondSignal create var cond 
 * @param pCond (IN) var cond to signal.
 * @return 0 success, -1 fail.
 * @sa HPR_CondDestroy(),HPR_CondWait(),HPR_CondTimedWait(),HPR_CondCreate(),HPR_CondBroadCast()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_CondSignal(HPR_COND_T* pCond);

/**
 * HPR_CondBroadCast create var cond 
 * @param pCond (IN) var cond to broadcast signal.
 * @return 0 success, -1 fail.
 * @sa HPR_CondDestroy(),HPR_CondWait(),HPR_CondTimedWait(),HPR_CondSignal(),HPR_CondCreate()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_CondBroadCast(HPR_COND_T* pCond);

#endif
