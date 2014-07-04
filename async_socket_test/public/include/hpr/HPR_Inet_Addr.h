#ifndef __HPR_INET_ADDR_H__
#define __HPR_INET_ADDR_H__

#include "HPR_Config.h"
#include "HPR_Types.h"
#include "HPR_Addr.h"

class HPR_DECLARE_CLASS HPR_Inet_Addr
{
public:
	HPR_Inet_Addr();
	HPR_Inet_Addr(const HPR_Inet_Addr&);
	HPR_Inet_Addr(const char host[], HPR_UINT16 port, int family);
	HPR_Inet_Addr(const char address[]);	
	HPR_Inet_Addr(HPR_UINT16 port, HPR_UINT32 ipaddr=INADDR_ANY);
	HPR_Inet_Addr(const char host[], const char port[], const char* proto = "TCP");
	HPR_Inet_Addr(HPR_UINT32 ipaddr, const char port[], const char* proto = "TCP"); 

	~HPR_Inet_Addr();

public:
	int Set(const HPR_Inet_Addr&);
	int Set(const char host[], HPR_UINT16 port, int family);
	int Set(const char address[]);
	int Set(HPR_UINT16 port, HPR_UINT32 ipaddr=INADDR_ANY);
	int Set(const char host[], const char port[], const char* proto = "TCP");
	int Set(HPR_UINT32 ipaddr, const char port[], const char* proto = "TCP");

	void* GetAddr() const;
	int GetAddrSize() const;

	int AddrToString(char host[], int size, int format=1) const;
	int StringToAddr(char address[]);

	int SetPortNumber(HPR_UINT16 port, int encode=1);
	int SetAddress(char* ipaddr, int len, int encode=1);

	HPR_UINT16 GetPortNumber()const;
	int GetHostName(char hostname[], int hostnamelen) const;
	int GetIpAddress()const;
	char* GetHostAddr();
	HPR_BOOL IsAny()const;
	HPR_BOOL IsLoopback()const;

	int GetFamily();

#if defined (OS_SUPPORT_IPV6)
	HPR_BOOL IsIPV4MappedIPV6()const;
	HPR_BOOL IsIPv4CompatIPV6()const;
	HPR_BOOL IsLinkLocal()const;
#endif

protected:
	int Reset();

private:
	struct HPR_ADDR_T m_struAddr;
};

#endif