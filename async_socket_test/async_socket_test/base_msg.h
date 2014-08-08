#ifndef _BASE_MSG_H
#define _BASE_MSG_H

#include "socket_io.h"
#include "base_type.h"
#include "buffer_loop.h"

class SOCKET_IO_DECLARE_CLASS CBaseMsg
{
public:
	CBaseMsg(void)
	{	m_nHeadLength = 0;
		m_nBodyLength = 0;
	}
	virtual ~CBaseMsg(void) {}

	uint32_t GetHeadLength() const { return m_nHeadLength; }
	uint32_t GetBodyLength() const { return m_nBodyLength; }
	uint32_t GetDataLength() const { return m_nHeadLength + m_nBodyLength; }

	char* header() { return m_szData; }
	char* body() { return m_szData + GetHeadLength(); }
	char* data() { return m_szData; }
	const char* body() const { return m_szData + GetHeadLength(); }
	const char* data() const { return m_szData; }
 
	virtual BOOL DecodeHeader(const char*szData, uint32_t nDataSize) = 0;
	//virtual void EncodeMsg(uint32_t nSessionSeq, uint32_t nCmdID, const char* szMsg, uint32_t nMsgSize) {};
private:
	void __SetHeadLength(uint32_t nHeadLength ) { m_nHeadLength = nHeadLength; }
	void __SetBodyLength(uint32_t nBodyLength) { m_nBodyLength = nBodyLength; }
	uint32_t m_nHeadLength;
	uint32_t m_nBodyLength;

    CBufferLoop m_databuffer;
};

#endif