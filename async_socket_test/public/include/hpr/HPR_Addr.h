/*
 * CopyRight:	HangZhou Hikvision System Technology Co., Ltd. All	Right Reserved.
 * FileName:	HRP_Addr.h
 * Desc:		OS Layer Component.
 * Author:		schina
 * Date:		2009-03-02
 * Contact:		zhaoyiji@hikvision.com.cn
 * History:		Created By Zhaoyiji 2009-03-02
 * */

/*! 
	\file HPR_Addr.h
	\brief HPR Network Address Manager.

	Details.
*/

#ifndef __HPR_ADDR_H__
#define __HPR_ADDR_H__

#include "HPR_Config.h"
#include "HPR_Types.h"


#if defined(OS_WINDOWS)
	typedef SOCKET HPR_SOCK_T;
	#if (_MSC_VER < 1310)
		#ifndef s6_addr
			struct in6_addr {
				union {
					u_char Byte[16];
					u_short Word[8];
				} u;
			};
			#define in_addr6 in6_addr
			/*
			** Defines to match RFC 2553.
			*/
			#define _S6_un     u
			#define _S6_u8     Byte
			#define s6_addr    _S6_un._S6_u8
			/*
			** Defines for our implementation.
			*/
			#define s6_bytes   u.Byte
			#define s6_words   u.Word
		#endif

		typedef struct {
			short   sin6_family;        /* AF_INET6 */
			u_short sin6_port;          /* Transport level port number */
			u_long  sin6_flowinfo;      /* IPv6 flow information */
			struct in6_addr sin6_addr;  /* IPv6 address */
			u_long sin6_scope_id;       /* set of interfaces for a scope */
		}sockaddr_in6_hpr;
	#else
		typedef sockaddr_in6 sockaddr_in6_hpr;
	#endif
#elif defined(OS_POSIX)
	typedef int HPR_SOCK_T;
	typedef sockaddr_in6 sockaddr_in6_hpr;
#else
	#error OS Not Implement Yet.
#endif



/**
 * struct HPR_ADDR_T
 */
typedef struct HPR_ADDR_T
{
	union
	{
		sockaddr_in sin4;	/**< IPV4 地址*/
#if defined(OS_SUPPORT_IPV6)
		sockaddr_in6_hpr sin6;	/**< IPV6 地址*/
#endif
	}SA;
}HPR_ADDR_T;

typedef struct HPR_ADDR_EXP_T
{
	HPR_INT32 iAf;				/**< 地址族 AF_INET或AF_INET6*/
	union
	{
		HPR_UINT32 nAddr4;		/**< IPV4整形地址 (网络字节序)*/
		HPR_UINT8 nAddr6[16];	/**< IPV6整形地址 (网络字节序)*/
	}ADDR;
	HPR_UINT16 nPort;			/**< 端口号(主机字节序)*/
	HPR_UINT16 nReserved;		/**< 保留*/
}HPR_ADDR_EXP_T;

#define HPR_AF_INET		AF_INET
#if defined(OS_SUPPORT_IPV6)
#define HPR_AF_INET6	AF_INET6
#endif

/**
 * HPR_MakeAddrByString 通过地址族类型,字符串格式的地址及端口号组建HPR网络地址结构.
 * @param iAf (IN) 地址族类型（AF_INET 或 AF_INET6）.
 * @param pAddr (IN) 以'\0'结尾的字符串格式网络地址.
 * @param nPort (IN) 网络端口号(主机字节序).
 * @param pHprAddr (OUT) HPR网络地址结构.
 * @return 0 成功, -1 失败.
 * @sa HPR_MakeAddr4ByInt(), HPR_MakeAddr6ByInt(), HPR_MakeAddr()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_MakeAddrByString(HPR_INT32 iAf, const char* pAddr, HPR_UINT16 nPort, HPR_ADDR_T* pHprAddr);

/**
 * HPR_MakeAddr4ByInt 通过整形地址及端口号组建HPR网络地址结构（IPV4）.
 * @param nAddr (IN) 整形网络地址.
 * @param nPort (IN) 网络端口号（主机字节序）.
 * @param pHprAddr (OUT) HPR网络地址结构.
 * @return 0 成功, -1 失败.
 * @sa HPR_MakeAddrByString(), HPR_MakeAddr6ByInt(), HPR_MakeAddr()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_MakeAddr4ByInt(HPR_UINT32 nAddr, HPR_UINT16 nPort, HPR_ADDR_T* pHprAddr);

#if defined(OS_SUPPORT_IPV6)
/**
 * HPR_MakeAddr6ByInt 通过整形地址及端口号组建HPR网络地址结构（IPV6）.
 * @param nAddr (IN) 整形网络地址.
 * @param nPort (IN) 网络端口号（主机字节序）.
 * @param pHprAddr (OUT) HPR网络地址结构.
 * @return 0 成功, -1 失败.
 * @sa HPR_MakeAddrByString(), HPR_MakeAddr4ByInt(), HPR_MakeAddr()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_MakeAddr6ByInt(HPR_UINT8 nAddr[16], HPR_UINT16 nPort, HPR_ADDR_T* pHprAddr);
#endif

/**
 * HPR_SetAddrPort 设置HPR网络地址结构端口号.
 * @param pHprAddr (IN) HPR网络地址结构的.
 * @param nPort (IN) 网络端口号(主机字节序).
 * @return 0成功，-1失败.
 * @sa HPR_GetAddrPort().
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_SetAddrPort(HPR_ADDR_T* pHprAddr, HPR_UINT16 nPort);

/**
 * HPR_GetAddrType 通过HPR网络地址结构获取地址族（AF_INET或AF_INET6）.
 * @param pHprAddr (IN) HPR网络地址结构指针.
 * @return -1失败，否则返回AF_INET或AF_INET6.
 * @sa HPR_MakeAddr6ByInt(), HPR_MakeAddr4ByInt(), HPR_MakeAddr(), HPR_MakeAddrByString().
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_GetAddrType(HPR_ADDR_T* pHprAddr);

/**
 * HPR_MakeAddr 通过SOCKADDR结构组建HPR网络地址结构.
 * @param pSockAddr (IN) SOCKADDR地址结构指针.
 * @param iSockAddrLen (IN) SOCKADDR地址结构长度.
 * @param pHprAddr (OUT) HPR网络地址结构指针.
 * @return 0 成功, -1 失败.
 * @sa HPR_MakeAddrByString(), HPR_MakeAddr4ByInt(), HPR_MakeAddr46ByInt()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_MakeAddr(HPR_VOIDPTR pSockAddr, HPR_INT32 iSockAddrLen, HPR_ADDR_T* pHprAddr);

/**
 * HPR_GetAddrString 通过HPR网络地址结构获取字符串格式地址.
 * @param pHprAddr (IN) HPR网络地址结构.
 * @return NULL失败，否则返回以'\0'字符串格式网络地址.
 * @sa HPR_GetAddrBySockFd(),HPR_GetAddrPort().
 */
HPR_DECLARE const char* CALLBACK HPR_GetAddrString(HPR_ADDR_T* pHprAddr);

/**
 * HPR_GetAddrPort 获取地址结构的网络端口号.
 * @param pHprAddr (IN) HPR网络地址结构，上层用户自己检测指针有效性.
 * @return 网络端口号(主机字节序).
 * @sa HPR_GetAddrString(),HPR_GetAddr4Int(),HPR_GetAddr6Int(),HPR_GetAddrInfo().
 */
HPR_DECLARE HPR_UINT16 CALLBACK HPR_GetAddrPort(HPR_ADDR_T* pHprAddr);

/**
 * HPR_GetAddr4Int 获取地址结构IPV4整形表示的网络地址.
 * @param pHprAddr (IN) HPR格式网络地址结构.
 * @param nAddr (OUT) 整形网络地址（网络字节序）.
 * @return 0成功，-1失败.
 * @sa HPR_GetAddr6Int(),HPR_GetAddrPort(),HPR_GetAddrInfo().
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_GetAddr4Int(HPR_ADDR_T* pHprAddr, HPR_UINT32* nAddr);

#if defined (OS_SUPPORT_IPV6)
/**
 * HPR_GetAddr6Int 获取地址结构IPV6整形表示的网络地址.
 * @param pHprAddr (IN) HPR格式网络地址结构.
 * @param nAddr (OUT) 整形网络地址（网络字节序）.
 * @return 0成功，-1失败.
 * @sa HPR_GetAddr4Int(),HPR_GetAddrInfo(),HPR_GetAddrPort().
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_GetAddr6Int(HPR_ADDR_T* pHprAddr, HPR_UINT8 nAddr[16]);
#endif

/**
 * HPR_GetAddrInfo 获取网络地址结构信息.
 * @param pHprAddr (IN) HPR格式网络地址结构.
 * @param pHprExp (OUT) 网络地址结构.
 * @return 0成功，-1失败.
 * @sa HPR_GetAddrPort(),HPR_GetAddr4Int(),HPR_GetAddr6Int().
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_GetAddrInfo(HPR_ADDR_T* pHprAddr, HPR_ADDR_EXP_T* pHprExp);

/**
 * HPR_GetAddrBySockFd 通过socket 套接字获取本地机远端地网络址结构.
 * @param iSockFd (IN) socket 套接字.
 * @param pHprLocal (OUT) HPR本地网络地址.
 * @param pRemote (OUT) HPR网络远端地址.
 * @return 0 成功, -1 失败.
 * @sa HPR_GetAddrString(),HPR_MakeAddr4ByInt(),HPR_MakeAddr6ByInt(),HPR_MakeAddr().
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_GetAddrBySockFd(HPR_SOCK_T iSockFd,HPR_ADDR_T* pHprLocal, HPR_ADDR_T* pRemote);


HPR_DECLARE HPR_INT32 CALLBACK HPR_InetPton(const char* pSrc, HPR_VOIDPTR pDst);

HPR_DECLARE const char* CALLBACK HPR_InetNtop(HPR_INT32 iAf, const unsigned char* pSrc, char* pDst, int iCnt);

#endif
