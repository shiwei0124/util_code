/*
 * CopyRight:	HangZhou Hikvision System Technology Co., Ltd. All	Right Reserved.
 * FileName:	HPR_Netdb.h
 * Desc:		define ByteOrder/Addr/MACAddr files.
 * Author:		schina
 * Date:		2009-03-02
 * Contact:		zhaoyiji@hikvision.com.cn
 * History:		Created By Zhaoyiji 2009-03-02
 * */

#ifndef __HPR_NETDB_H__
#define __HPR_NETDB_H__

#include "HPR_Config.h"
#include "HPR_Types.h"
#include "HPR_Addr.h"

// #if defined(OS_WINDOWS)
// #include "iptypes.h" 

// #define HPR_MAX_ADAPTER_NAME_LEN MAX_ADAPTER_NAME_LENGTH
// #define HPR_MAX_ADAPTER_ADDRESS_LEN MAX_ADAPTER_ADDRESS_LENGTH
// #define HPR_MAX_DESCRIPTION_LEN MAX_ADAPTER_DESCRIPTION_LENGTH
// #define HPR_MAX_IP_ADDRESS_LEN	16
// #define HPR_MAX_NIC_NUM			16
// #define HPR_DEF_ADAPTER_ADDRESS_LEN 6
// #elif defined (OS_POSIX)
#define HPR_MAX_ADAPTER_NAME_LEN 256
#define HPR_MAX_ADAPTER_ADDRESS_LEN 8
#define HPR_MAX_DESCRIPTION_LEN 128
#define HPR_MAX_IP_ADDRESS_LEN	16
#define HPR_MAX_NIC_NUM			16
#define HPR_DEF_ADAPTER_ADDRESS_LEN 6
/*#endif*/

/**
 *	IP地址结构
 */
struct HPR_IPADDR_STRING
{
	char szIPAddr[HPR_MAX_IP_ADDRESS_LEN];	//IP地址
	char szNetMask[HPR_MAX_IP_ADDRESS_LEN];	//掩码
};

/**
 * IP地址列表
 */
struct HPR_IPADDRESS
{
	HPR_IPADDRESS* pNext; //向前指针
	HPR_IPADDR_STRING struIpAddrString;//IP地址信息
};

/******************************************************************************
 * windows 网卡类型 MIB_IF_TYPE_OTHER/ETHERNET/TOKEN_RING/FDDI/PPP/LOOPBACK/SLIP
 * 与windows 的获取网卡信息相比，少了两个东西：
 * 1是WINS Server(Windwos) （windows 网络地址转换）
 * 2是DHCP获得时间及到期时间
 ******************************************************************************/
struct HPR_ADAPTER_INFO
{
	HPR_ADAPTER_INFO* pNext;						//向前指针
	HPR_INT32 iIndex;								//网卡索引号（从0开始）
	char szAdapterName[HPR_MAX_ADAPTER_NAME_LEN+4];	//网卡名称
	char szDescription[HPR_MAX_DESCRIPTION_LEN+4];	//网卡描述
	HPR_UINT32 nMacAddrLen;							//网卡地址长度
	unsigned char szMacAddr[HPR_MAX_ADAPTER_ADDRESS_LEN];	//网卡地址
	HPR_UINT32 nType;								//网卡类型 (POSIX无效)
	HPR_UINT32 nDhcpEnabled;						//是否开启DHCP(POSIX无效)
	HPR_IPADDRESS* pIpAddrList;						//IPv4地址列表
	HPR_IPADDRESS* pGatawayList;					//网关地址列表(POSIX无效)
	HPR_IPADDRESS* pDhcpServerList;					//DHCP服务器列表(POSIX无效)
	HPR_IPADDRESS* pIpAddrv6List;					//IPv6地址列表
};

struct HPR_ADDRESS_INFO
{
	HPR_INT32              iFlags;
	HPR_INT32              iFamily;
	HPR_INT32              iSockType;
	HPR_INT32              iProtocol;
	HPR_ADDR_T             Address;
	char                   *sCanonName;
	HPR_ADDRESS_INFO       *pNext;
};


HPR_DECLARE HPR_INT32 CALLBACK HPR_GetHostName(char* pName, HPR_INT32 iNameLen);

#if 1
HPR_DECLARE HPR_INT32 CALLBACK HPR_GetMacAddr(const char* pLocalIP, unsigned char* pMacAddr, HPR_INT32* iMacAddrlen);
HPR_DECLARE HPR_INT32 CALLBACK HPR_GetAdapterInfo(HPR_ADAPTER_INFO** pAdaterInfo);
HPR_DECLARE HPR_INT32 CALLBACK HPR_FreeAdapterInfo(HPR_ADAPTER_INFO* pAdaterInfo);
#endif

HPR_DECLARE hostent* CALLBACK HPR_GetAddressByName(const char* pName);

HPR_DECLARE HPR_UINT16 CALLBACK HPR_Htons(HPR_UINT16 nHostShort);

HPR_DECLARE HPR_UINT16 CALLBACK HPR_Ntohs(HPR_UINT16 nNetShort);

HPR_DECLARE HPR_UINT32 CALLBACK HPR_Htonl(HPR_UINT32 nHostLong);

HPR_DECLARE HPR_UINT32 CALLBACK HPR_Ntohl(HPR_UINT32 nNetLong);

//用于域名解析
HPR_DECLARE HPR_INT32 CALLBACK HPR_GetAddressInfo(const char *sNodeName, const char *sServerName, HPR_ADDRESS_INFO *Hints, HPR_ADDRESS_INFO **pRes);
HPR_DECLARE HPR_INT32 CALLBACK HPR_FreeAddressInfo(HPR_ADDRESS_INFO *pAddressInfo);

#endif
