/**	@file epoll_io_loop.h
 *	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief CEPOLLIOLoop类的头文件。
 *
 *	@author		shiwei
 *	@date		2014/03/20
 *
 *	@note 该类使用epoll的LT模式完成网络IO的读写
 *	@note 历史记录：
 *	@note V1.0.0  创建文件
 */
#pragma once
#include "HPR_Thread.h"
#include "HPR_Mutex.h"
#include "HPR_Socket.h"
#include "base_io_stream.h"
#include "io_loop.h"
#include <sys/epoll.h>  
#include <map>
#include <vector>
using namespace std;

#define EPOLL_SIZE  256

class CEpollIOLoop : public CIOLoop
{
public:
	CEpollIOLoop(void);
	virtual ~CEpollIOLoop(void);

	virtual void Start();
	virtual void Stop();
	virtual void Run();

	virtual void Add_Handler( CBaseIOStream* piostream );
	virtual void Remove_Handler(CBaseIOStream* piostream);

private:
	HPR_INT32 m_eid;
};
