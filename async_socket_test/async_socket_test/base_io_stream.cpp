#include "base_io_stream.h"
#include "socket_io_define.h"
CBaseIOStream::CBaseIOStream(CIOLoop* pio)
{
	m_socket = S_INVALID_SOCKET;
	m_sock_id = CSockIDGenerator::GetInstance()->GetSocketID();
	m_bCheckTcpConnected = FALSE;
	m_pio = pio;
}

CBaseIOStream::~CBaseIOStream(void)
{
	if (m_socket != S_INVALID_SOCKET)
	{
		S_CloseSocket(m_socket);
	}
	m_pio = NULL;
}

/**	@fn	HPR_BOOL CBaseIOStream::Bind(const char* szIP, uint32_t nPort)
*	@brief 
*	@param[in] szIP 
*	@param[in] nPort 
*	@return	
*/
BOOL CBaseIOStream::Bind( const char* szIP, uint32_t nPort ) const
{
	if (S_Bind(m_socket, szIP, nPort) == 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
