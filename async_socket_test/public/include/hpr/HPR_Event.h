/*
 * CopyRight:	HangZhou Hikvision System Technology Co., Ltd. All	Right Reserved.
 * FileName:	HRP_Event.h
 * Desc:		OS Layer Component.
 * Author:		schina
 * Date:		2009-03-02
 * Contact:		zhaoyiji@hikvision.com.cn
 * History:		Created By Zhaoyiji 2009-06-05
 * */

/*! 
	\file HPR_Event.h
	\brief HPR Event Manager.

	Details.
*/

#ifndef __HPR_EVENT_H__
#define __HPR_EVENT_H__

#include "HPR_Config.h"
#include "HPR_Types.h"
#include "HPR_Addr.h"

/**< 基数位 */
#define HPR_FD_BASE		1

/**< 操作位 */
#define HPR_FD_READ		(HPR_FD_BASE<<0)
#define HPR_FD_WRITE	(HPR_FD_BASE<<1)
#define HPR_FD_OOB		(HPR_FD_BASE<<2)
#define HPR_FD_ACCEPT	(HPR_FD_BASE<<3)
#define HPR_FD_CONNECT	(HPR_FD_BASE<<4)
#define HPR_FD_CLOSE	(HPR_FD_BASE<<5)
#define HPR_FD_QOS		(HPR_FD_BASE<<6)
#define HPR_GROUP_QOS	(HPR_FD_BASE<<7)

/**< 最大操作位数 */
#define HPR_FD_MAX_EVENT	8

#if defined (OS_WINDOWS)
#define HPR_WAIT_OBJECT_0 WAIT_OBJECT_0						/**<第一个事件	*/
#define HPR_WAIT_TIMEOUT		WAIT_TIMEOUT				/**<超时		*/
#define HPR_WAIT_INVALID_PARAM	((HPR_UINT32)0x7ffffffe)	/**<参数出错	*/
#define HPR_WAIT_INVALID_EVENT	((HPR_UINT32)0x7ffffffd)	/**<无效事件	*/
#define HPR_WAIT_FAILED			((HPR_UINT32)0x7ffffffc)	/**<等待失败	*/
typedef WSANETWORKEVENTS HPR_NETWORKEVENTS_T;
#elif defined (OS_POSIX)
#define HPR_WAIT_OBJECT_0		((HPR_UINT32)0x0)
#define HPR_WAIT_TIMEOUT		((HPR_UINT32)0x7fffffff)
#define HPR_WAIT_INVALID_PARAM	((HPR_UINT32)0x7ffffffe)
#define HPR_WAIT_INVALID_EVENT	((HPR_UINT32)0x7ffffffd)
#define HPR_WAIT_FAILED			((HPR_UINT32)0x7ffffffc)
typedef struct HPR_NETWORKEVENTS_S
{
	HPR_UINT32 lNetworkEvents;
	HPR_INT32 iErrorCode[HPR_FD_MAX_EVENT];
}HPR_NETWORKEVENTS_T;
#else
#endif

/**
 * 模拟完成 FD_CLOSE, FD_READ, FD_WRITE和windows的有区别。
 */

/**
 * HPR_CreateEvent 创建一个事件句柄.
 * @param bInitState (IN) 事件初始状态(HPR_TRUE触发状态,HPR_FALSE非触发状态).
 * @return NULL失败,其他值成功.
 * @sa HPR_CloseEvent().
 */
HPR_DECLARE HPR_HANDLE CALLBACK HPR_CreateEvent(HPR_BOOL bInitState);

/**
 * HPR_CloseEvent 关闭一个事件句柄.
 * @param hEvent (IN) 事件句柄，由HPR_CreateEvent创建.
 * @return HPR_TRUE 成功，HPR_FALSE失败.
 * @sa HPR_CreateEvent().
 */
HPR_DECLARE HPR_BOOL CALLBACK HPR_CloseEvent(HPR_HANDLE hEvent);

/**
 * HPR_WaitForSingleObject 等待单个事件消息到来
 * @param hEvent (IN) 事件句柄
 * @param nTimeOut (IN) 等待超时时间
 * @return HPR_WAIT_OBJECT_0成功，HPR_WAIT_TIMEOUT超时失败，其他值失败.
 * @sa HPR_WaitForMultipleObjects
 */
HPR_DECLARE HPR_UINT32 CALLBACK HPR_WaitForSingleObject(HPR_HANDLE hEvent, HPR_UINT32 nTimeOut);

/**
 * HPR_WaitForMultipleObjects 等待多个事件消息到来
 * @param nEventCount (IN) 事件个数
 * @param hEvents (IN) 事件句柄数组
 * @param bWaitAll (IN) 是否全部等待，不支持，只能等待其中的一个事件到来
 * @param nTimeOut (IN) 等待超时时间
 * @return HPR_WAIT_OBJECT_0成功，HPR_WAIT_TIMEOUT超时失败，其他值失败
 * @sa HPR_WaitForSingleObject
 */
HPR_DECLARE HPR_UINT32 CALLBACK HPR_WaitForMultipleObjects(HPR_UINT32 nEventCount, HPR_HANDLE* hEvents, HPR_BOOL bWaitAll, HPR_UINT32 nTimeOut);

/**
 * HPR_ResetEvent 重置事件消息
 * @param hEvent (IN) 事件句柄
 * @return HPR_TRUE 成功，HPR_FALSE失败.
 * @sa HPR_SetEvent
 */
HPR_DECLARE HPR_BOOL CALLBACK HPR_ResetEvent(HPR_HANDLE hEvent);

/**
 * HPR_SetEvent 触发事件
 * @param hEvent (IN) 事件句柄
 * @return HPR_TRUE 成功，HPR_FALSE失败.
 * @sa HPR_ResetEvent
 */
HPR_DECLARE HPR_BOOL CALLBACK HPR_SetEvent(HPR_HANDLE hEvent);

/**
 * HPR_EventSelect 将网络事件与事件句柄绑定
 * @param nSockFd (IN) 网络套接字
 * @param hEvent (IN) 事件句柄
 * @param nFlag (IN) 网络事件标志（操作位'|'操作）
 * @return 0 成功，-1失败.
 * @sa
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_EventSelect(HPR_SOCK_T nSockFd, HPR_HANDLE hEvent, HPR_UINT32 nFlag);

/**
 * HPR_EnumNetworkEvents 枚举网络事件
 * @param nSockFd (IN) 网络套接字
 * @param hEvent (IN) 事件句柄
 * @param pNetworkEvents (OUT) 网络事件集合
 * @return 0 成功，-1失败.
 * @sa
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_EnumNetworkEvents(HPR_SOCK_T nSockFd, HPR_HANDLE hEvent, HPR_NETWORKEVENTS_T* pNetworkEvents);

#endif
