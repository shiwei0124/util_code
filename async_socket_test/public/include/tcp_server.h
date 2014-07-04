#pragma once
#include "socket_io.h"
#include "base_io_stream.h"
#include "sigslot.h"
using namespace sigslot;
class SOCKET_IO_DECLARE_CLASS CTCPServer :
	public CBaseIOStream
{
public:
	CTCPServer(CIOLoop* pIO);
	~CTCPServer(void);

	void OnAccept();
	void Listen();
	void Stop();

	/*HPR_UINT32 nsockid, HPR_SOCK_T nSock, const char* szIP, HPR_INT32 nPort*/
	sigslot::signal4<HPR_UINT32, HPR_SOCK_T, const char*, HPR_INT32 > DoAccept;

	/*HPR_UINT32 nsockid*/
	sigslot::signal1<HPR_UINT32> DoClose;
	
private:
};
