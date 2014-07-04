#include "tcp_server.h"

CTCPServer::CTCPServer(void)
{
	SetSockType(SOCK_TCP_SERVER);
	m_socket = HPR_CreateSocket(AF_INET, SOCK_STREAM, 0);
	HPR_SetReuseAddr(m_socket, HPR_TRUE);
	m_phandler = NULL;
}

CTCPServer::~CTCPServer(void)
{
	Stop();
	m_phandler = NULL;
}

/**	@fn	void CTCPServer::OnAccept()
*	@brief 
*	@return	
*/
void CTCPServer::OnAccept()
{
	HPR_ADDR_T addr;
	memset(&addr, 0, sizeof(HPR_ADDR_T));
	HPR_SOCK_T sock = HPR_Accept(GetSocket(), &addr);
	if (sock != HPR_INVALID_SOCKET)
	{
		if (m_phandler != NULL)
		{
			m_phandler->OnAccept(GetSocketID(), sock, HPR_GetAddrString(&addr), HPR_GetAddrPort(&addr));
		}
	}
}

/**	@fn	void CTCPServer::Listen()
*	@brief 
*	@return	
*/
void CTCPServer::Listen()
{
	HPR_Listen(GetSocket(), 1000);
}


/**	@fn	void CTCPServer::RegisterHandler(CBaseHandler* pHandler)
*	@brief 
*	@param[in] pHandler 
*	@return	
*/
void CTCPServer::RegisterHandler( CBaseHandler* pHandler )
{
	m_phandler = pHandler;
}

/**	@fn	void CTCPServer::Stop()
*	@brief 
*	@return	
*/
void CTCPServer::Stop()
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

