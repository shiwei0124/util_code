/*
 * CopyRight:	HangZhou Hikvision System Technology Co., Ltd. All	Right Reserved.
 * FileName:	HRP_Timer.h
 * Desc:		OS Layer Component.
 * Author:		schina
 * Date:		2009-03-02
 * Contact:		zhaoyiji@hikvision.com.cn
 * History:		Created By Zhaoyiji 2009-03-02
 * */

/*! 
	\file HPR_Timer.h
	\brief HPR Timer Manager.

	Details.
*/

#ifndef __HPR_TIMER_H__
#define __HPR_TIMER_H__

#include "HPR_Config.h"
#include "HPR_Types.h"

/*
定时器方案：
定时器采用多媒体时钟+异步时钟的模式,多媒体时钟由于系统限制最多支持16个，
如果多媒体时钟分配失败，则分配异步时钟

时钟选择：HPR_SetTimer() bFlag=HPR_FALSE不使用多媒体时钟

定时器精度：多媒体时钟1ms,异步时钟1ms.

限制：定时器最多1500个
*/

typedef HPR_VOIDPTR (* HPR_CBF_Timer)(HPR_INT32 hEvent, HPR_VOIDPTR pUsrData);

/**
 * HPR_SetTimer create timer and set timer interval.
 * @param pCbf (IN) user defined function
 * @param pUsrData (IN) user defined data pass to pCbf 
 * @param nTimeOut (IN) timer interval in millisecond interval.
 * @param hEvent (OUT) timer event to create, may use in HPR_KillTimer.
 * @param bFlag (IN) special to use mmtimer. if false, not use mmtimer.
 * @return 0 success, -1 fail.
 * @sa HPR_KillTimer
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_SetTimer(HPR_CBF_Timer pCbf, HPR_VOIDPTR pUsrData,\
									HPR_UINT32 nTimeOut, HPR_INT32* hEvent, HPR_BOOL bFlag);

/**
 * HPR_KillTimer kill timer crated by HPR_SetTimer
 * @param hEvent (IN) timer event created by HPR_SetTimer
 * @return	0 success, -1 fail.
 * @sa HPR_SetTimer
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_KillTimer(HPR_INT32 hEvent); 

#endif
