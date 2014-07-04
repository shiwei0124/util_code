#pragma once
#include "base_io_stream.h"
#include "base_handler.h"
class CTCPServer :
	public CBaseIOStream
{
public:
	CTCPServer(void);
	~CTCPServer(void);

	void OnAccept();
	void Listen();
	void RegisterHandler(CBaseHandler* pHandler);
	void Stop();

private:
	CBaseHandler* m_phandler;
};
