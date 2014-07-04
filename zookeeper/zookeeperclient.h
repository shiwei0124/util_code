#ifndef _ZOOKEEPER_CLIENT_H
#define _ZOOKEEPER_CLIENT_H
#include "zookeeper/zookeeper.h"
#include "vector"
#include "string"
#include "hpr/HPR_Event.h"
#include "hpr/HPR_Hpr.h"
using namespace std;

#define ZOOKEEPER_TIMEOUT		10 * 1000

typedef struct tag_ServerInfo
{
	char szIP[32];
	HPR_INT32 nPort;
}ST_SERVERINFO, *LPST_SERVERINFO;

class CZooClient
{
public:
	CZooClient();
	virtual ~CZooClient();

	static void WatcherFn(zhandle_t *zh, int type, 
		int state, const char *path,void *watcherCtx);

	HPR_BOOL Init();
	void Fini();
	HPR_BOOL Connect();
	HPR_BOOL AsyncConnect();
	HPR_BOOL DisConnect();
	HPR_BOOL GetServerInfo(char* szIP, int* nPort);

	HPR_INT64 GetClientID() { return m_zclientid.client_id; }
	void SetClientID_T(clientid_t clientid) { m_zclientid = clientid; }
	const char* GetKey() const { return m_szKey; }
	void Setkey(const char* szkey) { strcpy(m_szKey, szkey); } 

	
	void WaitConnectEvent(HPR_INT32 nTime) { HPR_WaitForSingleObject(m_zconnectevent, nTime); }
	void WakeConnectEvent() { HPR_SetEvent(m_zconnectevent); }
	void ResetConnectEvent() { HPR_ResetEvent(m_zconnectevent); }
private:
	string _BuildHost();
	HPR_BOOL _GetServerList(vector<ST_SERVERINFO>& vServerInfo);
	HPR_INT32 _HashCode(const char* szValue, HPR_INT32 nLength) const;
private:
	char m_szKey[128];
	zhandle_t* m_zhandle;
	clientid_t m_zclientid;
	HPR_HANDLE m_zconnectevent;

};


#endif