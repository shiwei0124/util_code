#include "base_io_stream.h"
#include "socket_io_define.h"
CBaseIOStream::CBaseIOStream(CIOLoop* pio)
{
	m_socket = HPR_INVALID_SOCKET;
	m_sock_id = CSockIDGenerator::GetInstance()->GetSocketID();
	m_bCheckTcpConnected = HPR_FALSE;
	m_pio = pio;
}

CBaseIOStream::~CBaseIOStream(void)
{
	if (m_socket != HPR_INVALID_SOCKET)
	{
		HPR_CloseSocket(m_socket);
	}
	m_pio = NULL;
}

/**	@fn	HPR_BOOL CBaseIOStream::Bind(const char* szIP, int nPort)
*	@brief 
*	@param[in] szIP 
*	@param[in] nPort 
*	@return	
*/
HPR_BOOL CBaseIOStream::Bind( const char* szIP, int nPort )
{
	HPR_ADDR_T addr;
	memset(&addr, 0, sizeof(HPR_ADDR_T));
	HPR_MakeAddrByString(AF_INET, szIP, nPort, &addr);
	if (HPR_Bind(m_socket, &addr) == 0)
	{
		return HPR_TRUE;
	}
	else
	{
		return HPR_FALSE;
	}
}
