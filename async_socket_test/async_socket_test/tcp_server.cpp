#include "tcp_server.h"
#include "io_loop.h"
#include "socket_io_define.h"
CTCPServer::CTCPServer(CIOLoop* pIO) : CBaseIOStream(pIO)
{
	SetSockType(SOCK_TCP_SERVER);
	m_socket = HPR_CreateSocket(AF_INET, SOCK_STREAM, 0);
	HPR_SetReuseAddr(m_socket, HPR_TRUE);
}

CTCPServer::~CTCPServer(void)
{
	Stop();
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
		DoAccept(GetSocketID(), sock, HPR_GetAddrString(&addr), HPR_GetAddrPort(&addr));
	}
}

/**	@fn	void CTCPServer::Listen()
*	@brief 
*	@return	
*/
void CTCPServer::Listen()
{
	HPR_Listen(GetSocket(), 1000);
	m_pio->Add_Handler(this);
}

/**	@fn	void CTCPServer::Stop()
*	@brief 
*	@return	
*/
void CTCPServer::Stop()
{
	if (GetSocket() != HPR_INVALID_SOCKET)
	{
		m_pio->Remove_Handler(this);
		HPR_CloseSocket(GetSocket());
		DoClose(GetSocketID());
		m_socket = HPR_INVALID_SOCKET;
	}
}

