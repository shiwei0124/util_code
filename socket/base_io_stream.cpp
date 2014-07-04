#include "base_io_stream.h"

CBaseIOStream::CBaseIOStream(void)
{
	m_socket = HPR_INVALID_SOCKET;
	m_sock_id = CSockIDGenerator::GetInstance()->GetSocketID();
}

CBaseIOStream::~CBaseIOStream(void)
{
	if (m_socket != HPR_INVALID_SOCKET)
	{
		HPR_CloseSocket(m_socket);
	}
}

/**	@fn	HPR_BOOL CBaseIOStream::Bind(const char* szIP, HPR_INT32 nPort)
*	@brief 
*	@param[in] szIP 
*	@param[in] nPort 
*	@return	
*/
HPR_BOOL CBaseIOStream::Bind( const char* szIP, HPR_INT32 nPort )
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
