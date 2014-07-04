/**	@file udp_client.h
 *	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief 主要用于UDP Client/Server端的对象
 *
 *	@author		shiwei
 *	@date		2014/05/05
 *
 *	@note 这里填写本文件的详细功能描述和注解
 *	@note 历史记录：
 *	@note V1.0.0  创建文件
 */
#pragma once
#include "socket_io.h"
#include "HPR_Hpr.h"
#include "HPR_Mutex.h"
#include "base_io_stream.h"
#include <map>
#include <string>
#include "sigslot.h"
using namespace std;
using namespace sigslot;


class SOCKET_IO_DECLARE_CLASS CUDPClient : public CBaseIOStream
{
public:
	CUDPClient(CIOLoop* pIO);
	virtual ~CUDPClient(void);

	void OnRecv();
	HPR_INT32 SendMsg(const char* szIP, HPR_INT32 nPort, const char* szMsg, HPR_INT32 nMsgLength);
	void Stop();

	/*HPR_UINT32 nsockid, const char* szBuf, HPR_INT32 nBufSize, const char* szIP, HPR_INT32 nPort*/
	sigslot::signal5<HPR_UINT32, const char*, HPR_INT32, const char*, HPR_INT32 > DoRecv;
	/*HPR_UINT32 nsockid, HPR_INT32 nErrorCode*/
	sigslot::signal2<HPR_UINT32, HPR_INT32> DoException;
	/*HPR_UINT32 nsockid*/
	sigslot::signal1<HPR_UINT32> DoClose;
/* function */
private:

/* property */
private:
};
