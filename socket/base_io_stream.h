#pragma once
#include "HPR_Socket.h"
#include "HPR_Mutex.h"

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

	HPR_UINT32 GetSocketID()
	{
		m_socket_id_mutex.Lock();
		HPR_UINT32 sock_id = ++m_socket_id;
		m_socket_id_mutex.Unlock();
		return sock_id;
	}

private:
	HPR_UINT32 m_socket_id;					//确保唯一值的socket id
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
	CBaseIOStream(void);
	virtual ~CBaseIOStream(void);

	void SetSockType(EN_SOCKTYPE entype) { m_socktype = entype; }
	EN_SOCKTYPE GetSockType() { return m_socktype; }
	void SetSocket(HPR_SOCK_T nSock) {m_socket = nSock; }
	HPR_SOCK_T GetSocket() { return m_socket; }
	HPR_UINT32 GetSocketID() { return m_sock_id; }
	const char* GetLocalIP() const { 
		HPR_ADDR_T localaddr;
		memset(&localaddr, 0, sizeof(HPR_ADDR_T));
		HPR_INT32 nRet = HPR_GetAddrBySockFd(m_socket, &localaddr, NULL);
		return HPR_GetAddrString(&localaddr); 
	}
	HPR_INT32 GetLocalPort() {
		HPR_ADDR_T localaddr;
		memset(&localaddr, 0, sizeof(HPR_ADDR_T));
		HPR_GetAddrBySockFd(m_socket, &localaddr, NULL);
		return HPR_GetAddrPort(&localaddr); 
	}

	const char* GetRemoteIP() const {
		HPR_ADDR_T remoteaddr;
		memset(&remoteaddr, 0, sizeof(HPR_ADDR_T));
		HPR_INT32 nRet = HPR_GetAddrBySockFd(m_socket, NULL, &remoteaddr);
		return HPR_GetAddrString(&remoteaddr); 
	}
	HPR_INT32 GetRemotePort() {
		HPR_ADDR_T remoteaddr;
		memset(&remoteaddr, 0, sizeof(HPR_ADDR_T));
		HPR_GetAddrBySockFd(m_socket, NULL, &remoteaddr);
		return HPR_GetAddrPort(&remoteaddr); 
	}
	virtual void OnAccept() {};
	virtual void OnRecv() {};
	HPR_BOOL Bind(const char* szIP, HPR_INT32 nPort);
protected:
	

protected:
	HPR_SOCK_T m_socket;					//真实用于通信的socket
	HPR_UINT32 m_sock_id;					//仅用于区分唯一性的值
	EN_SOCKTYPE m_socktype;
};
