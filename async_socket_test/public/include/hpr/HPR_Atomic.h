/*
 * CopyRight:	HangZhou Hikvision System Technology Co., Ltd. All	Right Reserved.
 * FileName:	HPR_Atomic.h
 * Desc:		define CC Types files.
 * Author:		schina
 * Date:		2009-03-02
 * Contact:		zhaoyiji@hikvision.com.cn
 * History:		Created By Zhaoyiji 2009-03-02
 * */

/*! 
	\file HPR_Atomic.h
	\brief HPR Atomic Manager.

	Details.
*/

#ifndef __HPR_ATOMIC_H__
#define __HPR_ATOMIC_H__

#include "HPR_Config.h"
#include "HPR_Types.h"

#if defined(OS_WINDOWS)
	typedef LONG HPR_ATOMIC_T;
#elif defined(OS_POSIX)
	typedef HPR_UINT32 HPR_ATOMIC_T;
#else
	#error OS Not Implement Yet.
#endif

/**
 * HPR_AtomicInc atomic increment, The function prevents more than one thread from using the same variable simultaneously.
 * @param pVal (IN OUT) pointer to the variable to be incremented. 
 * @return void
 * @sa HPR_AtomicDec().
 */
HPR_DECLARE HPR_VOID CALLBACK HPR_AtomicInc(HPR_ATOMIC_T* pVal);

/**
 * HPR_AtomicDec atomic decrement, The function prevents more than one thread from using the same variable simultaneously.
 * @param pVal (IN OUT) pointer to the variable to be decremented.
 * @return void
 * @sa HPR_AtomicInc().
 */
HPR_DECLARE HPR_VOID CALLBACK HPR_AtomicDec(HPR_ATOMIC_T* pVal);

/**
 * HPR_AtomicAdd atomic add, The function prevents more than one thread from using the same variable simultaneously.
 * @param pDst (OUT) Pointer to the value that is to have Value added to it. 
 * @param nVal (IN) Value to be added to the variable pointed to by the pDst param. 
 * @return The return value is the initial value of the variable pointed to by the pdst param.
 * @sa HPR_AtomicInc(), HPR_AtomicDec().
 */
HPR_DECLARE HPR_UINT32 CALLBACK HPR_AtomicAdd(HPR_ATOMIC_T* pDst,HPR_ATOMIC_T nVal);

/**
 * HPR_AtomicSet atmoic set, The function prevents more than one thread from using the same variable simultaneously.
 * @param pDst (OUT) Pointer to the value that is to have Value set to it.
 * @param nVal (IN) Value to be set to the variable pointed to by the pDst param.
 * @return The return value is the initial value of the variable pointed to by the pdst param.
 * @sa HPR_AtomicAdd().
 */
HPR_DECLARE HPR_UINT32 CALLBACK HPR_AtomicSet(HPR_ATOMIC_T* pDst,HPR_ATOMIC_T nVal);

/**
 * HPR_AtomicCas atomic compare and set, performs an atomic comparison of the specified values and exchanges the values, based on the outcome of the comparison,\
	The function prevents more than one thread from using the same variable simultaneously.
 * @param pDst (OUT) Pointer to the destination value. 
 * @param nWith (IN) Exchange value.
 * @param nCmp (IN) Value to compare to Destination
 * @return The return value is the initial value of the destination
 * @sa HPR_AtomicCasPtr().
 */
HPR_DECLARE HPR_UINT32 CALLBACK HPR_AtomicCas(HPR_ATOMIC_T* pDst,HPR_ATOMIC_T nWith,HPR_ATOMIC_T nCmp);

/**
 * HPR_AtomicCasPtr atomic compare and set pointer, performs an atomic comparison of the specified values parameters and exchanges the values, based on the outcome of the comparison,\
	The function prevents more than one thread from using the same variable simultaneously.
 * @param pdst (OUT) Pointer to a pointer to the destination value
 * @param pWith (IN) Exchange value
 * @param pCmp (IN) Value to compare to Destination
 * @return The return value is the initial value of the destination
 * @sa HPR_AtomicCas().
 */
HPR_DECLARE HPR_VOIDPTR CALLBACK HPR_AtomicCasPtr(HPR_VOIDPTR* pDst,HPR_VOIDPTR pWith,HPR_VOIDPTR pCmp);

#endif
