#include "tcp_client_async.h"
#include "HPR_Time.h"
#include "socket_io_define.h"
#include "io_loop.h"
#if (defined(_WIN32) || defined(_WIN64))
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#elif defined(__linux__)
#include <sys/types.h>
#include <sys/socket.h>
#endif

CTCPClientAsync::CTCPClientAsync(CIOLoop* pIO) : CBaseIOStream(pIO)
{
	m_socket = HPR_CreateSocket(AF_INET, SOCK_STREAM, 0);
	SetSockType(SOCK_TCP_CLIENT);
	HPR_SetNonBlock(m_socket, HPR_TRUE);
	SetCheckConnect(HPR_TRUE);
}

CTCPClientAsync::~CTCPClientAsync(void)
{
	Stop();
}


/**	@fn	HPR_INT32 CTCPClientAsync::ConnectAsync(const char* szIP, HPR_INT32 nPort)
*	@brief 
*	@param[in] szIP 
*	@param[in] nPort 
*	@return	返回值为0
*/
int CTCPClientAsync::ConnectAsync( const char* szIP, int nPort )
{
	SetRemoteIP(szIP);
	SetRemotePort(nPort);
	HPR_ADDR_T addr;
	memset(&addr, 0, sizeof(HPR_ADDR_T));
	HPR_MakeAddrByString(AF_INET, szIP, nPort, &addr);

#if (defined(_WIN32) || defined(_WIN64))  
	//貌似这种做法对select没有用
	//HPR_INT32 timeout = 5000;
	//HPR_INT32 len = sizeof(timeout); 
	//setsockopt(GetSocket(), SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, len); 
#elif defined(__linux__)
	//貌似这个做法也没有效果
	struct timeval timeout = {10, 0};   
	socklen_t len = sizeof(timeout); 
	setsockopt(GetSocket(), SOL_SOCKET, SO_SNDTIMEO, &timeout, len); 
#endif

	if (HPR_ConnectWithTimeOut(GetSocket(), &addr) != 0)
	{
#if (defined(_WIN32) || defined(_WIN64))
		HPR_INT32 nError = ::GetLastError();
		if (nError != WSAEWOULDBLOCK)
#elif defined(__linux__)
		int nError = errno;
		if (nError != EINPROGRESS)
#endif
		{
			//连接出错
			SOCKET_IO_ERROR("connect error, errno: %d.", nError);
			DoException(GetSocketID(), SOCKET_IO_TCP_CONNECT_FAILED);
		}
		else
		{
			//连接建立已启动，但是还未成功
		}
	}
	else
	{
		//连接成功，一般连接的是本地端口可能会立即成功
		OnConnect(HPR_TRUE);
	}

	//对于tcp client来说，在此处在加入io中
	m_pio->Add_Handler(this);
	//对于windows/linux 的select来说，connect需要激活可写事件
	//对于epoll来说，激活的是EPOLLERR?
	m_pio->Add_WriteEvent(this);
	return 0;
}

/**	@fn	HPR_INT32 CTCPClientAsync::SendBufferAsync()
*	@brief 将待发送队列中的数据发送出去
*	@return	
*/
int CTCPClientAsync::SendBufferAsync()
{
	int nErrorCode = 0;
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
		int nError = ::GetLastError();
		if (WSAEWOULDBLOCK == nError)
#elif defined(__linux__)
		int nError = errno;
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
		//将未成功的数据重新放置buffer loop中，待下次发送
		int nSize = 0;
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

/**	@fn	HPR_INT32 CTCPClientAsync::SendMsgAsync(const char* szBuf, HPR_INT32 nBufSize)
*	@brief 
*	@param[in] szBuf 
*	@param[in] nBufSize 
*	@return	
*/
int CTCPClientAsync::SendMsgAsync(const char* szBuf, int nBufSize )
{
	int nErrorCode = 0;
	int nRet = HPR_Send(GetSocket(), (HPR_VOIDPTR)szBuf, nBufSize);
	if ( nRet < 0)
	{
#if (defined(_WIN32) || defined(_WIN64))
		int nError = ::GetLastError();
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
			SOCKET_IO_WARN("send tcp data, buffer is blocking.");
		}
		else
		{
			SOCKET_IO_ERROR("send tcp data error, errno: %d.", nError);
			DoException(GetSocketID(), SOCKET_IO_TCP_SEND_FAILED);
		}
	}
	else if (nRet == 0)
	{
		nErrorCode = 0;
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

/**	@fn	void CTCPClientAsync::Stop()
*	@brief 
*	@return	
*/
void CTCPClientAsync::Stop()
{
	if (GetSocket() != HPR_INVALID_SOCKET)
	{
		m_pio->Remove_Handler(this);
		HPR_INT32 nStart = HPR_GetTimeTick();
		HPR_CloseSocket(GetSocket());
		HPR_INT32 nStop = HPR_GetTimeTick();
		SOCKET_IO_INFO("close socket, sock %d, real sock: %d, time: %d.", m_sock_id, m_socket, nStop - nStart);
		DoClose(GetSocketID());
		m_socket = HPR_INVALID_SOCKET;
	}
}

/**	@fn	void CTCPClientAsync::OnConnect(HPR_BOOL bConnected)
*	@brief 
*	@return	
*/
void CTCPClientAsync::OnConnect(HPR_BOOL bConnected)
{
	//无论是否连接成功，都认为已经判断结束
	SetCheckConnect(HPR_FALSE);
	//连接完毕，则删除写/错误事件的注册,改成读事件
	m_pio->Remove_WriteEvent(this);
	if (HPR_TRUE == bConnected)
	{
		SOCKET_IO_INFO("socket connect successed, remote ip: %s, port: %d.", GetRemoteIP(), GetRemotePort());
		DoConnect(GetSocketID());
	}
	else
	{
		SOCKET_IO_INFO("socket connect failed, remote ip: %s, port: %d.", GetRemoteIP(), GetRemotePort());
		DoException(GetSocketID(), SOCKET_IO_TCP_CONNECT_FAILED);
	}
}

/**	@fn	void CTCPClientAsync::OnRecv()
*	@brief 
*	@return	
*/
void CTCPClientAsync::OnRecv()
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

/**	@fn	HPR_BOOL CTCPClientAsync::CheckWrite()
*	@brief 判断SOCKET是否需要设置可写
*	@return	
*/
HPR_BOOL CTCPClientAsync::CheckWrite()
{
	if (GetSendQueueSize() != 0)
	{
		return HPR_TRUE;
	}
	return CheckConnect();
}





