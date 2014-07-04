#include "TCPHandler.h"

CTCPHandler::CTCPHandler(void)
{
}

CTCPHandler::~CTCPHandler(void)
{
}

/**	@fn	void CTCPHandler::OnConnect(HPR_UINT32 nsockid)
*	@brief 
*	@param[in] nsockid 
*	@return	
*/
void CTCPHandler::OnConnect( HPR_UINT32 nsockid )
{
	printf("connected successed.\n");
}

/**	@fn	void CTCPHandler::OnException(HPR_UINT32 nsockid, HPR_INT32 nErrorCode)
*	@brief 
*	@param[in] nsockid 
*	@param[in] nErrorCode 
*	@return	
*/
void CTCPHandler::OnException( HPR_UINT32 nsockid, HPR_INT32 nErrorCode )
{
	printf("Exception %d.\n", nErrorCode);
}

/**	@fn	void CTCPHandler::OnClose(HPR_UINT32 nsockid)
*	@brief 
*	@param[in] nsockid 
*	@return	
*/
void CTCPHandler::OnClose( HPR_UINT32 nsockid )
{
	printf("Close.\n");
}

/**	@fn	void CTCPHandler::OnRecv(HPR_UINT32 nsockid, const char* szBuf, HPR_INT32 nBufSize, const char* szIP, HPR_INT32 nPort)
*	@brief 
*	@param[in] nsockid 
*	@param[in] szBuf 
*	@param[in] nBufSize 
*	@param[in] szIP 
*	@param[in] nPort 
*	@return	
*/
void CTCPHandler::OnRecv( HPR_UINT32 nsockid, const char* szBuf, HPR_INT32 nBufSize, const char* szIP, HPR_INT32 nPort )
{
	printf("%s.\n", szBuf);
}
