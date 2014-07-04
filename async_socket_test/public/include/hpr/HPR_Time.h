/*
 * CopyRight:	HangZhou Hikvision System Technology Co., Ltd. All	Right Reserved.
 * FileName:	HPR_Time.h
 * Desc:		OS Layer Component.
 * Author:		schina
 * Date:		2009-03-02
 * Contact:		zhaoyiji@hikvision.com.cn
 * History:		Created By Zhaoyiji 2009-03-02
 * */

/*! 
	\file HPR_Time.h
	\brief HPR Time Manager.

	Details.
*/

#ifndef __HPR_TIME_H__
#define __HPR_TIME_H__

#include "HPR_Config.h"
#include "HPR_Types.h"

/*! \def OS_WINDOWS
 *
 */

#if defined(OS_WINDOWS)
#if !defined OS_WINCE
#include <sys/timeb.h>
#endif
typedef SYSTEMTIME HPR_OS_EXP_TIME_T;
typedef FILETIME HPR_OS_TIME_T;
#elif defined(OS_POSIX)
#else
	#error OS Not Implement Yet.
#endif

typedef HPR_INT64 HPR_TIME_T; //(usec)

/**
 * struct HPR_TIME_EXP_T
 */
typedef struct HPR_TIME_EXP_S
{
	HPR_INT32 tm_usec;	/**<	micorseconds past tm_sec	*/
	HPR_INT32 tm_sec;	/**<	(0-59)secs past tm_min		*/
	HPR_INT32 tm_min;	/**<	(0-59)minutes past tm_hour	*/
	HPR_INT32 tm_hour;	/**<	(0-23)hours past midnight	*/
	HPR_INT32 tm_mday;	/**<	(0-31)day of month			*/
	HPR_INT32 tm_mon;	/**<	(0-11)month of year			*/
	HPR_INT32 tm_year;	/**<	year since 1900				*/
	HPR_INT32 tm_wday;	/**<	(0-6)days since sunday		*/
	HPR_INT32 tm_yday;	/**<	(0-365)days since Jan 1		*/
	HPR_INT32 tm_isdst; /**<	daylight saving time		*/
	HPR_INT32 tm_gmtoff;/**<	seconds east of UTC			*/
}HPR_TIME_EXP_T;

/**
 * HPR_TimeNow get now time
 * @return number of microsecond since January 1, 1970.
 * @sa HPR_GetTimeTick
 */
HPR_DECLARE HPR_TIME_T CALLBACK HPR_TimeNow();

/**
 * HPR_GetTimeTick get now time
 * @return number of milliseconds since system start. cpu time.
 * @sa HPR_TimeNow
 */
HPR_DECLARE HPR_UINT32 CALLBACK HPR_GetTimeTick();

/**
 * HPR_TimeFromAnsiTime convert time_t time to HPR_TIME_T time
 * @param tAnsiTime (IN) ansi time (may get by time())
 * @return HPR_TIME_T time
 * @sa HPR_TimeNow(),HPR_AnsiTimeFromTime().
 */
HPR_DECLARE HPR_TIME_T CALLBACK HPR_TimeFromAnsiTime(HPR_TIME_T tAnsiTime);

/**
 * HPR_AnsiTimeFromTime convert HPR_TIME_T time to time_t time
 * @param tAnsiTime (IN) ansi time (may get by time())
 * @return time_t time (from 1970.1.1)
 * @sa HPR_TimeFromAnsiTime().
 */
HPR_DECLARE HPR_TIME_T CALLBACK HPR_AnsiTimeFromTime(HPR_TIME_T iTime);

/**
 * HPR_TimeFromExpTime convert HPR_TIME_EXP_T time to HPR_TIME_T time
 * @param pExpTime (IN) HPR_TIME_EXP_T format time
 * @param pTime (OUT) interger time.
 * @return 0 success, -1 fail.
 * @sa HPR_TimeFromExpTimeGMT
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_TimeFromExpTime(HPR_TIME_EXP_T* pExpTime, HPR_TIME_T* pTime);

/**
 * HPR_TimeFromExpTimeGMT convert HPR_TIME_EXP_T time to HPR_TIME_T time
 * @param pExpTime (IN) HPR_TIME_EXP_T format time
 * @param pTime (OUT) interger time.
 * @return 0 success, -1 fail.
 * @sa HPR_TimeFromExpTime
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_TimeFromExpTimeGMT(HPR_TIME_EXP_T* pExpTime, HPR_TIME_T* pTime);

/**
 * HPR_ExpTimeFromTime convert HPR_TIME_T time to HPR_TIME_EXP_T
 * @param nTime (IN) interger time
 * @param iOff timezone (IN) from gmt time, in sec
 * @param pExpTime (OUT) struct HPR_TIME_EXP_T time
 * @return 0 success, -1 fail.
 * @sa HPR_TimeFromExpTimeGMT(),HPR_ExpTimeFromTimeLocal() 
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_ExpTimeFromTime(HPR_TIME_T nTime, HPR_INT32 iOff, HPR_TIME_EXP_T* pExpTime);

/**
 * HPR_ExpTimeFromTimeGMT convert HPR_TIME_T time to HPR_TIME_EXP_T time
 * @param iTime (IN) interger time
 * @param pExpTime (OUT) struct HPR_TIME_EXP_T time
 * @return 0 success, -1 fail.
 * @see HPR_TIME_EXP_T
 * @sa HPR_ExpTimeFromTimeLocal
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_ExpTimeFromTimeGMT(HPR_TIME_T iTime, HPR_TIME_EXP_T* pExpTime);

/**
 * HPR_ExpTimeFromTimeLocal convert HPR_TIME_T time to HPR_TIME_EXP_T time
 * @param iTime (IN) interger time
 * @param pExpTime (OUT) struct HPR_TIME_EXP_T time
 * @return 0 success, -1 fail.
 * @see HPR_TIME_EXP_T
 * @sa HPR_ExpTimeFromTimeGMT
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_ExpTimeFromTimeLocal(HPR_TIME_T iTime, HPR_TIME_EXP_T* pExpTime);

/**
 * HPR_CTime convert HPR_TIME_T to formated string time.
 * @param pDateStr (OUT) data buffer to store formated time.
 * @param nTime (IN) interger time format.
 * @return 0 success, -1 fail.
 * @see HPR_TIME_T
 * @sa HPR_StrFmtTime
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_CTime(char* pDateStr, HPR_TIME_T nTime);

/**
 * HPR_StrFmtTime
 * @param pDst (OUT dest data buffer to store formated time.
 * @param iDstLen (OUT) formated time length
 * @param iMaxDstSize (IN) dest data buffer length.
 * @param pFmt (IN) time format
 * @param pTimeExp (IN) struct HPR_TIME_EXP_T formated time.
 * @return 0 success, -1 fail.
 * @sa HPR_CTime()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_StrFmtTime(char* pDst, HPR_INT32* iDstLen, HPR_INT32 iMaxDstSize, const char* pFmt, HPR_TIME_EXP_T* pTimeExp);

#if 0

class HPR_DECLARE_CLASS HPR_Time_Value
{
public:
	HPR_Time_Value(const HPR_INT64 sec, const HPR_INT64 usec);
	HPR_Time_Value(const timeval& tv);
	HPR_Time_Value();

	~HPR_Time_Value();

public:
	void USec(HPR_TIME_T usec)
	{
		m_time = usec;
	};

	HPR_TIME_T USec() const
	{
		return m_time;
	};

	void MSec(HPR_TIME_T msec)
	{
		m_time = msec*1000;
	};

	HPR_TIME_T MSec() const
	{
		return m_time/1000;
	};

	void Set(const timeval &tv)
	{
		m_time = tv.tv_sec*1000000 + tv.tv_usec;
	};

	void Set(HPR_INT64 t)
	{
		m_time = t;
	};

	void Set(HPR_INT64 sec, HPR_INT64 usec)
	{
		m_time = sec*1000000 + usec;
	};

public:
	HPR_Time_Value& operator-- (void);
	HPR_Time_Value& operator++ (void);
	HPR_Time_Value&	operator += (HPR_Time_Value& tv);
	HPR_Time_Value& operator -= (HPR_Time_Value& tv);
	operator HPR_TIME_T () const
	{
		return this->m_time;
	};

	friend HPR_DECLARE_CLASS HPR_BOOL operator == (const HPR_Time_Value& tv1, const HPR_Time_Value& tv2);
	friend HPR_DECLARE_CLASS HPR_BOOL operator != (const HPR_Time_Value& tv1, const HPR_Time_Value& tv2);
	//
	friend HPR_DECLARE_CLASS HPR_Time_Value operator + (HPR_Time_Value& tv1, HPR_Time_Value& tv2);
	friend HPR_DECLARE_CLASS HPR_Time_Value operator - (HPR_Time_Value& tv1, HPR_Time_Value& tv2);

	friend HPR_DECLARE_CLASS HPR_BOOL operator > (const HPR_Time_Value& tv1, const HPR_Time_Value& tv2);
	friend HPR_DECLARE_CLASS HPR_BOOL operator >= (const HPR_Time_Value& tv1, const HPR_Time_Value& tv2);

	friend HPR_DECLARE_CLASS HPR_BOOL operator < (const HPR_Time_Value& tv1, const HPR_Time_Value& tv2);
	friend HPR_DECLARE_CLASS HPR_BOOL operator <= (const HPR_Time_Value& tv1, const HPR_Time_Value& tv2);


private:
	HPR_TIME_T m_time;
};

#endif

#endif



