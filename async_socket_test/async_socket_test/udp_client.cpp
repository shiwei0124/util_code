#include "udp_client.h"
#include "io_loop.h"
#include "socket_io_define.h"
CUDPClient::CUDPClient(CIOLoop* pIO) : CBaseIOStream(pIO)
{
	m_socket = HPR_CreateSocket(AF_INET, SOCK_DGRAM, 0);
	m_pio->Add_Handler(this);
}


CUDPClient::~CUDPClient(void)
{
	Stop();
}

/**	@fn	void CUDPClient::OnRecv()
*	@brief 
*	@return	
*/
void CUDPClient::OnRecv()
{
	HPR_ADDR_T addr;
	memset(&addr, 0, sizeof(HPR_ADDR_T));
	char szBuf[UDP_RECV_SIZE] = {0};
	HPR_INT32 nRet = HPR_RecvFrom(GetSocket(), szBuf, UDP_RECV_SIZE, &addr);
	if (nRet > 0)
	{
		HPR_INT32 nBufSize = nRet;
		DoRecv(GetSocketID(), szBuf, nBufSize, HPR_GetAddrString(&addr), HPR_GetAddrPort(&addr));
	}
	else
	{
#if (defined(_WIN32) || defined(_WIN64))  
		HPR_INT32 nErrorCode = ::GetLastError();
#elif defined(__linux__)
		HPR_INT32 nErrorCode = errno;
#endif
		{
			SOCKET_IO_ERROR("recv udp data error, errno: %d.", nErrorCode);
			DoException(GetSocketID(), SOCKET_IO_UDP_RECV_FAILED);
		}

	}
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
	HPR_INT32 nRet = 0;
	HPR_ADDR_T addr;
	memset(&addr, 0, sizeof(HPR_ADDR_T));
	HPR_MakeAddrByString(AF_INET, szIP, nPort, &addr);
	for (HPR_INT32 i = 0; i < 1; i++ )
	{
		if (HPR_SendTo(GetSocket(), (HPR_VOIDPTR)szMsg, nMsgLength, &addr) == -1)
		{
			//if (::GetLastError() != WSAEWOULDBLOCK)
			//{
				nRet = SOCKET_IO_UDP_SEND_FAILED;
				break;
			//}
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
		m_pio->Remove_Handler(this);
		HPR_CloseSocket(GetSocket());
		DoClose(GetSocketID());
		m_socket = HPR_INVALID_SOCKET;
	}
}




