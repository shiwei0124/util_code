#pragma once
#include "HPR_Socket.h"
#include "HPR_Mutex.h"
#include "HPR_Hpr.h"

class CIOLoop;

#define TCP_RECV_SIZE   1024 * 2
#define UDP_RECV_SIZE   1024 * 2

class CSockIDGenerator
{
public:
	CSockIDGenerator() { m_socket_id = 0; }
	~CSockIDGenerator() {}

	static CSockIDGenerator* GetInstance() 
	{
		static CSockIDGenerator id_generagtor;
		return &id_generagtor;
	}

	unsigned int GetSocketID()
	{
		m_socket_id_mutex.Lock();
		unsigned int sock_id = ++m_socket_id;
		m_socket_id_mutex.Unlock();
		return sock_id;
	}

private:
	unsigned int m_socket_id;					//确保唯一值的socket id
	HPR_Mutex m_socket_id_mutex;		
};

typedef enum en_socktype
{
	SOCK_NONE_TYPE = 0,
	SOCK_TCP_SERVER,
	SOCK_TCP_SESSION, 
	SOCK_TCP_CLIENT,
	SOCK_UDP_SESSION
}EN_SOCKTYPE;

class CBaseIOStream
{
public:
	CBaseIOStream(CIOLoop* pIO);
	virtual ~CBaseIOStream(void);

	void SetSockType(EN_SOCKTYPE entype) { m_socktype = entype; }
	EN_SOCKTYPE GetSockType() { return m_socktype; }
	virtual void SetSocket(HPR_SOCK_T nSock) {m_socket = nSock; }
	HPR_SOCK_T GetSocket() { return m_socket; }
	unsigned int GetSocketID() { return m_sock_id; }
	void SetCheckConnect(HPR_BOOL bCheck) { m_bCheckTcpConnected = bCheck; }
	HPR_BOOL CheckConnect() { return m_bCheckTcpConnected; }

	const char* GetLocalIP() const { 
		HPR_ADDR_T localaddr;
		memset(&localaddr, 0, sizeof(HPR_ADDR_T));
		int nRet = HPR_GetAddrBySockFd(m_socket, &localaddr, NULL);
		return HPR_GetAddrString(&localaddr); 
	}
	int GetLocalPort() {
		HPR_ADDR_T localaddr;
		memset(&localaddr, 0, sizeof(HPR_ADDR_T));
		HPR_GetAddrBySockFd(m_socket, &localaddr, NULL);
		return HPR_GetAddrPort(&localaddr); 
	}

	virtual const char* GetRemoteIP() const {
		HPR_ADDR_T remoteaddr;
		memset(&remoteaddr, 0, sizeof(HPR_ADDR_T));
		int nRet = HPR_GetAddrBySockFd(m_socket, NULL, &remoteaddr);
		return HPR_GetAddrString(&remoteaddr); 
	}
	virtual int GetRemotePort() {
		HPR_ADDR_T remoteaddr;
		memset(&remoteaddr, 0, sizeof(HPR_ADDR_T));
		HPR_GetAddrBySockFd(m_socket, NULL, &remoteaddr);
		return HPR_GetAddrPort(&remoteaddr); 
	}
	virtual void OnAccept() {};
	virtual void OnConnect(HPR_BOOL bConnected) {};
	virtual void OnRecv() {};
	virtual HPR_BOOL CheckWrite() { return HPR_FALSE; }
	//用于非阻塞的tcp socket缓存数据的发送
	virtual HPR_INT32 SendBufferAsync() { return 0; };  
	HPR_BOOL Bind(const char* szIP, int nPort);
protected:
	

protected:
	HPR_SOCK_T m_socket;					//真实用于通信的socket
	unsigned int m_sock_id;					//仅用于区分唯一性的值
	EN_SOCKTYPE m_socktype;
	
	HPR_BOOL m_bCheckTcpConnected;			//用于记录是否需要判断TCP CLIENT的connect状态;
	CIOLoop* m_pio;
};
