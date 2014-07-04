/*
 * CopyRight:	HangZhou Hikvision System Technology Co., Ltd. All	Right Reserved.
 * FileName:	HRP_Select.h
 * Desc:		OS Layer Component.
 * Author:		schina
 * Date:		2009-03-02
 * Contact:		zhaoyiji@hikvision.com.cn
 * History:		Created By Zhaoyiji 2009-03-02
 * */

/*! 
	\file HPR_Select.h
	\brief HPR Select Manager.

	Details.
*/

#ifndef __HPR_SELECT_H__
#define __HPR_SELECT_H__

#include "HPR_Config.h"
#include "HPR_Types.h"

#if defined (OS_POSIX)
#if defined(OS_S60)
#include <select.h>
#else
#include <poll.h>
#endif
#endif


HPR_DECLARE HPR_INT32 CALLBACK HPR_FdIsSet(HPR_INT32 fd, fd_set *set);

/**
 * HPR_Select select about windows and posix,功能上与windows的select一致
 * @param iMaxFd (IN) max fd join the select.
 * @param fdset_r (IN) the same meaning in select
 * @param fdset_w (IN) the same meaning in select
 * @param fdset_e (IN) the same meaning in select
 * @param tv (IN) timeout
 * return  the same meaning in select
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_Select(HPR_INT32 iMaxFd, fd_set* fdset_r, fd_set* fdset_w, fd_set* fdset_e, timeval* tv); 

/**
 * HPR_SelectEx select about windows and posix,功能上与linux的select一致
 * @param iMaxFd (IN) max fd join the select.
 * @param fdset_r (IN) the same meaning in select
 * @param fdset_w (IN) the same meaning in select
 * @param fdset_e (IN) the same meaning in select
 * @param tv (IN OUT) timeout
 * return  the same meaning in select
 */
HPR_DECLARE HPR_INT32 CALLBACK HPR_SelectEx(HPR_INT32 iMaxFd, fd_set* fdset_r, fd_set* fdset_w, fd_set* fdset_e, timeval* tv);

HPR_DECLARE HPR_INT32 CALLBACK HPR_Poll(struct pollfd* fds, HPR_INT32 iFds, HPR_INT32 iTimeO);

HPR_DECLARE HPR_INT32 CALLBACK HPR_PollEx(struct pollfd* fds, HPR_INT32 iFds, HPR_INT32* iTimeO);


#endif
