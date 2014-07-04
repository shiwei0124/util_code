/**	@file tcp_server.h
 *	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief 主要用于TCP Server端的对象
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
