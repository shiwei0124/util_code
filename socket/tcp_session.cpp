#include "tcp_session.h"
#include "hpr/HPR_Time.h"
#include "libdevanalysis_define.h"
#include "libdevanalysis_api.h"
#include "das_handler.h"
CTCPSession::CTCPSession(void)
{
	m_socket = HPR_INVALID_SOCKET;
	SetSockType(SOCK_TCP_SESSION);
	m_phandler = NULL;
}

CTCPSession::~CTCPSession(void)
{
	Stop();
	m_phandler = NULL;
}

/**	@fn	void CTCPSession::OnRecv()
*	@brief 
*	@return	
*/
void CTCPSession::OnRecv()
{
	char szBuf[RECV_MAX_BUF_SIZE] = {0};
	HPR_INT32 nRet = HPR_Recv(GetSocket(), szBuf, RECV_MAX_BUF_SIZE);
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
		// close socket
		if (m_phandler != NULL)
		{
			m_phandler->OnException(GetSocketID(), LIB_DEV_ANALYSIS_ERROR_RECV);
		}
		DEV_ANALYSIS_DEBUG(0, "DEV ANALYSIS Debug", 0, 0,
			"dev analysis recv tcp data error, peer closed.");
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

			DEV_ANALYSIS_DEBUG(0, "DEV ANALYSIS Debug", 0, 0,
				"dev analysis recv tcp data error, errno: %d.", nErrorCode);
			if (m_phandler != NULL)
			{
				m_phandler->OnException(GetSocketID(), LIB_DEV_ANALYSIS_ERROR_RECV);
			}
		}
	}
}

/**	@fn	void CTCPSession::RegisterHandler(CBaseHandler* pHandler)
*	@brief 
*	@param[in] pHandler 
*	@return	
*/
void CTCPSession::RegisterHandler( CBaseHandler* pHandler )
{
	m_phandler = pHandler;
}

/**	@fn	HPR_INT32 CTCPSession::SendMsg(const char* szBuf, HPR_INT32 nBufSize)
*	@brief 
*	@param[in] szBuf 
*	@param[in] nBufSize 
*	@return	
*/
HPR_INT32 CTCPSession::SendMsg( const char* szBuf, HPR_INT32 nBufSize )
{
	HPR_INT32 nErrorCode = LIB_DEV_ANALYSIS_ERROR_OK;
	HPR_INT32 nRet = HPR_Send(GetSocket(), (HPR_VOIDPTR)szBuf, nBufSize);
	if ( nRet < 0)
	{
#if (defined(_WIN32) || defined(_WIN64))
		if (::GetLastError() != WSAEWOULDBLOCK)
#elif defined(__linux__)
		if (errno != EAGAIN)
#endif
		{
			nErrorCode = LIB_DEV_ANALYSIS_ERROR_SEND;
		}
	}
	else if (nRet == 0)
	{
		nErrorCode = LIB_DEV_ANALYSIS_ERROR_SEND;
		DEV_ANALYSIS_DEBUG(0, "DEV ANALYSIS Debug", 0, 0,
			"dev analysis send tcp data error, peer closed.", nRet, nBufSize);
	}
	else if (nRet != nBufSize)
	{
		DEV_ANALYSIS_DEBUG(0, "DEV ANALYSIS Debug", 0, 0,
			"dev analysis send tcp data error, send size: %d, less than %d.", nRet, nBufSize);
	}
	return nErrorCode;
}

/**	@fn	void CTCPSession::Stop()
*	@brief 
*	@return	
*/
void CTCPSession::Stop()
{
	if (GetSocket() != HPR_INVALID_SOCKET)
	{
		HPR_INT32 nStart = HPR_GetTimeTick();
		HPR_CloseSocket(GetSocket());
		HPR_INT32 nStop = HPR_GetTimeTick();
		DEV_ANALYSIS_DEBUG(0, "DEV ANALYSIS Debug", 0, 0,
			"dev analysis close socket, sock id %d, real sock: %d, time: %d.", m_sock_id, m_socket, nStop - nStart);
		if (m_phandler != NULL)
		{
			m_phandler->OnClose(GetSocketID());
		}
		m_socket = HPR_INVALID_SOCKET;
	}
}
