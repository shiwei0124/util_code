#include "io_loop.h"
#include "HPR_Select.h"
CIOLoop::CIOLoop(void)
{
	m_bCloseRequest = HPR_FALSE;
	m_threadhandle = HPR_INVALID_THREAD;
}

CIOLoop::~CIOLoop(void)
{
}

/**	@fn	void CIOLoop::Start()
*	@brief 
*	@return	
*/
void CIOLoop::Start()
{
	if (m_threadhandle == HPR_INVALID_THREAD)
	{
		m_waker.Start();
		m_bCloseRequest = HPR_FALSE;
		m_threadhandle = HPR_Thread_Create(RunThread, this, 0);
	}
}

/**	@fn	void CIOLoop::Stop()
*	@brief 
*	@return	
*/
void CIOLoop::Stop()
{
	m_bCloseRequest = HPR_TRUE;
	m_waker.Wake();
	if (m_threadhandle != HPR_INVALID_THREAD)
	{
		HPR_Thread_Wait(m_threadhandle);
		m_threadhandle = HPR_INVALID_THREAD;
		m_MapIOStreamBySocket.clear();
	}
	m_waker.Stop();
}

/**	@fn	void CIOLoop::Run()
*	@brief 
*	@return	
*/
void CIOLoop::Run()
{
	fd_set fd_read;
	while (m_bCloseRequest == HPR_FALSE)
	{
		HPR_INT32 nSockNum = 0;
		HPR_INT32 nMaxfd = 0;
		FD_ZERO(&fd_read);
		FD_SET(m_waker.GetWakeSocket(), &fd_read);
		if(m_waker.GetWakeSocket() > nMaxfd) 
			nMaxfd = m_waker.GetWakeSocket();
		nSockNum++;
		m_MapMutex.Lock();
		map<HPR_SOCK_T, CBaseIOStream*> mapTmp = m_MapIOStreamBySocket;
		m_MapMutex.Unlock();
		map<HPR_SOCK_T, CBaseIOStream*>::iterator it = mapTmp.begin();
		for (; it != mapTmp.end(); it++)
		{
			FD_SET(it->first, &fd_read);
			if(it->first > nMaxfd) 
				nMaxfd = it->first;
			nSockNum++;
		}
		timeval tv;
		tv.tv_sec = 10;
		tv.tv_usec = 0;
		HPR_INT32 nRet = HPR_Select(nMaxfd + 1, &fd_read, NULL, NULL, NULL);
		if (nRet > 0)
		{
			if (HPR_FdIsSet(m_waker.GetWakeSocket(), &fd_read))
			{
				//stop
				m_waker.Recv();
			}
			else
			{
				map<HPR_SOCK_T, CBaseIOStream*>::iterator it1 = mapTmp.begin();
				for (; it1 != mapTmp.end(); it1++)
				{
					if (HPR_FdIsSet(it1->first, &fd_read))
					{
						CBaseIOStream* pIOStream = _GetHandlerBySock(it1->first);
						if (pIOStream != NULL)
						{
							if (pIOStream->GetSockType() == SOCK_TCP_SERVER)
							{
								pIOStream->OnAccept();
							}
							else
							{
								pIOStream->OnRecv();
							}	
						}
					}
				}
			}
		}
		else
		{
			
			//TODO
			//Error
		}

	}
}

/**	@fn	HPR_VOIDPTR __stdcall CIOLoop::RunThread(HPR_VOIDPTR ptr)
*	@brief 
*	@param[in] ptr 
*	@return	
*/
HPR_VOIDPTR CIOLoop::RunThread( HPR_VOIDPTR ptr )
{
	CIOLoop* pApp = (CIOLoop*)ptr;
	pApp->Run();
	return NULL;
}

/**	@fn	void CIOLoop::Add_Handler(CBaseIOStream* piostream)
*	@brief 
*	@param[in] piostream 
*	@return	
*/
void CIOLoop::Add_Handler( CBaseIOStream* piostream )
{
	m_MapMutex.Lock();
	m_MapIOStreamBySocket[piostream->GetSocket()] = piostream;
	m_waker.Wake();
	m_MapMutex.Unlock();
}

/**	@fn	void CIOLoop::Remove_Handler(CBaseIOStream* piostream)
*	@brief 
*	@param[in] piostream 
*	@return	
*/
void CIOLoop::Remove_Handler( CBaseIOStream* piostream )
{
	m_MapMutex.Lock();
	m_MapIOStreamBySocket.erase(piostream->GetSocket());
	m_waker.Wake();
	m_MapMutex.Unlock();
}

/**	@fn	CBaseIOStream* CIOLoop::_GetHandlerBySock(HPR_SOCK_T sock)
*	@brief 
*	@param[in] sock 
*	@return	
*/
CBaseIOStream* CIOLoop::_GetHandlerBySock( HPR_SOCK_T sock )
{
	CBaseIOStream* pIOStream = NULL;
	map<HPR_SOCK_T, CBaseIOStream*>::iterator it = m_MapIOStreamBySocket.find(sock);
	if (it != m_MapIOStreamBySocket.end())
	{
		pIOStream = it->second;
	}
	return pIOStream;
}
