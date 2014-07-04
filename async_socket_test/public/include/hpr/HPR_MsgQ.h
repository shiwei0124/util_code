/*
 * CopyRight:	HangZhou Hikvision System Technology Co., Ltd. All	Right Reserved.
 * FileName:	HRP_MsgQ.h
 * Desc:		OS Layer Component.
 * Author:		schina
 * Date:		2009-03-02
 * Contact:		zhaoyiji@hikvision.com.cn
 * History:		Created By Zhaoyiji 2009-03-02
 * */

/*! 
	\file HPR_MsgQ.h
	\brief HPR msgq Manager.

	Details.
*/

#ifndef __HPR_MSGQ_H__
#define __HPR_MSGQ_H__

#include "HPR_Config.h"
#include "HPR_Types.h"


#define HPR_MAX_MSGQ_COUNT	100
#define HPR_MAX_MSG_COUNT	100
#define HPR_MAX_MSGQ_SIZE	8096

/**
	带优先级超时机制的消息队列
*/

/**
 * struct HPR_MSG_T
 */
typedef struct HPR_MSG_BLOCK_T
{
	int iMsgType;		/**< 消息类型			*/
	int iPriority;		/**< 消息优先级			*/
	int iMsgLen;		/**< 消息长度(pMsg长度)	*/
	char pMsg[1];		/**< 消息内容			*/
}HPR_MSG_BLOCK_T;

/**
 * HPR_MsgQCreateEx 创建一个消息队列
 * @param pMsgQId (OUT) 消息队列句柄
 * @return 0成功，-1失败.
 * @sa HPR_MsgQDestroyEx
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_MsgQCreateEx(HPR_HANDLE* pMsgQId);

/**
 * HPR_MsgQDestroyEx 根据消息队列句柄销毁消息队列
 * @param hMsgQId (IN) 消息队列句柄
 * @return 0成功，-1失败。
 * @sa HPR_MsgQCreateEx
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_MsgQDestroyEx(HPR_HANDLE hMsgQId);

/**
 * HPR_MsgQSendEx 发送消息到消息队列
 * @param hMsgQId (IN) 消息队列句柄
 * @param pMsg (IN) 消息块,消息块最大长度8096字节
 * @param nTimeOut (IN) 超时时间，单位毫秒(ms), 小于等于0表示死等(INFINITE)
 * @return 0成功，-1失败
 * @sa HPR_MsgQReceiveEx
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_MsgQSendEx(HPR_HANDLE hMsgQId, HPR_MSG_BLOCK_T* pMsg, HPR_UINT32 nTimeOut=HPR_INFINITE);

/**
 * HPR_MsgQReceiveEx 从消息队列接收消息
 * @param hMsgQId (IN) 消息队列句柄
 * @param pMsg (OUT) 消息块,消息块最大长度8096字节
 * @param iTimeOut(IN) 超时时间，单位毫秒(ms), 小于等于0表示死等(INFINITE)
 * @return 0成功，-1失败
 * @sa HPR_MsgQSendEx
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_MsgQReceiveEx(HPR_HANDLE hMsgQId, HPR_MSG_BLOCK_T* pMsg, HPR_UINT32 nTimeOut=HPR_INFINITE);

#endif
