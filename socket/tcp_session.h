#pragma once
#include "base_io_stream.h"
#include "base_handler.h"
class CTCPSession :
	public CBaseIOStream
{
public:
	CTCPSession(void);
	~CTCPSession(void);

	void OnRecv();
	void RegisterHandler(CBaseHandler* pHandler);

	HPR_INT32 SendMsg(const char* szBuf, HPR_INT32 nBufSize);
	void Stop();

private:
	CBaseHandler* m_phandler;
};
