#include "tcp_session_async.h"
#include "hpr/HPR_Time.h"
#include "io_loop.h"
#include "socket_io_define.h"
CTCPSessionAsync::CTCPSessionAsync(CIOLoop* pIO) : CBaseIOStream(pIO)
{
	m_socket = HPR_INVALID_SOCKET;
	SetSockType(SOCK_TCP_SESSION);
}

/**	@fn	 CTCPSessionAsync::CTCPSessionAsync(CIOLoop* pIO, HPR_SOCK_T nSock)
*	@brief 
*	@param[in] pIO 
*	@param[in] nSock 
*	@return	
*/
CTCPSessionAsync::CTCPSessionAsync( CIOLoop* pIO, HPR_SOCK_T nSock ) : CBaseIOStream(pIO)
{
	SetSockType(SOCK_TCP_SESSION);
	SetSocket(nSock);
}

CTCPSessionAsync::~CTCPSessionAsync(void)
{
	Stop();
}

/**	@fn	void CTCPSessionAsync::SetSocket(HPR_SOCK_T nSock)
*	@brief 
*	@param[in] nSock 
*	@return	
*/
void CTCPSessionAsync::SetSocket( HPR_SOCK_T nSock )
{
	m_socket = nSock;
	HPR_SetNonBlock(m_socket, HPR_TRUE);
	m_pio->Add_Handler(this);
}


/**	@fn	void CTCPSessionAsync::OnRecv()
*	@brief 
*	@return	
*/
void CTCPSessionAsync::OnRecv()
{
	char szBuf[TCP_RECV_SIZE] = {0};
	HPR_INT32 nRet = HPR_Recv(GetSocket(), szBuf, TCP_RECV_SIZE);
	if (nRet > 0)
	{
		HPR_INT32 nBufSize = nRet;
		HPR_ADDR_T addr;
		memset(&addr, 0, sizeof(HPR_ADDR_T));
		HPR_GetAddrBySockFd(GetSocket(), NULL, &addr);
		DoRecv(GetSocketID(), szBuf, nBufSize, HPR_GetAddrString(&addr), HPR_GetAddrPort(&addr));
	}
	else if (nRet == 0)		
	{
		//对方关闭socket
		DoException(GetSocketID(), SOCKET_IO_TCP_RECV_FAILED);
		SOCKET_IO_WARN("recv tcp data error, peer closed.");
	}
	else
	{
#if (defined(_WIN32) || defined(_WIN64))
		HPR_INT32 nErrorCode = ::GetLastError();
		if (nErrorCode != WSAEWOULDBLOCK)
#elif defined(__linux__)
		HPR_INT32 nErrorCode = errno;
		if (nErrorCode != EAGAIN)
#endif
		{
			SOCKET_IO_ERROR("recv tcp data error, errno: %d.", nErrorCode);
			DoException(GetSocketID(), SOCKET_IO_TCP_RECV_FAILED);
		}
		else
		{
			//用select/epoll/iocp的方式应该不会有这个情况出现
			SOCKET_IO_WARN("recv tcp data error, buffer is blocking.");
		}
	}
}

/**	@fn	HPR_INT32 CTCPSessionAsync::SendBufferAsync()
*	@brief 
*	@return	
*/
HPR_INT32 CTCPSessionAsync::SendBufferAsync()
{
	HPR_INT32 nErrorCode = 0;
	m_sendqueuemutex.Lock();
	if (m_sendqueue.size() == 0)
	{
		//待发送队列中为空，则删除写事件的注册,改成读事件
		m_pio->Remove_WriteEvent(this);
		m_sendqueuemutex.Unlock();
		return nErrorCode;
	}
	CBufferLoop* pBufferLoop = m_sendqueue.front();
	m_sendqueuemutex.Unlock();
	char* szSendBuffer = new char[pBufferLoop->get_used_size()];
	HPR_INT32 nRealSize = 0;
	pBufferLoop->get_buffer_tmp(szSendBuffer, pBufferLoop->get_used_size(), &nRealSize);
	HPR_INT32 nRet = HPR_Send(GetSocket(), (HPR_VOIDPTR)szSendBuffer, nRealSize);
	if ( nRet < 0)
	{
#if (defined(_WIN32) || defined(_WIN64))
		HPR_INT32 nError = ::GetLastError();
		if (WSAEWOULDBLOCK == nError)
#elif defined(__linux__)
		HPR_INT32 nError = errno;
		if (EAGAIN == nError)
#endif
		{
			SOCKET_IO_WARN("send tcp data, buffer is blocking.")
		}
		else
		{
			SOCKET_IO_ERROR("send tcp data error, errno: %d.", nError);
			DoException(GetSocketID(), SOCKET_IO_TCP_SEND_FAILED);
		}
	}
	else if (nRet == 0)
	{
		SOCKET_IO_WARN("send tcp data error, peer closed.");
		DoException(GetSocketID(), SOCKET_IO_TCP_SEND_FAILED);
	}
	else if (nRet != nRealSize)
	{
		HPR_INT32 nSize = 0;
		//将未成功的数据重新放置buffer loop中，待下次发送
		pBufferLoop->get_buffer(szSendBuffer, nRet, &nSize);
		if (nRet != nSize)
		{
			//一般不可能出现这种情况
			SOCKET_IO_ERROR("send tcp data, send size: %d, less than %d, get buffer size wrong.", nRet, nRealSize);
		}
		else
		{
			SOCKET_IO_WARN("send tcp data, send size: %d, less than %d.", nRet, nRealSize);
		}
	}
	else
	{
		m_sendqueuemutex.Lock();
		m_sendqueue.pop();
		m_sendqueuemutex.Unlock();
	}
	return nErrorCode;
}

/**	@fn	HPR_INT32 CTCPSessionAsync::SendMsgAsync(const char* szBuf, HPR_INT32 nBufSize)
*	@brief 
*	@param[in] szBuf 
*	@param[in] nBufSize 
*	@return	
*/
HPR_INT32 CTCPSessionAsync::SendMsgAsync( const char* szBuf, HPR_INT32 nBufSize )
{
	HPR_INT32 nErrorCode = 0;
	HPR_INT32 nRet = HPR_Send(GetSocket(), (HPR_VOIDPTR)szBuf, nBufSize);
	if ( nRet < 0)
	{
#if (defined(_WIN32) || defined(_WIN64))
		HPR_INT32 nError = ::GetLastError();
		if (WSAEWOULDBLOCK == nError)
#elif defined(__linux__)
		HPR_INT32 nError = errno;
		if (EAGAIN == nError)
#endif
		{
			CBufferLoop* pBufferLoop = new CBufferLoop();
			pBufferLoop->create_buffer(nBufSize);
			pBufferLoop->append_buffer(szBuf, nBufSize);
			m_sendqueuemutex.Lock();
			m_sendqueue.push(pBufferLoop);
			m_sendqueuemutex.Unlock();
			//有数据放入待发送队列，则注册为写事件
			m_pio->Add_WriteEvent(this);
			SOCKET_IO_WARN("send tcp data, buffer is blocking.")
		}
		else
		{
			SOCKET_IO_ERROR("send tcp data error, errno: %d.", nError);
			DoException(GetSocketID(), SOCKET_IO_TCP_SEND_FAILED);
		}
	}
	else if (nRet == 0)
	{
		SOCKET_IO_WARN("send tcp data error, peer closed.");
		DoException(GetSocketID(), SOCKET_IO_TCP_SEND_FAILED);
	}
	else if (nRet != nBufSize)
	{
		HPR_INT32 nRest = nBufSize - nRet; 
		CBufferLoop* pBufferLoop = new CBufferLoop();
		pBufferLoop->create_buffer(nRest);
		pBufferLoop->append_buffer(szBuf + nRet, nRest);
		m_sendqueuemutex.Lock();
		m_sendqueue.push(pBufferLoop);
		m_sendqueuemutex.Unlock();
		//有数据放入待发送队列，则注册为写事件
		m_pio->Add_WriteEvent(this);
		SOCKET_IO_WARN("send tcp data, send size: %d, less than %d.", nRet, nBufSize);
	}
	return nErrorCode;
}

/**	@fn	void CTCPSessionAsync::Stop()
*	@brief 
*	@return	
*/
void CTCPSessionAsync::Stop()
{
	if (GetSocket() != HPR_INVALID_SOCKET)
	{
		m_pio->Remove_Handler(this);
		HPR_INT32 nStart = HPR_GetTimeTick();
		HPR_CloseSocket(GetSocket());
		HPR_INT32 nStop = HPR_GetTimeTick();
		SOCKET_IO_DEBUG("close socket, sock id %d, real sock: %d, time: %d.", m_sock_id, m_socket, nStop - nStart);
		DoClose(GetSocketID());
		m_socket = HPR_INVALID_SOCKET;
	}
}

/**	@fn	HPR_BOOL CTCPSessionAsync::CheckWrite()
*	@brief 判断SOCKET是否需要设置可写
*	@return	
*/
HPR_BOOL CTCPSessionAsync::CheckWrite()
{
	if (GetSendQueueSize() != 0)
	{
		return HPR_TRUE;
	}
	return HPR_FALSE;
}



