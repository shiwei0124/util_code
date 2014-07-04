/*
 * CopyRight:	HangZhou Hikvision System Technology Co., Ltd. All	Right Reserved.
 * FileName:	HRP_Guard.h
 * Desc:		OS Layer Component.
 * Author:		schina
 * Date:		2009-03-02
 * Contact:		zhaoyiji@hikvision.com.cn
 * History:		Created By Zhaoyiji 2009-03-02
 * */

#ifndef __HPR_GUARD_H__
#define __HPR_GUARD_H__

#include "HPR_Mutex.h"

//////////////////////////////////////////////////////////////////////////
//c++ ±àÒëÆ÷²ÅÖ§³Ö
#if defined (CXXCOMPILE)
class HPR_DECLARE_CLASS HPR_Guard
{
public:
	HPR_Guard(HPR_Mutex* pMutex)
	:m_pMutex(pMutex)
	{
		pMutex->Lock();	
	}
	
	~HPR_Guard()
	{
		Release();
	}

	void Release()
	{
		if (m_pMutex)
		{
			m_pMutex->Unlock();
			m_pMutex = NULL;
		}
	}

private:
	HPR_Mutex* m_pMutex;
};
#endif
//////////////////////////////////////////////////////////////////////////

#endif

