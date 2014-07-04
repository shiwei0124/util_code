#pragma once
#include "HPR_Hpr.h"

#define MSG_HEAD_MAX_LENGTH		32
#define MSG_BODY_MAX_LENGTH		RECV_MAX_BUF_SIZE - MSG_HEAD_MAX_LENGTH


class CSessionMsg
{
public:
	CSessionMsg(void) 
	{	m_nHeadLength = 0;
		m_nBodyLength = 0;
		memset(m_szData, 0, sizeof(m_szData));
	}
	virtual ~CSessionMsg(void) {}

	HPR_INT32 GetHeadLength() const { return m_nHeadLength; }

	HPR_INT32 GetBodyLength() const { return m_nBodyLength; }

	HPR_INT32 GetDataLength() const { return m_nHeadLength + m_nBodyLength; }
	HPR_INT32 GetCmdID() const { return m_nCmdID; }
	HPR_INT32 GetSessionReq() const {return m_nSessionReq; }

	char* header() { return m_szData; }
	char* body() { return m_szData + GetHeadLength(); }
	char* data() { return m_szData; }
	const char* body() const { return m_szData + GetHeadLength(); }
	const char* data() const { return m_szData; }
 
private:
	void __SetHeadLength(HPR_INT32 nHeadLength ) { m_nHeadLength = nHeadLength; }
	void __SetBodyLength(HPR_INT32 nBodyLength) { m_nBodyLength = nBodyLength; }
	void __SetCmdID(HPR_INT32 nCmdID) { m_nCmdID = nCmdID; }
	void __SetSessionReq(HPR_INT32 nSessionReq) { m_nSessionReq = nSessionReq; }
	HPR_INT32 m_nHeadLength; 
	HPR_INT32 m_nBodyLength;

	char m_szData[MSG_HEAD_MAX_LENGTH + MSG_BODY_MAX_LENGTH];

	HPR_INT32 m_nCmdID;					//消息ID
	HPR_INT32 m_nSessionReq;			//消息会话ID
	HPR_INT32 m_nRspResutl;				//响应的结果
};

