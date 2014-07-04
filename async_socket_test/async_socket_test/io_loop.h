#pragma once
#include "socket_io.h"
#include "HPR_Thread.h"
#include "HPR_Mutex.h"
#include "HPR_Socket.h"
#include "base_io_stream.h"
#include <map>
#include <vector>
using namespace std;

class CWakerPipe
{
public:
	CWakerPipe() 
	{ 
		m_wake_sock_recv = HPR_INVALID_SOCKET;
		m_wake_sock_send = HPR_INVALID_SOCKET;
	}
	~CWakerPipe() 
	{
	}

	void Start() 
	{
		m_wake_sock_recv = HPR_CreateSocket(AF_INET, SOCK_DGRAM, 0);
		m_wake_sock_send = HPR_CreateSocket(AF_INET, SOCK_DGRAM, 0);
		memset(&m_wake_addr_recv, 0, sizeof(HPR_ADDR_T));
		HPR_ADDR_T addr;
		memset(&addr, 0, sizeof(HPR_ADDR_T));
		//涓���戒娇���0.0.0.0锛�������锛�HPR_Sendto�����跺��浼���洪��
		HPR_MakeAddrByString(AF_INET, "127.0.0.1", 0, &addr);
		HPR_Bind(m_wake_sock_recv, &addr);
		HPR_GetAddrBySockFd(m_wake_sock_recv, &m_wake_addr_recv, NULL);
	}

	void Stop() 
	{
		if (m_wake_sock_recv != HPR_INVALID_SOCKET)
		{
			HPR_CloseSocket(m_wake_sock_recv);
			m_wake_sock_recv = HPR_INVALID_SOCKET;
		}
		if (m_wake_sock_send != HPR_INVALID_SOCKET)
		{
			HPR_CloseSocket(m_wake_sock_send);
			m_wake_sock_send = HPR_INVALID_SOCKET;
		}
	}

	HPR_SOCK_T GetWakeSocket() { return m_wake_sock_recv; }

	//��ㄤ����ら��IO涓����select寰����
	void Wake() 
	{
		char* szMsg = "lljzj";
		int nRet = HPR_SendTo(m_wake_sock_send, szMsg, strlen(szMsg), &m_wake_addr_recv);
	}

	void Recv()
	{
		char szMsg[64] = {0};
		HPR_ADDR_T addr;
		memset(&addr, 0, sizeof(HPR_ADDR_T));
		HPR_RecvFrom(m_wake_sock_recv, szMsg, 32, &addr);
	}
private:
	HPR_SOCK_T m_wake_sock_recv;
	HPR_SOCK_T m_wake_sock_send;
	HPR_ADDR_T m_wake_addr_recv;
};

class SOCKET_IO_DECLARE_CLASS CIOLoop
{
public:
	CIOLoop(void);
	virtual ~CIOLoop(void);

	virtual void Start();
	virtual void Stop();
	static HPR_VOIDPTR CALLBACK RunThread(HPR_VOIDPTR ptr);
	virtual void Run();

	virtual void Add_Handler( CBaseIOStream* piostream );
	virtual void Remove_Handler(CBaseIOStream* piostream);
	virtual void Add_WriteEvent(CBaseIOStream* piostream) { m_waker.Wake(); }
	virtual void Remove_WriteEvent(CBaseIOStream* piostream) { m_waker.Wake(); }
protected:
	CBaseIOStream* _GetHandlerBySock(HPR_SOCK_T sock);

protected:
	HPR_BOOL m_bCloseRequest;
	HPR_HANDLE m_threadhandle;

	map<HPR_SOCK_T, CBaseIOStream*> m_MapIOStreamBySocket;
    HPR_Mutex m_MapMutex;

	CWakerPipe m_waker;
};
