/*
 * CopyRight:	HangZhou Hikvision System Technology Co., Ltd. All	Right Reserved.
 * FileName:	HRP_ThreadPool.h
 * Desc:		OS Layer Component.
 * Author:		guanguochen
 * Date:		2009-03-30
 * Contact:		guanguochen@hikvision.com.cn
 * History:		Created By guanguochen 2009-03-30
 * */

/*! 
	\file HPR_ThreadPool.h
	\brief HPR ThreadPool Manager.

	Details.
*/

#ifndef __HPR_THREADPOOL_H__	
#define __HPR_THREADPOOL_H__

#include "HPR_Config.h"
#include "HPR_Types.h"
#include "HPR_Mutex.h"
#include "HPR_Semaphore.h"
#include "HPR_Thread.h"

//#include <vector>
//#include <algorithm>
//using namespace std;

/**
 * HPR_ThreadPool_Create
 * @param InitThreadNum (IN) init thread number.
 * @param MaxThreadNum (IN) max thread number.
 * @return NULL fail, ese success.
 * @sa HPR_ThreadPool_Work(),HPR_ThreadPool_Destroy()
 */
HPR_DECLARE HPR_HANDLE CALLBACK HPR_ThreadPool_Create(HPR_UINT32 InitThreadNum, HPR_UINT32 MaxThreadNum);
HPR_DECLARE HPR_HANDLE CALLBACK HPR_ThreadPool_CreateEx(HPR_UINT32 InitThreadNum, HPR_UINT32 MaxThreadNum, HPR_UINT32 StackSize);
HPR_DECLARE HPR_HANDLE CALLBACK HPR_ThreadPool_CreateFlex(HPR_UINT32 InitThreadNum, HPR_UINT32 MaxThreadNum, 
                                                          HPR_UINT32 StackSize, HPR_UINT32 TimeOut);

/**
 * HPR_ThreadPool_Destroy
 * @param ThreadPoolHandle (IN) created by HPR_ThreadPool_Create
 * @return 0 success, -1 fail.
 * @sa HPR_ThreadPool_Create(),HPR_ThreadPool_Work()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_ThreadPool_Destroy(HPR_HANDLE ThreadPoolHandle);

/**
 * HPR_ThreadPool_Work
 * @param ThreadPoolHandle (IN) created by HPR_ThreadPool_Create
 * @param WorkRoutine (IN) do the job function.
 * @param Params (IN) parameter pass to WorkRoutine.
 * @return 0 success, -1 fail.
 * @sa HPR_ThreadPool_Create(),HPR_ThreadPool_Destroy()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_ThreadPool_Work(HPR_HANDLE ThreadPoolHandle, HPR_VOIDPTR(CALLBACK *WorkRoutine)(HPR_VOIDPTR), HPR_VOIDPTR Params);
HPR_DECLARE HPR_INT32 CALLBACK HPR_ThreadPool_WorkEx(HPR_HANDLE ThreadPoolHandle, HPR_VOIDPTR(CALLBACK *WorkRoutine)(HPR_VOIDPTR), HPR_VOIDPTR Params, HPR_BOOL bWaitForIdle);

typedef HPR_VOIDPTR (CALLBACK *WORKROUTINE)(HPR_VOIDPTR);

#if 1
class HPR_ThreadPool
{
public:
	HPR_ThreadPool(HPR_VOID);
	~HPR_ThreadPool(HPR_VOID);

public:
	HPR_INT32 Create(HPR_UINT32 InitThreadNum, HPR_UINT32 MaxThreadNum);
	HPR_INT32 Destroy(HPR_VOID);
	HPR_INT32 Work(HPR_VOIDPTR(CALLBACK *WorkRoutine)(HPR_VOIDPTR), HPR_VOIDPTR Params);

private:
	HPR_HANDLE m_threadpoolhandle;
};

#endif

#endif
