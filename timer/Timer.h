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
#include <map>
#include <utility>
using namespace std;

typedef void  (CALLBACK *pTimerProc)(int, void* );


class CTimer
{
public:
	CTimer();
	virtual ~CTimer();

public:

	//启动定时器
	int StartTimer(int nIndex, pTimerProc pfnTimerProc, unsigned int nMilliSeconds, void* param);

	//停止定时器
	int StopTimer();

	int KillTimer(int nIndex);

	//获取唯一实例
	//static CTimer* GetInstance();

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

	static  void* CALLBACK ThreadProc(void* pParam);
};

#endif // !defined(AFX_TIMER_H__A98C6783_0D74_4BF4_9CE9_496A49D828A0__INCLUDED_)
