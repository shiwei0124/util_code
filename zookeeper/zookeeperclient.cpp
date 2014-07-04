#include "zookeeperclient.h"
#include "monitorclient_define.h"
#include "configer.h"
#include "json/json.h"
#include <map>
#include <string>


/**	@fn	 CZooClient::CZooClient()
*	@brief 
*	@return	
*/
CZooClient::CZooClient()
{
	m_szKey[0] = '\0';
	m_zhandle = NULL;
	memset(&m_zclientid, 0, sizeof(clientid_t));
	m_zconnectevent = HPR_CreateEvent(HPR_FALSE);

}

/**	@fn	 CZooClient::~CZooClient()
*	@brief 
*	@return	
*/
CZooClient::~CZooClient()
{
	HPR_CloseEvent(m_zconnectevent);
}

/**	@fn	void CZooClient::WatcherFn(zhandle_t *zh, int type, int state, const char *path,void *watcherCtx)
*	@brief 
*	@param[in] zh 
*	@param[in] type 
*	@param[in] state 
*	@param[in] path 
*	@param[in] watcherCtx 
*	@return	
*/
void CZooClient::WatcherFn( zhandle_t *zh, int type, int state, const char *path,void *watcherCtx )
{
	CZooClient* p=(CZooClient*)watcherCtx;

	if (type == ZOO_SESSION_EVENT) 
	{
		if(state==ZOO_CONNECTED_STATE)
		{
			const clientid_t *id = zoo_client_id(zh);
			if (p->GetClientID() == 0 || p->GetClientID() != id->client_id) 
			{
				p->SetClientID_T(*id);
				MONITOR_CLIENT_WARN(0, "MONITOR CLIENT Warn", 0, 0, 
					"%s, zookeeper session state is connected.", __FUNCTION__);
			}
			else
			{
				MONITOR_CLIENT_WARN(0, "MONITOR CLIENT Warn", 0, 0, 
					"%s, zookeeper session state connected, but not assignment.", __FUNCTION__);
			}
		}
		if (state == ZOO_EXPIRED_SESSION_STATE)
		{
			MONITOR_CLIENT_WARN(0, "MONITOR CLIENT Warn", 0, 0, 
				"%s, zookeeper session state is expired.", __FUNCTION__);
			p->DisConnect();
			p->AsyncConnect();
		}
		//不管是链接成功还是失败，都进行唤醒操作。
		p->WakeConnectEvent();
		p->ResetConnectEvent();
	} 
}

/**	@fn	HPR_BOOL CZooClient::Init() 
*	@brief 
*	@return	
*/
HPR_BOOL CZooClient::Init()
{
	HPR_BOOL bRet = HPR_TRUE;
	//if (LoadConfig() != HPR_TRUE)
	//{
	//	bRet = HPR_FALSE;
	//	MONITOR_CLIENT_ERROR(0, "MONITOR CLIENT Error", 0, 0,
	//		"%s, load config failed.", __FUNCTION__);
	//}
	return bRet;
}

/**	@fn	void CZooClient::Fini()
*	@brief 
*	@return	
*/
void CZooClient::Fini()
{
	DisConnect();
}

/**	@fn	std::string CZooClient::_BuildHost()
*	@brief 构造一个host集合的字符串
*	@return	
*/
std::string CZooClient::_BuildHost()
{
	string strHost = "";
	vector<string>& vzoohost = CConfiger::GetInstance()->GetZooHost();
	for (HPR_INT32 i = 0; i < (HPR_INT32)vzoohost.size(); i++)
	{
		string strTmp = vzoohost[i];
		if ( i != 0)
		{
			strHost.append(",");
		}
		strHost.append(strTmp);
	}
	return strHost;
}

/**	@fn	HPR_BOOL CZooClient::Connect()
*	@brief 
*	@return	
*/
HPR_BOOL CZooClient::Connect()
{
	HPR_BOOL bRet = HPR_FALSE;
	string strHost = _BuildHost();
	if (strHost.length() == 0)
	{
		MONITOR_CLIENT_ERROR(0, "MONITOR CLIENT Error", 0, 0,
			"%s, zookeeper_init failed, no host info.", __FUNCTION__);
		return bRet;
	}
	m_zhandle = zookeeper_init(strHost.c_str(), WatcherFn, ZOOKEEPER_TIMEOUT, &m_zclientid, this, 0);
	if(!m_zhandle)
	{
		MONITOR_CLIENT_ERROR(0, "MONITOR CLIENT Error", 0, 0,
			"%s, zookeeper_init failed.", __FUNCTION__);
		return bRet;
	}

	WaitConnectEvent(30 * 1000);
	if (m_zclientid.client_id == 0)
	{
		MONITOR_CLIENT_ERROR(0, "MONITOR CLIENT Error", 0, 0,
			"%s, zookeeper connect failed, client_id is 0.", __FUNCTION__);
		return bRet;
	}
	else
	{
		MONITOR_CLIENT_TRACE(0, "MONITOR CLIENT Trace", 0, 0,
			"%s, zookeeper connect successed.", __FUNCTION__);
	}
	bRet = HPR_TRUE;
	return bRet;
}

/**	@fn	HPR_BOOL CZooClient::AsyncConnect()
*	@brief 
*	@return	
*/
HPR_BOOL CZooClient::AsyncConnect()
{
	HPR_BOOL bRet = HPR_FALSE;
	string strHost = _BuildHost();
	m_zhandle = zookeeper_init(strHost.c_str(), WatcherFn, ZOOKEEPER_TIMEOUT, &m_zclientid, this, 0);
	if(!m_zhandle)
	{
		MONITOR_CLIENT_ERROR(0, "MONITOR CLIENT Error", 0, 0,
			"%s, zookeeper_init failed.", __FUNCTION__);
		return bRet;
	}
	bRet = HPR_TRUE;
	return bRet;
}

/**	@fn	HPR_BOOL CZooClient::GetServerInfo(char* szIP, int* nPort)
*	@brief 
*	@param[out] szIP 
*	@param[out] nPort 
*	@return	
*/
HPR_BOOL CZooClient::GetServerInfo( char* szIP, int* nPort )
{
	HPR_BOOL bRet = HPR_FALSE;
	szIP[0] = '\0';
	*nPort = 0;
	vector<ST_SERVERINFO> vServerList;
	bRet = _GetServerList(vServerList);
	if (bRet == HPR_TRUE)
	{
		HPR_INT32 nHashCode = _HashCode(GetKey(), strlen(GetKey()));
		HPR_INT32 nIndex = abs(nHashCode % vServerList.size());
		ST_SERVERINFO stServer = vServerList[nIndex];
		strcpy(szIP, stServer.szIP);
		*nPort = stServer.nPort;
		bRet = HPR_TRUE;
	}
	return bRet;
}

/**	@fn	HPR_BOOL CZooClient::DisConnect()
*	@brief 
*	@return	
*/
HPR_BOOL CZooClient::DisConnect()
{
	if(m_zhandle)
	{
		zookeeper_close(m_zhandle);
		m_zhandle = NULL;
		memset(&m_zclientid, 0, sizeof(clientid_t));
	}
	return HPR_TRUE;
}

/**	@fn	HPR_INT32 CZooClient::_HashCode(const char* szValue, HPR_INT32 nLength)
*	@brief 
*	@param[in] szValue 
*	@param[in] nLength 
*	@return	
*/
HPR_INT32 CZooClient::_HashCode( const char* szValue, HPR_INT32 nLength ) const
{
	int h = 0;
	if (h == 0 && nLength > 0) 
	{
		const char* szVal = szValue;
		for (int i = 0; i < nLength; i++) 
		{
			h = 31 * h + szVal[i];
		}
	}
	return h;
}

/**	@fn	HPR_BOOL CZooClient::_GetServerList(vector<ST_SERVERINFO>& vServerInfo)
*	@brief 
*	@param[in] vServerInfo 
*	@return	
*/
HPR_BOOL CZooClient::_GetServerList( vector<ST_SERVERINFO>& vServerInfo )
{
	HPR_BOOL bRet = HPR_FALSE;
	if (m_zclientid.client_id == 0)
	{
		MONITOR_CLIENT_ERROR(0, "MONITOR CLIENT Error", 0, 0,
			"%s, zookeeper client id is 0.", __FUNCTION__);
		return bRet;
	}

	char node_path[512] = {0};
	strcpy(node_path, CConfiger::GetInstance()->GetPath());
	int r=zoo_exists(m_zhandle,node_path,0,NULL);
	if(r!=ZOK)
	{
		MONITOR_CLIENT_ERROR(0, "MONITOR CLIENT Error", 0, 0,
			"%s, zookeeper zoo_exists failed, error code: %d.", __FUNCTION__, r);
		return bRet;
	}

	String_vector sv;
	r=zoo_get_children(m_zhandle,node_path,0,&sv);
	if(r!=ZOK)
	{
		MONITOR_CLIENT_ERROR(0, "MONITOR CLIENT Error", 0, 0,
			"%s, zookeeper zoo_get_children failed, error code: %d.", __FUNCTION__, r);
		return bRet;
	}

	map<string, ST_SERVERINFO> mapServerInfo;
	string path=node_path;
	path+="/";
	for (int i=0;i<sv.count;i++)
	{
		char buf[1024]={0};
		int len=1023;
		string tmp = path + sv.data[i];
		r = zoo_get(m_zhandle,tmp.c_str(),0,buf,&len,NULL);
		if(r != ZOK)
			continue;

		Json::Reader rd;
		Json::Value v;
		if(!rd.parse(buf,v,false))
		{
			MONITOR_CLIENT_ERROR(0, "MONITOR CLIENT Error", 0, 0,
				"%s, zookeeper parse value failed.", __FUNCTION__);
			return bRet;
		}

		ST_SERVERINFO si={0};
		v["ipAddress"].asString().copy(si.szIP,31);
		si.nPort=v["port"].asUInt();
		char szKey[128] = {0};
		sprintf(szKey, "%s:%d", si.szIP, si.nPort);
		mapServerInfo[string(szKey)] = si;
	}

	map<string, ST_SERVERINFO>::iterator it = mapServerInfo.begin();
	for (; it != mapServerInfo.end(); it++)
	{
		vServerInfo.push_back(it->second);
	}
	
	if (vServerInfo.size() != 0)
	{
		bRet = HPR_TRUE;
	}
	else
	{
		MONITOR_CLIENT_ERROR(0, "MONITOR CLIENT Error", 0, 0,
			"%s, zookeeper get server info failed, count is 0.", __FUNCTION__);
	}
	
	return bRet;
}






