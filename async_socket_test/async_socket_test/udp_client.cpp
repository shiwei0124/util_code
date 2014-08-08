#include "udp_client.h"
#include "io_loop.h"
#include "socket_io_define.h"
CUDPClient::CUDPClient(CIOLoop* pIO) : CBaseIOStream(pIO)
{
	m_socket = S_CreateSocket(AF_INET, SOCK_DGRAM, 0);
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
	char szBuf[UDP_RECV_SIZE] = {0};
    char szIP[32] = {0};
    int32_t nPort = 0;
	int32_t nRet = S_RecvFrom(GetSocket(), szBuf, UDP_RECV_SIZE, szIP, &nPort);
	if (nRet > 0)
	{
		int32_t nBufSize = nRet;
		DoRecv(GetSocketID(), szBuf, nBufSize, szIP, nPort);
	}
	else
	{
#if (defined(_WIN32) || defined(_WIN64))  
		int32_t nErrorCode = ::GetLastError();
#else
		int32_t nErrorCode = errno;
#endif
		{
			SOCKET_IO_ERROR("recv udp data error, errno: %d.", nErrorCode);
			DoException(GetSocketID(), SOCKET_IO_UDP_RECV_FAILED);
		}

	}
}

/**	@fn	int32_t CUDPClient::SendMsg(const char* szIP, int32_t nPort, const char* szMsg, int32_t nMsgLength)
*	@brief 
*	@param[in] szIP 
*	@param[in] nPort 
*	@param[in] szMsg 
*	@param[in] nMsgLength 
*	@return	
*/
int32_t CUDPClient::SendMsg( const char* szIP, int32_t nPort, const char* szMsg, int32_t nMsgLength )
{
	int32_t nRet = 0;
    if (S_SendTo(GetSocket(), (void*)szMsg, nMsgLength, szIP, nPort) == -1)
    {
        nRet = SOCKET_IO_UDP_SEND_FAILED;
    }
	return nRet;
}

/**	@fn	void CUDPClient::Stop()
*	@brief 
*	@return	
*/
void CUDPClient::Stop()
{
	if (GetSocket() != S_INVALID_SOCKET)
	{
		m_pio->Remove_Handler(this);
		S_CloseSocket(GetSocket());
		DoClose(GetSocketID());
		m_socket = S_INVALID_SOCKET;
	}
}




