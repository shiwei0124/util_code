#ifndef _BASE_IO_STREAM_H
#define _BASE_IO_STREAM_H
#include "base_type.h"
#include "base_socket.hpp"
#include "base_thread.hpp"

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

    int32_t GetSocketID()
	{
		m_socket_id_mutex.Lock();
		uint32_t sock_id = ++m_socket_id;
		m_socket_id_mutex.Unlock();
		return sock_id;
	}

private:
	uint32_t m_socket_id;					//确保唯一值的socket id
	CBaseMutex m_socket_id_mutex;
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
	virtual void SetSocket(S_SOCKET nSock) {m_socket = nSock; }
	S_SOCKET GetSocket() { return m_socket; }
	uint32_t GetSocketID() { return m_sock_id; }
	void SetCheckConnect(BOOL bCheck) { m_bCheckTcpConnected = bCheck; }
	BOOL CheckConnect() { return m_bCheckTcpConnected; }

    const char* GetLocalIP() {
        int32_t nPort = 0;
        S_GetSockName(m_socket, m_szIP, &nPort);
        return m_szIP;

	}
	uint32_t GetLocalPort() {
        int32_t nPort = 0;
        S_GetSockName(m_socket, m_szIP, &nPort);
		return nPort;
	}

    virtual const char* GetRemoteIP() {
        int32_t nPort = 0;
        S_GetPeerName(m_socket, m_szIP, &nPort);
        return m_szIP;
	}
	virtual uint32_t GetRemotePort(){
        int32_t nPort = 0;
        S_GetPeerName(m_socket, m_szIP, &nPort);
		return nPort;
	}
	virtual void OnAccept() {};
	virtual void OnConnect(bool bConnected) {};
	virtual void OnRecv() {};
	virtual bool CheckWrite() { return false; }
	//用于非阻塞的tcp socket缓存数据的发送
	virtual int32_t SendBufferAsync() { return 0; };
	BOOL Bind(const char* szIP, uint32_t nPort) const;
protected:
	

protected:
    char m_szIP[32];
	S_SOCKET m_socket;					//真实用于通信的socket
	uint32_t m_sock_id;					//仅用于区分唯一性的值
	EN_SOCKTYPE m_socktype;
	
	BOOL m_bCheckTcpConnected;			//用于记录是否需要判断TCP CLIENT的connect状态;
	CIOLoop* m_pio;
};
#endif
