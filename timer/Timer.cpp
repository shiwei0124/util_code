// Timer.cpp: implementation of the CTimer class.
//
//////////////////////////////////////////////////////////////////////

#include "Timer.h"
#include "hpr/HPR_Thread.h"
#include <assert.h>
#include <stdio.h>
#include "hpr/HPR_Utils.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTimer::CTimer()
: m_iQuit(0),
//m_Interval(-1),
m_pTimerProc(NULL),
m_bThreadFinished(true),
m_StartTimer(false)
{
}

CTimer::~CTimer()
{
	StopTimer();
}

//static CTimer s_cTimer;
//CTimer* CTimer::GetInstance()
//{
//	return &s_cTimer;
//}

int CTimer::StartTimer(int nIndex, pTimerProc pfnTimerProc, unsigned int nMilliSeconds, void* param)
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
		HPR_INT32 nRet = HPR_ThreadDetached_Create(ThreadProc, this, 0);
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
int CTimer::StopTimer()
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

void* CTimer::ThreadProc(void* pParam)
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

int CTimer::KillTimer( int nIndex )
{
	m_MapMutex.Lock();
	m_MapInterval.erase(nIndex);
	m_MapParam.erase(nIndex);
	m_MapMutex.Unlock();
	return 0;
}