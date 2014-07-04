// Timer.h: interface for the CTimer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMER_H__A98C6783_0D74_4BF4_9CE9_496A49D828A0__INCLUDED_)
#define AFX_TIMER_H__A98C6783_0D74_4BF4_9CE9_496A49D828A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "hpr/HPR_Mutex.h"
#include "hpr/HPR_Config.h"
#include "hpr/HPR_Utils.h"
#include "hpr/HPR_Thread.h"
#include <map>
#include <utility>
#include <assert.h>
#include <stdio.h>

using namespace std;

typedef void  (CALLBACK *pTimerProc)(int, void* );


class CTimer
{
public:
	CTimer()
	{
		m_iQuit = 0;
		m_pTimerProc = NULL;
		m_bThreadFinished = true;
		m_StartTimer = false;
	}
	virtual ~CTimer()
	{
		StopTimer();
	}

public:

	//启动定时器
	int StartTimer(int nIndex, pTimerProc pfnTimerProc, unsigned int nMilliSeconds, void* param)
	{
		int iRet = -1;
		assert(pfnTimerProc != NULL);
		m_Mutex.Lock();
		if (m_StartTimer == false)
		{
			m_iQuit = 0;
			m_MapMutex.Lock();
			m_MapInterval[nIndex] = make_pair(nMilliSeconds, 0);
			m_MapParam[nIndex] = param;
			m_MapMutex.Unlock();
			m_pTimerProc = pfnTimerProc;

			//set to false before thread running
			m_bThreadFinished = false;
			int nRet = HPR_ThreadDetached_Create(ThreadProc, this, 0);
			if (nRet < 0)
			{
				//thread not run
				m_bThreadFinished = true;
			}
			else
			{
				iRet = 0;
				m_StartTimer = true;
			}
		}
		else
		{
			m_MapMutex.Lock();
			m_MapInterval[nIndex] = make_pair(nMilliSeconds, 0);
			m_MapParam[nIndex] = param;
			m_MapMutex.Unlock();
		}
		m_Mutex.Unlock();
		return iRet;
	}

	//停止定时器
	int StopTimer()
	{
		m_iQuit = 1;
		while (!m_bThreadFinished)
		{
			HPR_Sleep(100);
		}
		//m_Interval = -1;
		m_MapInterval.clear();
		m_MapParam.clear();
		m_pTimerProc = NULL;
		m_StartTimer = false;
		return 0;
	}

	int KillTimer(int nIndex)
	{
		m_MapMutex.Lock();
		m_MapInterval.erase(nIndex);
		m_MapParam.erase(nIndex);
		m_MapMutex.Unlock();
		return 0;
	}

	static  void* CALLBACK ThreadProc(void* pParam)
	{
		//detach the thread
		CTimer* pInstance = reinterpret_cast<CTimer*>(pParam);
		pInstance->m_bThreadFinished = false;
		int iTime = 0;
		while (pInstance->m_iQuit == 0)
		{
			//Sleep( pInstance->m_Interval );
			HPR_Sleep(500);
			//iTime += 500;

			//复制一个MAP表进行操作，即使在m_pTimerProc()函数中再次创建一个定时器，也不会出现死锁
			pInstance->m_MapMutex.Lock();
			map<int, pair<unsigned int, unsigned int > > tmp = pInstance->m_MapInterval;
			map<int, void*> paramtmp = pInstance->m_MapParam;
			pInstance->m_MapMutex.Unlock();

			map<int, pair<unsigned int, unsigned int > >::iterator iter = tmp.begin();
			while (iter != tmp.end())
			{
				iter->second.second += 500;
				//printf("\n时间打印，%d       %d\n", iter->first, iter->second.second);
				if (iter->second.second >= iter->second.first)
				{
					pInstance->m_pTimerProc(iter->first, paramtmp[iter->first]);
					//触发定时器，则在原MAP表中将当前时间重置
					pInstance->m_MapMutex.Lock();
					map<int, pair<unsigned int, unsigned int > >::iterator it = pInstance->m_MapInterval.find(iter->first);
					if (it != pInstance->m_MapInterval.end())
					{
						//当前时间重置为0
						it->second.second = 0;
					}
					pInstance->m_MapMutex.Unlock();
				}
				else
				{
					//没有触发定时器，则在原MAP表中将时间叠加
					pInstance->m_MapMutex.Lock();
					map<int, pair<unsigned int, unsigned int > >::iterator it = pInstance->m_MapInterval.find(iter->first);
					if (it != pInstance->m_MapInterval.end())
					{
						//当前时间继续增加
						it->second.second += 500;
					}
					pInstance->m_MapMutex.Unlock();
				}
				iter++;
			}
		}
		pInstance->m_bThreadFinished = true;
		return NULL;
	}
public:
	HPR_Mutex m_Mutex;
	HPR_Mutex m_MapMutex;

	//Quit flag 1:quit; 0--not
	int m_iQuit;

	//unsigned int m_Interval;

	map<int/* nIndex */, pair<unsigned int/* time of miliseconds 预设定时器的时间 */ , unsigned int/* time of miliseconds，当前时间 */> > m_MapInterval;            //最多可以设置10个定时器	
	map<int/* nIndex */, void* > m_MapParam;
	bool m_StartTimer;

	//时间线程
	//HPR_HANDLE m_hTimeThread;

	//线程是否结束标志, false:时间线程还没有结束; true: 时间线程结束了
	bool m_bThreadFinished;

	//定时回调函数
	pTimerProc m_pTimerProc;
};

#endif // !defined(AFX_TIMER_H__A98C6783_0D74_4BF4_9CE9_496A49D828A0__INCLUDED_)
