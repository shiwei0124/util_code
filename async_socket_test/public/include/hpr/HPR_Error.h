#ifndef __HPR_ERROR_H__
#define __HPR_ERROR_H__

#include "HPR_Config.h"
#include "HPR_Types.h"

#define HPR_ERROR_BASE 40000

#define HPR_EPERM		(HPR_ERROR_BASE+1)	//权限不足
#define HPR_EBADARG		(HPR_ERROR_BASE+2)	//参数不合法
#define HPR_EDISORDER	(HPR_ERROR_BASE+3)	//调用顺序不对
#define HPR_ENORESOURCE	(HPR_ERROR_BASE+4)	//资源不足
#define HPR_ENOMEM		(HPR_ERROR_BASE+5)	//没有内存
#define HPR_EWOULDBLOCK	(HPR_ERROR_BASE+6)	//暂时不能完成，需要重试
#define HPR_ETIMEO		(HPR_ERROR_BASE+7)	//超时
#define HPR_EEOF		(HPR_ERROR_BASE+8)	//EOF
#define HPR_EBUSY		(HPR_ERROR_BASE+9)	//系统忙
#define HPR_EINIT		(HPR_ERROE_BASE+10)	//初始化错误
#define HPR_ENOTFOUND	(HPR_ERROR_BASE+11)	//没有找到
#define HPR_EBADPATH	(HPR_ERROR_BASE+12)	//路径错误
#define HPR_EBADIP		(HPR_ERROR_BASE+13)	//不合法的IP地址
#define HPR_EBADMASK	(HPR_ERROR_BASE+14)	//无效掩码
#define HPR_EINVALIDSOCK (HPR_ERROR_BASE+15)//无效套接字
#define HPR_ESOCKET		(HPR_ERROR_BASE+16)	//套接字出错
#define HPR_ESOCKCLOSE	(HPR_ERROR_BASE+17)	//套接字关闭


HPR_DECLARE HPR_UINT32 CALLBACK HPR_GetLastError();

HPR_DECLARE HPR_UINT32 CALLBACK HPR_SetLastError(HPR_UINT32 nErrno);

HPR_DECLARE HPR_UINT32 CALLBACK HPR_GetSystemLastError();

HPR_DECLARE HPR_VOID   CALLBACK HPR_SetSystemLastError(HPR_UINT32 nError);

#endif

