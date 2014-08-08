#include "tcp_server.h"
#include "io_loop.h"
#include "socket_io_define.h"
CTCPServer::CTCPServer(CIOLoop* pIO) : CBaseIOStream(pIO)
{
	SetSockType(SOCK_TCP_SERVER);
	m_socket = S_CreateSocket(AF_INET, SOCK_STREAM, 0);
    
	S_SetReuseAddr(m_socket, TRUE);
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
    char szIP[32] = {0};
    int32_t nPort = 0;
	S_SOCKET sock = S_Accept(GetSocket(), szIP, &nPort);
	if (sock != S_INVALID_SOCKET)
	{
		DoAccept(GetSocketID(), sock, szIP, nPort);
	}
}

/**	@fn	void CTCPServer::Listen()
*	@brief 
*	@return	
*/
void CTCPServer::Listen()
{
	S_Listen(GetSocket(), 1000);
	m_pio->Add_Handler(this);
}

/**	@fn	void CTCPServer::Stop()
*	@brief 
*	@return	
*/
void CTCPServer::Stop()
{
	if (GetSocket() != S_INVALID_SOCKET)
	{
		m_pio->Remove_Handler(this);
		S_CloseSocket(GetSocket());
		DoClose(GetSocketID());
		m_socket = S_INVALID_SOCKET;
	}
}

