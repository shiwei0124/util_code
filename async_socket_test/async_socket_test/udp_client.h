/**	@file udp_client.h
 *	@note  
 *	@brief 主要用于UDP Client/Server端的对象
 *
 *	@author		shiwei
 *	@date		2014/05/05
 *
 *	@note 这里填写本文件的详细功能描述和注解
 *	@note 历史记录：
 *	@note V1.0.0  创建文件
 */
#ifndef _UDP_CLIENT_H
#define _UDP_CLIENT_H

#include "socket_io.h"
#include "base_io_stream.h"
#include <map>
#include "sigslot.h"
using namespace std;
using namespace sigslot;


class SOCKET_IO_DECLARE_CLASS CUDPClient : public CBaseIOStream
{
public:
	CUDPClient(CIOLoop* pIO);
	virtual ~CUDPClient(void);

	void OnRecv();
	int32_t SendMsg(const char* szIP, int32_t nPort, const char* szMsg, int32_t nMsgLength);
	void Stop();

	/*uint32_t nsockid, const char* szBuf, int32_t nBufSize, const char* szIP, int32_t nPort*/
	sigslot::signal5<uint32_t, const char*, int32_t, const char*, int32_t > DoRecv;
	/*uint32_t nsockid, int32_t nErrorCode*/
	sigslot::signal2<uint32_t, int32_t> DoException;
	/*uint32_t nsockid*/
	sigslot::signal1<uint32_t> DoClose;
/* function */
private:

/* property */
private:
};
#endif
