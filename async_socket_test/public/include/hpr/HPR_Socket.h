/*
 * CopyRight:	HangZhou Hikvision System Technology Co., Ltd. All	Right Reserved.
 * FileName:	HRP_Socket.h
 * Desc:		OS Layer Component.
 * Author:		schina
 * Date:		2009-03-02
 * Contact:		zhaoyiji@hikvision.com.cn
 * History:		Created By Zhaoyiji 2009-03-02
 * */

/*! 
	\file HPR_Socket.h
	\brief HPR Socket Manager.

	Details.
*/

#ifndef __HPR_SOCKET_H__
#define __HPR_SOCKET_H__

#include "HPR_Config.h"
#include "HPR_Types.h"
#include "HPR_Addr.h"
#include "HPR_Time.h"

#if defined(OS_WINDOWS)
	#define HPR_SOCK_ERROR SOCKET_ERROR
	#define HPR_INVALID_SOCKET INVALID_SOCKET
	#define HPR_SHUT_RD		SD_RECEIVE
	#define HPR_SHUT_WR		SD_SEND
	#define HPR_SHUT_RDWR	SD_BOTH
	static HPR_BOOL g_bWSAStartup = HPR_FALSE;
#elif defined(OS_POSIX)
	#define HPR_SOCK_ERROR		-1
	#define HPR_INVALID_SOCKET -1
	#define HPR_SHUT_RD		SHUT_RD
	#define HPR_SHUT_WR		SHUT_WR
	#define HPR_SHUT_RDWR	SHUT_RDWR
#else
	#error OS Not Implement Yet.
#endif

/**
 * HPR_CreateSocket
 * @param iAf (IN) address family AF_INET/AF_INET6
 * @param iType (IN) socket type SOCK_DGRAM/SOCK_STREAM
 * @param iProto (IN) 0
 * @return HPR_INVALID_SOCKET fail, else success.
 * @sa HPR_CloseSocket()
 */
HPR_DECLARE HPR_SOCK_T CALLBACK HPR_CreateSocket(HPR_INT32 iAf, HPR_INT32 iType, HPR_INT32 iProto);

/**
 * HPR_CloseSocket
 * @param iSockFd (IN) socket fd get by HPR_CreateSocket
 * @param bForce (IN) judge if force to close socket, call shutdown
 * @return 0 success, -1 fail.
 * @sa HPR_CreateSocket()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_CloseSocket(HPR_SOCK_T iSockFd, HPR_BOOL bForce = HPR_FALSE);

/**
 * HPR_ShutDown shutdown socket fd.
 * @param iSockFd (IN) socket fd.
 * @param iHow (IN) how to shut. HPR_SHUT_RD/HPR_SHUT_WR/HPR_SHUT_RDWR
 * @see HPR_SHUT_RD,HPR_SHUT_RDWR, HPR_SHUT_WR
 * @return 0 success, -1 fail.
 * @sa HPR_CloseSocket(),HPR_CreateSocket()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_ShutDown(HPR_SOCK_T iSockFd, HPR_INT32 iHow);

/**
 * HPR_Bind bind socket fd to address.
 * @param iSockFd (IN) socket fd.
 * @param pHprAddr (IN) HPR_ADDR_T address.
 * @return 0 success, -1 fail.
 * @sa HPR_CreateSocket()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_Bind(HPR_SOCK_T iSockFd, HPR_ADDR_T* pHprAddr);

/**
 * HPR_Listen listen on port (tcp)
 * @param iSockFd (IN) socket fd
 * @param iBackLog (IN) listen queue long.
 * @return 0 success, -1 fail.
 * @sa HPR_Bind(),HPR_CreateSocket()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_Listen(HPR_SOCK_T iSockFd, HPR_INT32 iBackLog);

/**
 * HPR_Accept accept remote connect (tcp)
 * @param iSockFd (IN) socket fd.
 * @param pHprAddr (OUT) remote address.
 * @param nTimeOut (IN) connect timeout
 * @return HPR_INVALID_SOCKET fail, else success.
 * @see HPR_INVALID_SOCKET
 * @sa HPR_Bind(),HPR_CreateSocket(),HPR_Listen()
 */
HPR_DECLARE HPR_SOCK_T CALLBACK HPR_Accept(HPR_SOCK_T iSockFd, HPR_ADDR_T* pHprAddr = NULL, HPR_UINT32 nTimeOut = HPR_INFINITE);

/**
 * HPR_ConnectWithTimeOut connect to remote listen address. (tcp)
 * @param iSockFd (IN) socket fd.
 * @param pHprAddr (IN) struct HPR_ADDR_T.
 * @param iTimeOut (IN) connect timeout, if iTimeOut==0, it means connect with no timeout.
 * @return 0 success, -1 fail.
 * @sa HPR_Accept
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_ConnectWithTimeOut(HPR_SOCK_T iSockFd, HPR_ADDR_T* pHprAddr, HPR_UINT32 nTimeOut=HPR_INFINITE);

/**
 * HPR_BroadCast set broadcast.
 * @param iSockFd (IN) socket fd.
 * @return 0 success -1 fail.
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_BroadCast(HPR_SOCK_T iSockFd);

/**
 * HPR_JoinMultiCastGroup join multicast group
 * @param iSockFd (IN) socket fd.
 * @param pLocal (IN) local address.
 * @param pMcastGroupAddr (IN) multicast group address.
 * @return 0 success, -1 fail.
 * @sa HPR_LeaveMultiCastGroup
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_JoinMultiCastGroup(HPR_SOCK_T iSockFd, HPR_ADDR_T* pLocal, HPR_ADDR_T* pMcastGroupAddr);

/**
 * HPR_LeaveMultiCastGroup leave multicast group.
 * @param iSockFd (IN) socket fd.
 * @param pLocal (IN) local address.
 * @param pMcastGroupAddr (IN) multicast group address.
 * @return 0 success, -1 fail.
 * @sa HPR_JoinMultiCastGroup
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_LeaveMultiCastGroup(HPR_SOCK_T iSockFd, HPR_ADDR_T* pLocal, HPR_ADDR_T* pMcastGroupAddr);

/**
 * HPR_SetNonBlock set enable/disable nonblock.
 * @param iSockFd (IN) socket fd.
 * @param bYesNo (IN) enable/disable nonblock.
 * @return 0 success, -1 fail.
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_SetNonBlock(HPR_SOCK_T iSockFd, HPR_BOOL bYesNo);

/**
 * HPR_SetTTL set ttl
 * @param iSockFd (IN) socket fd
 * @param iTTL (IN) ttl
 * @return 0 success, -1 fail.
 * @sa HPR_GetTTL
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_SetTTL(HPR_SOCK_T iSockFd, HPR_INT32 iTTL);

/**
 * Function:	HPR_SetMultiCastTTL
 * Desc:
 * Input:
 * Output:
 * Return:		if success return HPR_OK, otherwise return HPR_ERROR.
 * */
HPR_DECLARE HPR_INT32 CALLBACK HPR_SetMultiCastTTL(HPR_SOCK_T iSockFd, HPR_INT32 iTTL);

/**
 * HPR_GetTTL get ttl
 * @param iSockFd (IN) socket fd
 * @param iTTL (OUT) ttl
 * @return 0 success, -1 fail.
 * @sa HPR_SetTTL
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_GetTTL(HPR_SOCK_T iSockFd, HPR_INT32* iTTL);

/**
 * HPR_SetTOS set tos
 * @param iSockFd (IN)  socket fd
 * @param iTOS (IN)
 * @return 0 success, -1 fail.
 * @sa HPR_GetTOS
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_SetTOS(HPR_SOCK_T iSockFd, HPR_INT32 iTOS);

/**
 * HPR_GetTOS get tos
 * @param iSockFd (IN) socket fd
 * @param iTOS (OUT)
 * @return 0 success, -1 fail.
 * @sa HPR_SetTOS
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_GetTOS(HPR_SOCK_T iSockFd, HPR_INT32* iTOS);

/**
 * HPR_SetReuseAddr set enable/disable reuse address.
 * @param iSockFd (IN) socket fd.
 * @param bYesNo (IN) enable/disable reuse address.
 * @return 0 success, -1 fail.
 * @sa HPR_SetTimeOut
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_SetReuseAddr(HPR_SOCK_T iSockFd, HPR_BOOL bYesNo);

/**
 * HPR_SetTimeOut set socket fd I/O timeout.
 * @param iSockFd (IN) socket fd
 * @param iSndTimeO (IN) snd timeout
 * @param iRcvTimeO (IN) rcv timeout
 * @return 0 success, -1 fail.
 * @sa HPR_SetReuseAddr
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_SetTimeOut(HPR_SOCK_T iSockFd, HPR_INT32 iSndTimeO, HPR_INT32 iRcvTimeO);

/**
 * HPR_SetBuffSize set socket snd/rcv buffsize.
 * @param iSockFd socket fd.
 * @param iSndBuffSize (IN) snd buffer size.
 * @param iRcvBuffSize (IN) rcv buffer size.
 * @return 0 succes, -1 fail.
 * @sa HPR_GetBuffSize
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_SetBuffSize(HPR_SOCK_T iSockFd, HPR_INT32 iSndBuffSize, HPR_INT32 iRcvBuffSize);

/**
 * HPR_GetBuffSize get socket snd/rcv buffsize.
 * @param iSockFd socket fd.
 * @param iSndBuffSize (OUT) snd buffer size.
 * @param iRcvBuffSize (OUT) rcv buffer size.
 * @return 0 succes, -1 fail.
 * @sa HPR_SetBuffSize
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_GetBuffSize(HPR_SOCK_T iSockFd, HPR_INT32* iSndBuffSize, HPR_INT32* iRcvBuffSize);

/**
 * HPR_LingerOn disable linger (tcp)
 * @param iSockFd (IN) socket fd.
 * @param iTimeOutSec (IN) linger timeout
 * @return 0 success, -1 fail.
 * @sa HPR_LingerOff()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_LingerOff(HPR_SOCK_T iSockFd);

/**
 * HPR_LingerOn enable linger (tcp)
 * @param iSockFd (IN) socket fd.
 * @param nTimeOut (IN) linger time.
 * @return 0 success, -1 fail.
 * @sa HPR_LingerOn()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_LingerOn(HPR_SOCK_T iSockFd,HPR_UINT16 nTimeOutSec);

/**
 * HPR_SetNoDelay enable/disabel nagle algorithm (tcp)
 * @param iSockFd (IN) socket
 * @param bYesNo (IN) enable/disabel nagle algorithm
 * @return 0 success, -1 fail
 * @sa HPR_LingerOn(),HPR_LingerOff()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_SetNoDelay(HPR_SOCK_T iSockFd,HPR_BOOL bYesNo);

/**
 * HPR_Sendn send dada to socket fd (tcp).
 * @param iSockFd (IN) socket fd.
 * @param pBuf (IN) data pointer to send data.
 * @param iBufLen (IN) data count want to send.
 * @param iTimeOut (IN) send timeout.
 * @return count of data sent, -1 for error. 
 * @sa HPR_Recv
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_Send(HPR_SOCK_T iSockFd, HPR_VOIDPTR pBuf, HPR_INT32 iBufLen);

/**
 * HPR_Sendn send n byte dada to socket fd (tcp).
 * @param iSockFd (IN) socket fd.
 * @param pBuf (IN) data pointer to send data.
 * @param iBufLen (IN) data count want to send.
 * @param iTimeOut (IN) send timeout.
 * @return count of data sent, -1 for error. 
 * @sa HPR_Recv
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_Sendn(HPR_SOCK_T iSockFd, HPR_VOIDPTR pBuf, HPR_INT32 iBufLen, HPR_UINT32 nTimeOut=HPR_INFINITE);

/**
 * HPR_Recvn recv data from socket fd (tcp).
 * @param iSockFd (IN) socket fd.
 * @param pBuf (OUT) data pointer to recv data.
 * @param iBufCount (IN) data length want to recv.
 * @return count of data received, -1 for error.
 * @sa HPR_Send
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_Recv(HPR_SOCK_T iSockFd, HPR_VOIDPTR pBuf, HPR_INT32 iBufCount);
HPR_DECLARE HPR_INT32 CALLBACK HPR_RecvWithTimeOut(HPR_SOCK_T iSockFd, HPR_VOIDPTR pBuf, HPR_INT32 iBufCount, HPR_UINT32 nTimeOut);

/**
 * HPR_Recvn recv n bytes data from socket fd (tcp).
 * @param iSockFd (IN) socket fd.
 * @param pBuf (OUT) data pointer to recv data.
 * @param iBufCount (IN) data length want to recv.
 * @param iTimeOut (IN) recv timeout
 * @return count of data received, -1 for error.
 * @sa HPR_Recv
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_Recvn(HPR_SOCK_T iSockFd, HPR_VOIDPTR pBuf, HPR_INT32 iBufCount, HPR_UINT32 nTimeOut=HPR_INFINITE);


/**
 * HPR_SendTo sendto (udp)
 * @param iSockFd (IN) socket fd
 * @param pBuf (IN) data pointer
 * @param iBufLen (IN) max data buffer size
 * @param pHprAddr (IN) HPR_ADDR_T pointer.
 * @return count of data sent, -1 for errror.
 * @sa HPR_RecvFrom
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_SendTo(HPR_SOCK_T iSockFd, HPR_VOIDPTR pBuf, HPR_INT32 iBufLen, HPR_ADDR_T* pHprAddr);

/**
 * HPR_RecvFrom recvfrom (udp)
 * @param iSockFd (IN) socket fd
 * @param pBuf (OUT) data pointer.
 * @param iBufCount (IN) max data buffer size.
 * @param pHprAddr (OUT) HPR_ADDR_T pointer.
 * @return count of data received, -1 for error.
 * @sa HPR_SendTo
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_RecvFrom(HPR_SOCK_T iSockFd, HPR_VOIDPTR pBuf, HPR_INT32 iBufCount, HPR_ADDR_T* pHprAddr);

HPR_DECLARE HPR_INT32 CALLBACK HPR_Ioctl(HPR_SOCK_T iSockFd, HPR_INT32 iCmd, HPR_UINT32 *nData);

#if 0
///////////////////////////////////
///class Export
class HPR_DECLARE_CLASS HPR_Sock
{
public:
	//HPR_Sock();
	//~HPR_Sock();

public:
	int Open(int type,
				int proto_family=AF_INET,
				int proto=0,
				int reuseaddr=0,
				int flag=0);

	void Close()
	{
		if (m_sockFd != HPR_INVALID_SOCKET)
		{
			HPR_CloseSocket(m_sockFd);
			m_sockFd = HPR_INVALID_SOCKET;
		}

		return;
	};

	void SetFd(HPR_SOCK_T sockfd)
	{
		m_sockFd = sockfd;
	};

	HPR_SOCK_T GetFd()
	{
		return m_sockFd;
	};

	int GetType()
	{
		return m_iType;
	};

public:
	int m_iType;
	HPR_SOCK_T m_sockFd;
};

class HPR_DECLARE_CLASS HPR_Sock_Dgram : public HPR_Sock
{
public:
	HPR_Sock_Dgram(const HPR_Inet_Addr& local,int proto_family=AF_INET,int proto=0,int reuseaddr=0,int flag=0);
	HPR_Sock_Dgram();
	~HPR_Sock_Dgram();

public:
	int Open(const HPR_Inet_Addr& local,int proto_family=AF_INET,int proto=0,int reuseaddr=0,int flag=0);
	
	int Send(const void* buf, int n, HPR_Inet_Addr& addr,int flag=0)const;
	int Recv(void* buf,int n,HPR_Inet_Addr& addr,int flag=0)const;
	int Send(const void* buf, int n, HPR_Inet_Addr& addr,int flag,HPR_Time_Value& tv)const;
	int Recv(void* buf,int n,HPR_Inet_Addr& addr,int flag,HPR_Time_Value& tv)const;
};

class HPR_DECLARE_CLASS HPR_Sock_Stream : public HPR_Sock
{
public:
	HPR_Sock_Stream();
	HPR_Sock_Stream(HPR_SOCK_T sockfd);

	~HPR_Sock_Stream();

public:
	int CloseRead(void)
	{
		return HPR_ShutDown(m_sockFd, HPR_SHUT_RD);
	};

	int CloseWrite(void)
	{
		return HPR_ShutDown(m_sockFd, HPR_SHUT_WR);
	};

	int Close(void)
	{
		return HPR_CloseSocket(m_sockFd);
	};

	int Recvn(char* buf,
				int n,
				int flag,
				HPR_Time_Value* t=0, 
				int* transfered=0) const;

	int Recvn(char* buf,
				int n,
				HPR_Time_Value* t=0, 
				int* transfered=0) const;

	int Sendn(char* buf,
				int n,
				int flag,
				HPR_Time_Value* t=0,
				int* transfered=0);

	int Sendn(char* buf,
				int n,
				HPR_Time_Value* t=0,
				int* transfered=0);

	int Recv();

	int Send();

private:
	
};

class HPR_DECLARE_CLASS HPR_Sock_Mcast : public HPR_Sock_Dgram
{
public:
	HPR_Sock_Mcast();
	~HPR_Sock_Mcast();
public:
private:
};

class HPR_DECLARE_CLASS HPR_Sock_Connector : public HPR_Sock
{
public:
	HPR_Sock_Connector();
	~HPR_Sock_Connector();

public:

private:
};

class HPR_DECLARE_CLASS HPR_Sock_Accept : public HPR_Sock
{
public:
	HPR_Sock_Accept();
	~HPR_Sock_Accept();

public:

private:
};
#endif

#endif
