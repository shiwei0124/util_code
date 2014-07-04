#pragma once
#include "socket_io.h"
#include "base_io_stream.h"
#include "buffer_loop.hpp"
#include <queue>
#include "sigslot.h"
using namespace std;
using namespace sigslot;

class SOCKET_IO_DECLARE_CLASS CTCPClientAsync : public CBaseIOStream
{
public:
	CTCPClientAsync(CIOLoop* pIO);
	virtual ~CTCPClientAsync(void);

	HPR_INT32 GetSendQueueSize() { 
		m_sendqueuemutex.Lock();
		HPR_INT32 nSize = m_sendqueue.size();
		m_sendqueuemutex.Unlock();
		return nSize; 
	}

	virtual void OnConnect(HPR_BOOL bConnected);
	virtual void OnRecv();
	HPR_INT32 ConnectAsync(const char* szIP, HPR_INT32 nPort);
	
	HPR_INT32 SendMsgAsync(const char* szBuf, HPR_INT32 nBufSize);
	void Stop();

	virtual HPR_INT32 SendBufferAsync();
	virtual HPR_BOOL CheckWrite();
	void SetRemoteIP(const char* szIP) { strcpy(m_szRemoteIP, szIP); }
	virtual const char* GetRemoteIP() const {
		return m_szRemoteIP;
	}
	void SetRemotePort(HPR_INT32 nPort) { m_nRemotePort = nPort; }
	virtual HPR_INT32 GetRemotePort() {
		return m_nRemotePort;
	}
	
	/*HPR_UINT32 nsockid*/
	sigslot::signal1<HPR_UINT32> DoConnect; 
	/*HPR_UINT32 nsockid, const char* szBuf, HPR_INT32 nBufSize, const char* szIP, HPR_INT32 nPort*/
	sigslot::signal5<HPR_UINT32, const char*, HPR_INT32, const char*, HPR_INT32 > DoRecv;
	/*HPR_UINT32 nsockid, HPR_INT32 nErrorCode*/
	sigslot::signal2<HPR_UINT32, HPR_INT32> DoException;
	/*HPR_UINT32 nsockid*/
	sigslot::signal1<HPR_UINT32> DoClose;
private:
	char m_szRemoteIP[32];
	HPR_INT32 m_nRemotePort;
	CBufferLoop m_recvbuffer;

	queue<CBufferLoop*> m_sendqueue;		//待发送队列，只有非阻塞的TCP socket才会用到
	HPR_Mutex m_sendqueuemutex;

};
