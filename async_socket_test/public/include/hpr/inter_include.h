/*
 * CopyRight:	HangZhou Hikvision System Technology Co., Ltd. All	Right Reserved.
 * FileName:	inter_time.h
 * Desc:		OS Layer Component.
 * Author:		schina
 * Date:		2009-03-02
 * Contact:		zhaoyiji@hikvision.com.cn
 * History:		Created By Zhaoyiji 2009-03-02
 * */
#ifndef __INTER_INCLUDE_H__
#define __INTER_INCLUDE_H__

#include "HPR_Config.h"
#include "HPR_Types.h"

#define HPR_USEC_PER_SEC 1000000
#define HPR_MSEC_PER_SEC 1000
#define HPR_USEC_PER_MSEC 1000

#define HPR_DELTA_EPOCH_IN_USEC   HPR_TIME_T(11644473600000000)

#define HPR_TIME_SEC(time) ((time)/HPR_USEC_PER_SEC)
#define HPR_TIME_MSEC(time) (((time)/HPR_MSEC_PER_SEC)%HPR_MSEC_PER_SEC)
#define HPR_TIME_USEC(time) ((time)%HPR_SEC_PER_USEC)
#define HPR_TIME_FROM_SEC(sec) ((HPR_TIME_T)(sec)*HPR_USEC_PER_SEC)
#define HPR_TIME_MAKE(sec, usec) ((HPR_TIME_T)(sec)*HPR_USEC_PER_SEC + (HPR_TIME_T)usec)

/* Leap year is any year divisible by four, but not by 100 unless also
 * divisible by 400
 */
#define IsLeapYear(y) ((!(y % 4)) ? (((!(y % 400)) && (y % 100)) ? 1 : 0) : 0)

/**
 * SetLastError
 */
#if defined(OS_WINDOWS)
#define SetLastError(HPR_ERRNO)  SetLastError(HPR_ERRNO)
#else
#define SetLastError(HPR_ERRNO)  (errno = HPR_ERROR)
#endif

#endif ///#ifndef __INTER_INCLUDE_H__
