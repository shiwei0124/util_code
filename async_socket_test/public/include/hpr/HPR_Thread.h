/*
 * CopyRight:	HangZhou Hikvision System Technology Co., Ltd. All	Right Reserved.
 * FileName:	HRP_Thread.h
 * Desc:		OS Layer Component.
 * Author:		guanguochen
 * Date:		2009-03-30
 * Contact:		guanguochen@hikvision.com.cn
 * History:		Created By guanguochen 2009-03-30
 * */

/*! 
	\file HPR_Thread.h
	\brief HPR Thread Manager.

	Details.
*/

#ifndef __HPR_THREAD_H__	
#define __HPR_THREAD_H__

#include "HPR_Config.h"
#include "HPR_Types.h"

#if defined (OS_WINDOWS)
#define HPR_INVALID_THREAD (HPR_HANDLE)(NULL)
#elif defined (OS_POSIX)
#define HPR_INVALID_THREAD (HPR_HANDLE)(pthread_t)(-1)
#endif

/**
 * HPR_Thread_Create create thread.
 * @param StartAddress (IN) thread body
 * @param Params (IN) parameter input into thread body.
 * @param StackSize (IN) thread body stack size.
 * @param IsSuspend (IN) is thread suspend after create, only valid in windows.
 * @param Priority (IN) thread priority
 * @param SchedPolicy (IN) thread sched policy
 * @return NULL fail, else success.
 * @sa HPR_ThreadDetached_Create(),HPR_Thread_Wait()
 */
HPR_DECLARE HPR_HANDLE CALLBACK HPR_Thread_Create(HPR_VOIDPTR (CALLBACK *StartAddress)(HPR_VOIDPTR), HPR_VOID* Params, HPR_UINT32 StackSize,\
						HPR_BOOL IsSuspend = HPR_FALSE, HPR_INT32 Priority = 0, HPR_INT32 SchedPolicy = 0);

/**
 * HPR_ThreadDetached_Create create thread with detached attr.
 * @param StartAddress (IN) thread body
 * @param Params (IN) parameter input into thread body.
 * @param StackSize (IN) thread body stack size.
 * @return 0 success, -1 fail
 * @sa HPR_Thread_Create()
 */
HPR_DECLARE HPR_BOOL CALLBACK HPR_ThreadDetached_Create(HPR_VOIDPTR(CALLBACK *StartAddress)(HPR_VOIDPTR),HPR_VOID* Params, HPR_UINT32 StackSize);

/**
 * HPR_Thread_Wait wait thread and release resource.
 * @param ThreadHandle (IN) thread handle, created by HPR_Thread_Create
 * @return 0 success, -1 fail
 * @sa HPR_Thread_Create()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_Thread_Wait(HPR_HANDLE ThreadHandle);

/**
 * HPR_Thread_Suspend suspend thread
 * @param ThreadHandle (IN) thread handle, created by HPR_Thread_Create
 * @return 0 success, -1 fail
 * @sa HPR_Thread_Create()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_Thread_Suspend(HPR_HANDLE ThreadHandle);

/**
 * HPR_Thread_Resume resume thread, only valid in windows
 * @param ThreadHandle (IN) thread handle, created by HPR_Thread_Create
 * @return 0 success, -1 fail
 * @sa HPR_Thread_Create()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_Thread_Resume(HPR_HANDLE ThreadHandle);

/**
 * HPR_Thread_SetPriority set thread priority
 * @param ThreadHandle (IN) thread handle, created by HPR_Thread_Create
 * @param Priority (IN) sched priority
 * @return 0 success, -1 fail
 * @sa HPR_Thread_Create()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_Thread_SetPriority(HPR_HANDLE ThreadHandle, HPR_INT32 Priority);

/**
 * HPR_Thread_SetSchedPolicy set thread sched policy
 * @param ThreadHandle (IN) thread handle, created by HPR_Thread_Create
 * @param SchedPolicy (IN) sched policy
 * @return 0 success, -1 fail
 * @sa HPR_Thread_Create()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_Thread_SetSchedPolicy(HPR_HANDLE ThreadHandle, HPR_INT32 SchedPolicy);

/**
 * HPR_Thread_Exit exit thread.
 * @return 0 success, -1 fail.
 * @sa HPR_ThreadDetached_Create()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_Thread_Exit();

/**
 * HPR_Thread_GetId
 * @param void
 * @return 线程ID
 * @sa
 */
HPR_DECLARE HPR_HANDLE CALLBACK HPR_Thread_GetSelfId();


//////////////////////////////////////////////////////////////////////////


#define HPR_INVALID_TLS		(HPR_HANDLE)(-1)

/**
* HPR_ThreadTls_Create
* @param void
* @return 线程本地存储句柄
* @sa
*/
HPR_DECLARE HPR_HANDLE CALLBACK HPR_ThreadTls_Create();
HPR_DECLARE HPR_INT32 CALLBACK HPR_ThreadTls_SetValue(HPR_HANDLE hTls, HPR_VOIDPTR pVal);
HPR_DECLARE HPR_VOIDPTR CALLBACK HPR_ThreadTls_GetValue(HPR_HANDLE hTls);
HPR_DECLARE HPR_INT32 CALLBACK HPR_ThreadTls_Destroy(HPR_HANDLE hTls);


//////////////////////////////////////////////////////////////////////////

class HPR_Thread
{
public:
	HPR_Thread(HPR_VOID);
	~HPR_Thread(HPR_VOID);

	HPR_INT32 Create(HPR_VOIDPTR(CALLBACK *StartAddress)(HPR_VOIDPTR), HPR_VOIDPTR Params, HPR_UINT32 StackSize,\
						HPR_BOOL IsSuspend, HPR_INT32 Priority, HPR_INT32 SchedPolicy);
	HPR_INT32 Wait(HPR_VOID);
	HPR_INT32 Suspend(HPR_VOID);
	HPR_INT32 Resume(HPR_VOID);
	HPR_INT32 SetPriority(HPR_INT32 Priority);
	HPR_INT32 SetSchedPolicy(HPR_INT32 SchedPolicy);

private:
	HPR_HANDLE m_threadhandle;
};


#endif
