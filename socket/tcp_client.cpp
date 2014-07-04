
#include "tcp_client.h"
#include "libdevproxy_api.h"
#include "libdevproxy_define.h"
#include "HPR_Time.h"
CTCPClient::CTCPClient(void)
{
	m_socket = HPR_CreateSocket(AF_INET, SOCK_STREAM, 0);
	SetSockType(SOCK_TCP_CLIENT);
	m_phandler = NULL;
}

CTCPClient::~CTCPClient(void)
{
	Stop();
	m_phandler = NULL;
}

/**	@fn	HPR_INT32 CTCPClient::Connect(const char* szIP, HPR_INT32 nPort)
*	@brief 
*	@param[in] szIP 
*	@param[in] nPort 
*	@return	
*/
HPR_INT32 CTCPClient::Connect( const char* szIP, HPR_INT32 nPort )
{
	HPR_INT32 nErrorCode = LIB_DEV_PROXY_ERROR_OK;
	HPR_ADDR_T addr;
	memset(&addr, 0, sizeof(HPR_ADDR_T));
	HPR_MakeAddrByString(AF_INET, szIP, nPort, &addr);
	if (HPR_ConnectWithTimeOut(GetSocket(), &addr, 5000) != 0)
	{
		nErrorCode = LIB_DEV_PROXY_ERROR_CONNECT;
	}
	return nErrorCode;
}

/**	@fn	HPR_INT32 CTCPClient::SendMsg(const char* szBuf, HPR_INT32 nBufSize)
*	@brief 
*	@param[in] szBuf 
*	@param[in] nBufSize 
*	@return	
*/
HPR_INT32 CTCPClient::SendMsg(const char* szBuf, HPR_INT32 nBufSize )
{
	HPR_INT32 nErrorCode = LIB_DEV_PROXY_ERROR_OK;
	HPR_INT32 nRet = HPR_Send(GetSocket(), (HPR_VOIDPTR)szBuf, nBufSize);
	if ( nRet < 0)
	{
#if (defined(_WIN32) || defined(_WIN64))
		if (::GetLastError() != WSAEWOULDBLOCK)
#elif defined(__linux__)
		if (errno != EAGAIN)
#endif
		{
			nErrorCode = LIB_DEV_PROXY_ERROR_SEND;
		}
	}
	else if (nRet == 0)
	{
		nErrorCode = LIB_DEV_PROXY_ERROR_SEND;
		DEV_PROXY_DEBUG(0, "DEV PROXY Debug", 0, 0,
			"dev proxy send tcp data error, peer closed.", nRet, nBufSize);
	}
	else if (nRet != nBufSize)
	{
		DEV_PROXY_DEBUG(0, "DEV PROXY Debug", 0, 0,
			"dev proxy send tcp data error, send size: %d, less than %d.", nRet, nBufSize);
	}
	return nErrorCode;
}

/**	@fn	void CTCPClient::Stop()
*	@brief 
*	@return	
*/
void CTCPClient::Stop()
{
	if (GetSocket() != HPR_INVALID_SOCKET)
	{
		HPR_INT32 nStart = HPR_GetTimeTick();
		HPR_CloseSocket(GetSocket());
		HPR_INT32 nStop = HPR_GetTimeTick();
		DEV_PROXY_DEBUG(0, "DEV PROXY Debug", 0, 0,
			"dev proxy close socket, sock %d, real sock: %d, time: %d.", m_sock_id, m_socket, nStop - nStart);
		if (m_phandler != NULL)
		{
			m_phandler->OnClose(GetSocketID());
		}
		m_socket = HPR_INVALID_SOCKET;

	}
}

/**	@fn	void CTCPClient::RegisterHandler(CBaseHandler* pHandler)
*	@brief 
*	@param[in] pHandler 
*	@return	
*/
void CTCPClient::RegisterHandler( CBaseHandler* pHandler )
{
	m_phandler = pHandler;
}

/**	@fn	void CTCPClient::OnRecv()
*	@brief 
*	@return	
*/
void CTCPClient::OnRecv()
{
	char szBuf[TCP_MAX_BUF_SIZE] = {0};
	HPR_INT32 nRet = HPR_Recv(GetSocket(), szBuf, TCP_MAX_BUF_SIZE);
	if (nRet > 0)
	{
		HPR_INT32 nBufSize = nRet;
		if (m_phandler != NULL)
		{
			HPR_ADDR_T addr;
			memset(&addr, 0, sizeof(HPR_ADDR_T));
			HPR_GetAddrBySockFd(GetSocket(), NULL, &addr);
			if (m_phandler != NULL)
			{
				m_phandler->OnRecv(GetSocketID(), szBuf, nBufSize, HPR_GetAddrString(&addr), HPR_GetAddrPort(&addr));
			}
		}
	}
	else if (nRet == 0)		
	{
		//对方关闭socket
		if (m_phandler != NULL)
		{
			m_phandler->OnException(GetSocketID(), LIB_DEV_PROXY_ERROR_RECV);
		}
		DEV_PROXY_DEBUG(0, "DEV PROXY Debug", 0, 0,
			"dev proxy recv tcp data error, peer closed.");
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
			DEV_PROXY_DEBUG(0, "DEV PROXY Debug", 0, 0,
				"dev proxy recv tcp data error, errno: %d.", nErrorCode);
			if (m_phandler != NULL)
			{
				m_phandler->OnException(GetSocketID(), LIB_DEV_PROXY_ERROR_RECV);
			}
		}
	}
}
