#pragma once
#include "socket_io.h"
#include "HPR_Hpr.h"

#define MSG_HEAD_MAX_LENGTH		32
#define MSG_BODY_MAX_LENGTH		1024 * 10 - MSG_HEAD_MAX_LENGTH


class SOCKET_IO_DECLARE_CLASS CSessionMsg
{
public:
	CSessionMsg(void) 
	{	m_nHeadLength = 0;
		m_nBodyLength = 0;
		memset(m_szData, 0, sizeof(m_szData));
	}
	virtual ~CSessionMsg(void) {}

	int GetHeadLength() const { return m_nHeadLength; }

	int GetBodyLength() const { return m_nBodyLength; }

	int GetDataLength() const { return m_nHeadLength + m_nBodyLength; }
	int GetCmdID() const { return m_nCmdID; }
	int GetSessionReq() const {return m_nSessionReq; }

	char* header() { return m_szData; }
	char* body() { return m_szData + GetHeadLength(); }
	char* data() { return m_szData; }
	const char* body() const { return m_szData + GetHeadLength(); }
	const char* data() const { return m_szData; }
 
	virtual HPR_BOOL DecodeHeader() = 0;
	virtual void EncodeMsg(HPR_INT32 nSessionSeq, HPR_INT32 nCmdID, const char* szMsg, HPR_INT32 nMsgSize) {};
private:
	void __SetHeadLength(HPR_INT32 nHeadLength ) { m_nHeadLength = nHeadLength; }
	void __SetBodyLength(HPR_INT32 nBodyLength) { m_nBodyLength = nBodyLength; }
	void __SetCmdID(HPR_INT32 nCmdID) { m_nCmdID = nCmdID; }
	void __SetSessionReq(HPR_INT32 nSessionReq) { m_nSessionReq = nSessionReq; }
	int m_nHeadLength;
	int m_nBodyLength;

	char m_szData[MSG_HEAD_MAX_LENGTH + MSG_BODY_MAX_LENGTH];

	int m_nCmdID;					//娑����ID
	int m_nSessionReq;				//娑����浼�璇�ID
	int m_nRspResutl;				//���搴����缁����
};

