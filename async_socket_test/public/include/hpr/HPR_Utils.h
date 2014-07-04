/*
 * CopyRight:	HangZhou Hikvision System Technology Co., Ltd. All	Right Reserved.
 * FileName:	HRP_Utils.h
 * Desc:		OS Layer Component.
 * Author:		schina
 * Date:		2009-03-02
 * Contact:		zhaoyiji@hikvision.com.cn
 * History:		Created By Zhaoyiji 2009-03-02
 * */

/*! 
	\file HPR_Utils.h
	\brief HPR Utils Manager.

	Details.
*/

#ifndef __HPR_UTILS_H__
#define __HPR_UTILS_H__

#include "HPR_Config.h"
#include "HPR_Types.h"

#if defined LLONG_MAX
	#define HPR_INT64_MAX (LLONG_MAX) //(HPR_INT64)(0x7fffffffffffffff)
	#define HPR_INT64_MIN (LLONG_MIN) //((HPR_INT64)0x7fffffffffffffff) - (HPR_INT64)1)
#else
	#define HPR_INT64_MAX (HPR_INT64)(0x7fffffffffffffff)
	#define HPR_INT64_MIN ((HPR_INT64)(0x7fffffffffffffff) - (HPR_INT64)1)
#endif
#define HPR_INT32_MAX (INT_MAX)
#define HPR_INT32_MIN (INT_MIN)

#if defined(OS_WINDOWS)
	#define HPR_Vsnprintf _vsnprintf
	#define HPR_Snprintf _snprintf
#elif defined(OS_POSIX)
	#define HPR_Vsnprintf vsnprintf
	#define HPR_Snprintf snprintf
#else
	#error OS Not Implement Yet.
#endif

#if defined (HPR_SUPPORT_NOTHROW)
#define HPR_NEW_RETURN(POINTER,CONSTRUCTURE) \
	do { POINTER = new (::std::nothrow) CONSTRUCTURE;\
	if (POINTER == 0) {return NULL;}\
	return POINTER;\
	}while(0)
#else
#define HPR_NEW_RETURN(POINTER,CONSTRUCTURE) \
	do {	try {POINTER = new CONSTRUCTURE;\
			return POINTER;}\
	catch (::std::bad_alloc & a){POINTER=0,return NULL}\
		}while(0)
#endif


struct HPR_CPU_PERFORMANCE
{
	//in 100-nanosecond intervals.
	//total time = kernel + user + res1[0] + res1[1]
	//cpu usage = (total - idle) / total
	HPR_UINT64 u64IdleTime;
	HPR_UINT64 u64KernelTime;
	HPR_UINT64 u64UserTime;
	HPR_UINT64 u64Reserve1[2];
	HPR_UINT64 u64Reserve2[2];	//useless by now
};

struct HPR_MEMORY_STATUS
{
	//in bytes
	HPR_UINT64 u64TotalPhys;
	HPR_UINT64 u64AvailPhys;
	HPR_UINT64 u64TotalPageFile;	//swap in linux
	HPR_UINT64 u64AvailPageFile;
	HPR_UINT64 u64Reserve[2];
};

struct HPR_NETWORK_FLOWDATA
{
	char szAdapterName[260];	//[in] get from HPR_ADAPTER_INFO.szAdapterName {req in linux}
	HPR_INT32 iIndex;	//[in] get from HPR_ADAPTER_INFO.iIndex
	HPR_UINT32 nSpeed;	//in bps
	HPR_UINT32 nInBytes;
	HPR_UINT32 nInPackets;
	HPR_UINT32 nOutBytes;
	HPR_UINT32 nOutPackets;
	HPR_UINT32 nReserve[4];
};


/**
 * HPR_OutputDebug format and print data in debug/release mode.
 * @param format (IN) string format.
 * @return void
 * @sa HPR_Debug
 */
HPR_DECLARE HPR_VOID CALLBACK HPR_OutputDebug(const char* format, ...);


/**
 * HPR_Debug format and print data in debug mode.
 * @param format (IN) string format.
 * @return void
 * @sa HPR_OutputDebug
 */
#if defined _DEBUG
	#define HPR_Debug HPR_OutputDebug
#else
	#define HPR_Debug 
#endif

/**
 * HPR_OutputDebugString output string directly in debug/release mode.
 * @param format (IN) string pointer.
 * @return void
 * @sa HPR_OutputDebug
 */
HPR_DECLARE HPR_VOID CALLBACK HPR_OutputDebugString(const char* lpOutputString);

/**
 * HPR_USleep suspend execute for millisecond intervals
 * @param millisecond (IN) count of millisecond interval
 * @return void
 * @sa HPR_USleep
 */
HPR_DECLARE HPR_VOID CALLBACK HPR_Sleep(HPR_INT32 millisecond);

/**
 * HPR_USleep suspend execute for microsecond intervals
 * @param nUSec (IN) count of microsecond interval
 * @return void
 * @sa HPR_Sleep
 */
HPR_DECLARE HPR_VOID CALLBACK HPR_USleep(HPR_INT64 nUSec);

/**
 * Function: HPR_Rand
 * Desc:	获取随机数,随机数种子由内部HPR_GetTimeTick()获取
 * Input:	
 * Output:
 * Return:	随机数
 */
HPR_DECLARE HPR_UINT32 CALLBACK HPR_Rand();

/**
 * Function: HPR_ZeroMemory
 * Desc:	把内存清零调用 memset(mem, 0, len);
 * Input:	@param mem : 内存首指针
			@param len : 内存长度
 * Output:
 * Return:	
 */
HPR_DECLARE HPR_VOID CALLBACK HPR_ZeroMemory(void* mem, int len);

/**
 * Function: HPR_GetCPUNumber
 * Desc:	获取CPU个数
 * Input:	
 * Output:
 * Return:	
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_GetCPUNumber();

typedef unsigned char HPR_UUID[16];
HPR_DECLARE HPR_INT32 CALLBACK HPR_GetUUID(HPR_UUID uuid);
HPR_DECLARE HPR_INT32 CALLBACK HPR_StringFromUUID(HPR_UUID uuid, char* uuidString, HPR_INT32 len);

//////////////////////////////////////////////////////////////////////////
//UTF8编码到多字节编码之间的转换

#if defined (CXXCOMPILE)
#include <string>
HPR_DECLARE_CXX std::string CALLBACK HPR_UTF82A(const char* cont);

HPR_DECLARE_CXX std::string CALLBACK HPR_A2UTF8(const char* cont);
#endif
//////////////////////////////////////////////////////////////////////////


HPR_DECLARE HPR_INT32 CALLBACK HPR_GetCPUPerformance(HPR_CPU_PERFORMANCE *performance, HPR_UINT32 *size);
HPR_DECLARE HPR_INT32 CALLBACK HPR_GetMemoryStatus(HPR_MEMORY_STATUS *memory_status);
HPR_DECLARE HPR_INT32 CALLBACK HPR_GetNetWorkFlowData(HPR_NETWORK_FLOWDATA *flowdata);

#endif
