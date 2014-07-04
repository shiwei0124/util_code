/**	@file tcp_session_async.h
 *	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief TCP Server accept到后的socket对象，非阻塞模式
 *
 *	@author		shiwei
 *	@date		2014/05/05
 *
 *	@note 这里填写本文件的详细功能描述和注解
 *	@note 历史记录：
 *	@note V1.0.0  创建文件
 */
#pragma once
#include "socket_io.h"
#include "base_io_stream.h"
#include "buffer_loop.hpp"
#include <queue>
#include "sigslot.h"
using namespace std;
using namespace sigslot;



class SOCKET_IO_DECLARE_CLASS CTCPSessionAsync :
	public CBaseIOStream
{
public:
	CTCPSessionAsync(CIOLoop* pIO);
	CTCPSessionAsync(CIOLoop* pIO, HPR_SOCK_T nSock);
	~CTCPSessionAsync(void);

	virtual void SetSocket(HPR_SOCK_T nSock);
	HPR_INT32 GetSendQueueSize() { 
		m_sendqueuemutex.Lock();
		HPR_INT32 nSize = m_sendqueue.size();
		m_sendqueuemutex.Unlock();
		return nSize; 
	}

	void OnRecv();

	HPR_INT32 SendMsgAsync(const char* szBuf, HPR_INT32 nBufSize);
	void Stop();

	virtual HPR_INT32 SendBufferAsync();
	virtual HPR_BOOL CheckWrite();

	/*HPR_UINT32 nsockid, const char* szBuf, HPR_INT32 nBufSize, const char* szIP, HPR_INT32 nPort*/
	sigslot::signal5<HPR_UINT32, const char*, HPR_INT32, const char*, HPR_INT32 > DoRecv;
	/*HPR_UINT32 nsockid, HPR_INT32 nErrorCode*/
	sigslot::signal2<HPR_UINT32, HPR_INT32> DoException;
	/*HPR_UINT32 nsockid*/
	sigslot::signal1<HPR_UINT32> DoClose;
private:
	CBufferLoop m_recvbuffer;
	queue<CBufferLoop*> m_sendqueue;		//待发送队列，只有非阻塞的TCP socket才会用到
	HPR_Mutex m_sendqueuemutex;

};
