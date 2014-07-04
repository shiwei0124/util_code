/*
 * CopyRight:	HangZhou Hikvision System Technology Co., Ltd. All	Right Reserved.
 * FileName:	TimedWait.h
 * Desc:		OS Layer Component.
 * Author:		schina
 * Date:		2009-11-10
 * Contact:		zhaoyiji@hikvision.com.cn
 * History:		Created By Zhaoyiji 2009-11-10
 * */

#ifndef __TIMED_WAIT__
#define __TIMED_WIAT__

#include "HPR_Cond.h"
#include "HPR_Mutex.h"
#include "HPR_Semaphore.h"
#include "HPR_Thread.h"
#include "HPR_Utils.h"
#include "HPR_Time.h"

#if defined (OS_APPLE)

//初始化Timedwait资源
HPR_INT32 HPR_InitTimeWait_Inter();

//销毁Timedwair资源
void HPR_FiniTimedWait_Inter();

//带超时等待条件变量
HPR_INT32 CondTimedWait(HPR_COND_T* pCond, HPR_MUTEX_T* pMutex, HPR_UINT32 nTimeOut);

//带超时等待信号量
HPR_INT32 SemTimedWait(HPR_SEM_T* pSem, HPR_UINT32 nTimeOut);

#endif

#endif
