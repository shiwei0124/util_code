#pragma once
#include "HPR_Hpr.h"
#include "HPR_Mutex.h"
#include "base_io_stream.h"
#include "session_msg.h"
#include "base_handler.h"
#include <map>
#include <string>
using namespace std;


class CUDPClient : public CBaseIOStream
{
public:
	CUDPClient(void);
	virtual ~CUDPClient(void);

	void OnRecv();
	void RegisterHandler(CBaseHandler* pHandler);
	HPR_INT32 SendMsg(const char* szIP, HPR_INT32 nPort, const char* szMsg, HPR_INT32 nMsgLength);
	void Stop();
/* function */
private:

/* property */
private:
	CBaseHandler* m_phandler;
};
