#pragma once
#include "hpr/HPR_Hpr.h"
#include "sigslot.h"
using namespace sigslot; 
class CTCPHandler : public has_slots<>
{
public:
	CTCPHandler(void);
	~CTCPHandler(void);

	virtual void OnConnect(HPR_UINT32 nsockid);
	virtual void OnRecv(HPR_UINT32 nsockid, const char* szBuf, HPR_INT32 nBufSize, const char* szIP, HPR_INT32 nPort);
	virtual void OnException(HPR_UINT32 nsockid, HPR_INT32 nErrorCode);
	virtual void OnClose(HPR_UINT32 nsockid);
};
