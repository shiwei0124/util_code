#include "udp_client.h"
#include "libdevproxy_api.h"
#include "libdevproxy_define.h"
CUDPClient::CUDPClient(void)
{
	m_socket = HPR_CreateSocket(AF_INET, SOCK_DGRAM, 0);
	m_phandler = NULL;
}


CUDPClient::~CUDPClient(void)
{
	Stop();
	m_phandler = NULL;
}

/**	@fn	void CUDPClient::OnRecv()
*	@brief 
*	@return	
*/
void CUDPClient::OnRecv()
{
	HPR_ADDR_T addr;
	memset(&addr, 0, sizeof(HPR_ADDR_T));
	char szBuf[RECV_MAX_BUF_SIZE] = {0};
	HPR_INT32 nRet = HPR_RecvFrom(GetSocket(), szBuf, RECV_MAX_BUF_SIZE, &addr);
	if (nRet > 0)
	{
		HPR_INT32 nBufSize = nRet;
		if (m_phandler != NULL)
		{
			m_phandler->OnRecv(GetSocketID(), szBuf, nBufSize, HPR_GetAddrString(&addr), HPR_GetAddrPort(&addr));
		}
	}
	else
	{
		HPR_INT32 nErrorCode = ::GetLastError();
		if (nErrorCode != WSAEWOULDBLOCK)
		{
			DEV_PROXY_DEBUG(0, "DEV PROXY Debug", 0, 0,
				"dev proxy recv udp data error, errno: %d.", nErrorCode);
			if (m_phandler != NULL)
			{
				m_phandler->OnException(GetSocketID(), LIB_DEV_PROXY_ERROR_RECV);
			}
		}
	}
}

/**	@fn	void CUDPClient::RegisterHandler(CBaseHandler* pHandler)
*	@brief 
*	@param[in] pHandler 
*	@return	
*/
void CUDPClient::RegisterHandler( CBaseHandler* pHandler )
{
	m_phandler = pHandler;
}

/**	@fn	HPR_INT32 CUDPClient::SendMsg(const char* szIP, HPR_INT32 nPort, const char* szMsg, HPR_INT32 nMsgLength)
*	@brief 
*	@param[in] szIP 
*	@param[in] nPort 
*	@param[in] szMsg 
*	@param[in] nMsgLength 
*	@return	
*/
HPR_INT32 CUDPClient::SendMsg( const char* szIP, HPR_INT32 nPort, const char* szMsg, HPR_INT32 nMsgLength )
{
	HPR_INT32 nRet = LIB_DEV_PROXY_ERROR_OK;
	HPR_ADDR_T addr;
	memset(&addr, 0, sizeof(HPR_ADDR_T));
	HPR_MakeAddrByString(AF_INET, szIP, nPort, &addr);
	for (HPR_INT32 i = 0; i < 1; i++ )
	{
		if (HPR_SendTo(GetSocket(), (HPR_VOIDPTR)szMsg, nMsgLength, &addr) == -1)
		{
			if (::GetLastError() != WSAEWOULDBLOCK)
			{
				nRet = LIB_DEV_PROXY_ERROR_SEND;
				break;
			}
		}
	}
	return nRet;
}

/**	@fn	void CUDPClient::Stop()
*	@brief 
*	@return	
*/
void CUDPClient::Stop()
{
	if (GetSocket() != HPR_INVALID_SOCKET)
	{
		HPR_CloseSocket(GetSocket());
		if (m_phandler != NULL)
		{
			m_phandler->OnClose(GetSocketID());
		}
		m_socket = HPR_INVALID_SOCKET;
	}
}




