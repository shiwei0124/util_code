#pragma once
#include "HPR_Hpr.h"
#include "HPR_Socket.h"
class CBaseHandler
{
public:
	CBaseHandler(void) {}
	virtual ~CBaseHandler(void) {}

	virtual void OnAccept(HPR_UINT32 nsockid, HPR_SOCK_T nSock, const char* szIP, HPR_INT32 nPort) {}
	virtual void OnRecv(HPR_UINT32 nsockid, const char* szBuf, HPR_INT32 nBufSize, const char* szIP, HPR_INT32 nPort) {}
	virtual void OnException(HPR_UINT32 nsockid, HPR_INT32 nErrorCode) {}
	virtual void OnClose(HPR_UINT32 nsockid) {}
};
