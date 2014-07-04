/*
 * CopyRight:	HangZhou Hikvision System Technology Co., Ltd. All	Right Reserved.
 * FileName:	HRP_AsyncIO.h
 * Desc:		OS Layer Component.
 * Author:		guanguochen
 * Date:		2009-03-30
 * Contact:		guanguochen@hikvision.com.cn
 * History:		Created By guanguochen 2009-03-30
 * */

/*! 
	\file HPR_AsyncIO.h
	\brief HPR Network Address Manager.

	Details.
*/

#ifndef __HPR_ASYNCIO_H__	
#define __HPR_ASYNCIO_H__

#include "HPR_Config.h"
#include "HPR_Types.h"
#include "HPR_Addr.h"

#if defined(OS_WINDOWS)
	#define HPR_INVALID_ASYNCIOQUEUE NULL
	#define HPR_INVALID_ASYNCIO NULL
#elif defined(OS_POSIX)
	#define HPR_INVALID_ASYNCIOQUEUE (HPR_VOIDPTR)-1
	#define HPR_INVALID_ASYNCIO (HPR_VOIDPTR)-1
#else
	#error OS Not Implement Yet.
#endif

/**
 * HPR_AsyncIO_CreateQueue create async io queue.
 * @param void
 * @return NULL fail, else success.
 * @sa HPR_AsyncIO_DestroyQueue()
 */
HPR_DECLARE HPR_HANDLE CALLBACK HPR_AsyncIO_CreateQueue(void);
HPR_DECLARE HPR_HANDLE CALLBACK HPR_AsyncIO_CreateQueueEx(int iThreadNum);

/**
 * HPR_AsyncIO_DestroyQueue destroy async io queue.
 * @param IOQueueHandle io queue handle, created by  HPR_AsyncIO_CreateQueue.
 * @return 0 success, -1 fail.
 * @sa HPR_AsyncIO_CreateQueue()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_AsyncIO_DestroyQueue(HPR_HANDLE hIOCP);

/**
 * HPR_AsyncIO_BindIOHandleToQueue bind io handle to async io queue.
 * @param IOHandle io handle
 * @param IOQueueHandle io queue handle, created by  HPR_AsyncIO_CreateQueue.
 * @return 0 success, -1 fail.
 * @sa HPR_AsyncIO_BindCallBackToIOHandle()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_AsyncIO_BindIOHandleToQueue(HPR_HANDLE hIOFd, HPR_HANDLE hIOCP);
HPR_DECLARE HPR_INT32 CALLBACK HPR_AsyncIO_UnBindIOHandle(HPR_HANDLE hIOFd, HPR_HANDLE hIOCP);

/**
 * HPR_AsyncIO_BindCallBackToIOHandle bind callback function to io handle.
 * @param IOHandle io handle.
 * @param CallBackFunc io completed callback.
 * @return 0 success, -1 fail.
 * @sa HPR_AsyncIO_BindIOHandleToQueue()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_AsyncIO_BindCallBackToIOHandle(HPR_HANDLE hIOFd, HPR_VOID (*CallBackFunc)(HPR_ULONG nErrorCode, HPR_ULONG nNumberOfBytes, HPR_VOIDPTR pUsrData));

HPR_DECLARE HPR_INT32 CALLBACK HPR_AsyncIO_PostQueuedCompleteStatus(HPR_HANDLE hIOCP, HPR_HANDLE hIOFd,HPR_INT32 iErrorCode,HPR_UINT32 nNumberOfBytesTransfered, HPR_VOIDPTR pUsrData);

/**
 * HPR_AsyncIO_Send send data by io handle use tcp.
 * @param IOHandle io handle 
 * @param pBuffer data pointer
 * @param BytesToSend data length want to send
 * @param pUsrData user defined data
 * @return 0 success, -1 fail
 * @sa HPR_AsyncIO_Recv()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_AsyncIO_Send(HPR_HANDLE hIOFd, HPR_VOIDPTR pBuffer, HPR_ULONG BytesToSend, HPR_VOIDPTR pUsrData);

/**
 * HPR_AsyncIO_Recv recv data by io handle use tcp
 * @param IOHandle io handle
 * @param pBuffer data pointer
 * @param BufferLen data length want to recv
 * @param pUsrData user defined data
 * @return 0 success,-1 fail.
 * @sa HPR_AsyncIO_Send()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_AsyncIO_Recv(HPR_HANDLE hIOFd, HPR_VOIDPTR pBuffer, HPR_ULONG nBufferLen, HPR_VOIDPTR pUsrData);
	
/**
 * HPR_AsyncIO_SendTo send data by io handle use udp.
 * @param IOHandle io handle
 * @param pBuffer data pointer
 * @param BytesToSend data length want to sendto
 * @param pUsrData user defined data
 * @param pToAddr remote address
 * @return
 * @sa HPR_AsyncIO_RecvFrom()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_AsyncIO_SendTo(HPR_HANDLE hIOFd, HPR_VOIDPTR pBuffer, HPR_ULONG nBytesToSend, HPR_VOIDPTR pUsrData, HPR_ADDR_T* pDstAddr);

/**
 * HPR_AsyncIO_RecvFrom recv data by io handle by udp.
 * @param IOHandle io handle
 * @param pBuffer data pointer.
 * @param BufferLen data length want to recv
 * @param pUsrData user defined data
 * @param pFromAddr remote address
 * @return 0 success, -1 fail
 * @sa HPR_AsyncIO_SendTo()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_AsyncIO_RecvFrom(HPR_HANDLE hIOFd, HPR_VOIDPTR pBuffer, HPR_ULONG nBufferLen, HPR_VOIDPTR pUsrData, HPR_ADDR_T* pAddrSrc);

/**
 * HPR_AsyncIO_WriteFile write data to io handle
 * @param IOHandle io handle
 * @param pBuffer data to write
 * @param BytesToWrite data length want to write to handle.
 * @param pUsrData user defined data.
 * @return 0 success, -1 fail.
 * @sa HPR_AsyncIO_ReadFile()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_AsyncIO_WriteFile(HPR_HANDLE hIOFd, HPR_VOIDPTR pBuffer, HPR_ULONG nBytesToWrite, HPR_VOIDPTR pUsrData);

/**
 * HPR_AsyncIO_ReadFile read data from io handle
 * @param IOHandle io handle
 * @param pBuffer data pointer
 * @param BufferLen data len want to read.
 * @param pUsrData user defined data.
 * @return 0 success, -1 fail.
 * @sa HPR_AsyncIO_WriteFile()
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_AsyncIO_ReadFile(HPR_HANDLE hIOFd, HPR_VOIDPTR pBuffer, HPR_ULONG nBufferLen, HPR_VOIDPTR pUsrData);

typedef HPR_VOID (*IOCALLBACK)(HPR_ULONG nErrorCode, HPR_ULONG nNumberOfBytes, HPR_VOIDPTR pUsrData);

class HPR_AsyncIOQueue;

class HPR_AsyncIO
{
public:
	HPR_AsyncIO(HPR_HANDLE IOHandle);
	~HPR_AsyncIO(HPR_VOID);

	HPR_INT32 BindIOHandleToQueue(HPR_AsyncIOQueue *pIOQueue);
	HPR_INT32 BindCallBackToIOHandle(HPR_VOID (*CallBackFunc)(HPR_ULONG nErrorCode, HPR_ULONG NumberOfBytes, HPR_VOIDPTR pUsrData));
	HPR_INT32 IOSend(HPR_VOIDPTR pBuffer, HPR_ULONG BytesToSend, HPR_VOIDPTR pUsrData);
	HPR_INT32 IORecv(HPR_VOIDPTR pBuffer, HPR_ULONG BufferLen, HPR_VOIDPTR pUsrData);
	HPR_INT32 IOSendTo(HPR_VOIDPTR pBuffer, HPR_ULONG BytesToSend, HPR_VOIDPTR pUsrData, HPR_ADDR_T* pToAddr);
	HPR_INT32 IORecvFrom(HPR_VOIDPTR pBuffer, HPR_ULONG BufferLen, HPR_VOIDPTR pUsrData, HPR_ADDR_T* pFromAddr);
	HPR_INT32 IOWriteFile(HPR_VOIDPTR pBuffer, HPR_ULONG BytesToWrite, HPR_VOIDPTR pUsrData);
	HPR_INT32 IOReadFile(HPR_VOIDPTR pBuffer, HPR_ULONG BufferLen, HPR_VOIDPTR pUsrData);

private:
	HPR_HANDLE m_iohandle;
};

class HPR_AsyncIOQueue
{
public:
	HPR_AsyncIOQueue(HPR_VOID);
	~HPR_AsyncIOQueue(HPR_VOID);

	HPR_INT32 Initialize(HPR_VOID);
	HPR_INT32 Release(HPR_VOID);
	HPR_HANDLE GetHandle(HPR_VOID);

private:
	HPR_HANDLE m_queuehandle;
};

#endif
