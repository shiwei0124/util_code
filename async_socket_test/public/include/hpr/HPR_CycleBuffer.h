/*
 * ====================================================================
 * Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
 *
 *        Filename:  HPR_CycleBuffer.h
 * 
 *     Description:  循环缓冲区接口定义
 * 
 *          Author:  tyu, yutao NOSPAM hikvision DOT com
 *
 *            Date:  2009-11-26 19:45:03 中国标准时间
 *
 * Modification History:
 *            <version > <time> <author> <desc>
 * ====================================================================
 */

#ifndef __HPR_CYCLEBUFFER_H__
#define __HPR_CYCLEBUFFER_H__

#include "HPR_Config.h"
#include "HPR_Types.h"

typedef void (*CycleStreamCallBack)(HPR_UINT8* pBuf, HPR_INT32 len, HPR_VOIDPTR userData);

/**
 * Function:	HPR_CycleBufferInit
 * Desc:		
 * Input:		@param 
				@param 
 * Output:      @param
 * Return:		if success return HPR_OK, otherwise return HPR_ERROR.
 * */
HPR_DECLARE HPR_INT32 CALLBACK HPR_CycleBufferInit(HPR_HANDLE* pCycleBuf, HPR_INT32 bufSize, CycleStreamCallBack cb, HPR_VOIDPTR userData);

/**
 * Function:	HPR_CycleBufferWrite
 * Desc:		
 * Input:		@param 
				@param 
				@param 
 * Output
 * Return:		if success return HPR_OK, otherwise return HPR_ERROR.
 * */
HPR_DECLARE HPR_INT32 CALLBACK HPR_CycleBufferWrite(HPR_HANDLE cycleBuf, unsigned char* pBuf, HPR_INT32 len);

/**
 * Function:	HPR_CycleBufferRead
 * Desc:		
 * Input:		@param 
 * Output:      
 * Return:		if success return HPR_OK, otherwise return HPR_ERROR.
 * */
HPR_DECLARE HPR_INT32 CALLBACK HPR_CycleBufferRead(HPR_HANDLE cycleBuf);

/**
 * Function:	HPR_CycleBufferValue
 * Desc:		
 * Input:		@param 
 * Output:      @param
 * Return:		if success return HPR_OK, otherwise return HPR_ERROR.
 * */
HPR_DECLARE HPR_INT32 CALLBACK HPR_CycleBufferValue(HPR_HANDLE cycleBuf, HPR_INT32* len);

/**
 * Function:	HPR_CycleBufferDestroy
 * Desc:		
 * Input:		@param 
 * Output:
 * Return:		if success return HPR_OK, otherwise return HPR_ERROR.
 * */
HPR_DECLARE HPR_INT32 CALLBACK HPR_CycleBufferDestroy(HPR_HANDLE cycleBuf);

#endif


