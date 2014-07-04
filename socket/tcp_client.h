#pragma once
#include "base_io_stream.h"
#include "base_handler.h"


class CTCPClient : public CBaseIOStream
{
public:
	CTCPClient(void);
	virtual ~CTCPClient(void);

	void OnRecv();
	void RegisterHandler(CBaseHandler* pHandler);
	HPR_INT32 Connect(const char* szIP, HPR_INT32 nPort);
	
	HPR_INT32 SendMsg(const char* szBuf, HPR_INT32 nBufSize);
	void Stop();

private:
	CBaseHandler* m_phandler;
};
