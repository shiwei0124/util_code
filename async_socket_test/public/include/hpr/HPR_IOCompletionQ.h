/*
 * ====================================================================
 * Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
 *
 *        Filename:  IOCompletionQ.h
 *
 *     Description:  simulate the CompletionPort
 *
 *          Author:  tyu, yutao NOSPAM hikvision DOT com
 *
 *            Date:  2009-10-19 16:43:43 中国标准时间
 *
 * Modification History:
 *            <version > <time> <author> <desc>
 * ====================================================================
 */
#ifndef __IO_COMPLETIONQ_H__
#define __IO_COMPLETIONQ_H__

#include "HPR_Config.h"
#include "HPR_Types.h"

#define COMPLETIONQ_MAX_HANDLE 1500

// ----------------------------------------------------------------------------
// This LPOVERLAPPED typedef is from WINBASE.H
typedef struct _OVERLAPPED 
{
    HPR_UINT32  Internal;
    HPR_UINT32  InternalHigh;
    HPR_UINT32  Offset;
    HPR_UINT32  OffsetHigh;
    HPR_HANDLE  hEvent;
} OVERLAPPED, *LPOVERLAPPED;

// ----------------------------------------------------------------------------

HPR_DECLARE HPR_HANDLE HPR_AIOCreateIoCompletionPort(HPR_HANDLE file_handle, HPR_HANDLE cq, HPR_UINT32 completion_key, HPR_UINT32 num_threads);
HPR_DECLARE HPR_BOOL   HPR_AIOCloseIoCompletionPort(HPR_HANDLE cq);

HPR_DECLARE HPR_BOOL   HPR_AIOGetQueuedCompletionStatus(HPR_HANDLE cq, HPR_UINT32* bytes_transferred, HPR_UINT32* completion_key,
                                            LPOVERLAPPED * lpOverlapped, HPR_UINT32 tmout);

HPR_DECLARE HPR_BOOL   HPR_AIOPostQueuedCompletionStatus(HPR_HANDLE cq, HPR_UINT32 dwNumberOfBytesTransferred,
                                            HPR_UINT32 dwCompletionKey, LPOVERLAPPED lpOverlapped);

HPR_DECLARE HPR_HANDLE HPR_AIOOpenFile(const char* pFileName, HPR_UINT32 nFlag, HPR_UINT32 nFileAttr);
HPR_DECLARE HPR_BOOL   HPR_AIOCloseFile(HPR_HANDLE file_handle);
HPR_DECLARE HPR_BOOL   HPR_AIOCancelIO(HPR_HANDLE file_handle);

HPR_DECLARE HPR_BOOL   HPR_AIOReadFile(HPR_HANDLE file_handle, HPR_VOIDPTR buffer, HPR_UINT32 bytes_to_read, HPR_UINT32* bytes_read, 
                                       LPOVERLAPPED lpOverlapped);
HPR_DECLARE HPR_BOOL   HPR_AIOWriteFile(HPR_HANDLE file_handle, HPR_VOIDPTR buffer, HPR_UINT32 bytes_to_write, HPR_UINT32* bytes_written, 
                                       LPOVERLAPPED lpOverlapped);


#endif

