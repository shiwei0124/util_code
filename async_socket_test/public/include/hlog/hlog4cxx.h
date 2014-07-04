/** @file hlog4cxx.h
 *  @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *  @brief log4cxx日志系统的封装类
 *
 *  @author     胡通海
 *  @date       2010/06/28
 */
#ifndef __HLOG4CXX_H__
#define __HLOG4CXX_H__

#include "hlogconfig.h"

// 日志对象句柄
typedef void* HLOG4CXX_HANDLE;

#ifdef __cplusplus
extern "C"
{
#endif

    /** @fn void hlog_ConfigureA(const char* filename)
    *   @brief 根据配置文件，配置log4cxx
    *   @param filename 配置文件路径
    */
    HLOG_DECLARE void HLOG_API hlog_ConfigureA(const char* filename);
    HLOG_DECLARE void HLOG_API hlog_ConfigureW(const wchar_t* filename);

    /** @fn HLOG4CXX_HANDLE hlog_OpenLoggerA(const char* name)
    *   @brief 获取日志句柄
    *   @param name 日志对象名称
    *   @return 日志对象句柄，根据 name 查找配置文件配置的日志对象，若不存在则返回默认的日志对象
    */
    HLOG_DECLARE HLOG4CXX_HANDLE HLOG_API hlog_OpenLoggerA(const char* name);
    HLOG_DECLARE HLOG4CXX_HANDLE HLOG_API hlog_OpenLoggerW(const wchar_t* name);

    /** @fn void hlog_CloseLogger(HLOG4CXX_HANDLE logger)
    *   @brief 释放日志句柄
    *   @param logger 日志对象句柄
    */
    HLOG_DECLARE void HLOG_API hlog_CloseLogger(HLOG4CXX_HANDLE logger);

    /** @fn bool hlog_IsEnabledFor(HLOG4CXX_HANDLE logger, HLOG_LEVEL level)
    *   @brief 判断某级别的日志是否会被输出
    *   @param logger 日志对象句柄
    *   @param level 日志级别
    *   @return true - level 级别的日志会被输出，false - level 级别的日志不会被输出
    */
    HLOG_DECLARE bool HLOG_API hlog_IsEnabledFor(HLOG4CXX_HANDLE logger, HLOG_LEVEL level);

    /** @fn HLOG_LEVEL hlog_GetEffectiveLevel(HLOG4CXX_HANDLE logger)
    *   @brief 获取日志最低输出级别
    *   @param logger 日志对象句柄
    *   @return 日志最低输出级别，即低于该级别的日志将不会被输出
    */
    HLOG_DECLARE HLOG_LEVEL HLOG_API hlog_GetEffectiveLevel(HLOG4CXX_HANDLE logger);
 
    /** @fn int hlog_GetNameA(HLOG4CXX_HANDLE logger, char* const name, int size)
    *   @brief 获取日志对象名称
    *   @param logger 日志对象句柄
    *   @param name 日志对象名称容器
    *   @param size 日志对象名称容器尺寸
    *   @return 设定：length = 日志对象名称的长度（不包括结束符）。
    *       若 name == NULL ，返回 length
    *       若 size <= length ，返回 size - 1
    *       若 size > length ，返回 length
    */
    HLOG_DECLARE int HLOG_API hlog_GetNameA(HLOG4CXX_HANDLE logger, char* const name, int size);
    HLOG_DECLARE int HLOG_API hlog_GetNameW(HLOG4CXX_HANDLE logger, wchar_t* const name, int size);

    /** @fn void hlog_LogA(HLOG4CXX_HANDLE logger, HLOG_LEVEL level, const char* message)
    *   @brief 输出日志信息
    *   @param logger 日志对象句柄
    *   @param level 日志级别
    *   @param message 日志信息
    */
    HLOG_DECLARE void HLOG_API hlog_LogA(HLOG4CXX_HANDLE logger, HLOG_LEVEL level, const char* message);
    HLOG_DECLARE void HLOG_API hlog_LogW(HLOG4CXX_HANDLE logger, HLOG_LEVEL level, const wchar_t* message);

    /** @fn void hlog_LogExtA(HLOG4CXX_HANDLE logger, HLOG_LEVEL level, const char* message, const char* filename, const char* functionName, int lineNumber)
    *   @brief 输出日志信息，及相关代码信息
    *   @param logger 日志对象句柄
    *   @param level 日志级别
    *   @param message 日志信息
    *   @param filename 输出日志的代码文件路径
    *   @param functionName 输出日志的代码方法名称
    *   @param lineNumber 输出日志的代码行号
    */
    HLOG_DECLARE void HLOG_API hlog_LogExtA(HLOG4CXX_HANDLE logger, HLOG_LEVEL level, const char* message, const char* filename, const char* functionName, int lineNumber);
    HLOG_DECLARE void HLOG_API hlog_LogExtW(HLOG4CXX_HANDLE logger, HLOG_LEVEL level, const wchar_t* message, const char* filename, const char* functionName, int lineNumber);

#ifdef __cplusplus
}
#endif

#endif // __HLOG4CXX_H__
