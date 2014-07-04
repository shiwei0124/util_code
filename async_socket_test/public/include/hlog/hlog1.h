/** @file   hlog1.h
 *  @note   HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *  @brief  日智库旧接口
 *          
 *  @note   create file
 *  @author wangjingyf@hikvision.com
 *  @date   2012/09/05 15:30:44
 *
 *  @change log
 *
 */
#ifndef __HLOG1_H__
#define __HLOG1_H__

#include "hlogconfig.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef void (HLOG_API *HLOG_CB)(HLOG_LEVEL level, const char* message);

    /** @fn void hlog_init(const char* name, char async = false)
    *   @brief 兼容旧接口，不执行任何操作
    */
#ifdef __cplusplus
    HLOG_DECLARE int HLOG_API hlog_init(const char* name, char async = false);
#else
    HLOG_DECLARE int HLOG_API hlog_init(const char* name, char async);
#endif

    /** @fn void hlog_ocx_init(const char* name, const char* path, char async = false)
    *   @brief 兼容旧接口，不执行任何操作
    */
#ifdef __cplusplus
    HLOG_DECLARE int HLOG_API hlog_ocx_init(const char* name, const char* path, char async = false);
#else
    HLOG_DECLARE int HLOG_API hlog_ocx_init(const char* name, const char* path, char async);
#endif

    /** @fn void hlog_fini()
    *   @brief 兼容旧接口，不执行任何操作
    */
    HLOG_DECLARE int HLOG_API hlog_fini();

    /** @fn void hlog_set_callback(HLOG_CB callback)
    *   @brief 设置日志的回调函数
    *   @param callback 用于处理日志信息的回调函数
    */
    HLOG_DECLARE int HLOG_API hlog_set_callback(HLOG_CB callback);

    /** @fn void hlog_level(HLOG_LEVEL level)
    *   @brief 兼容旧接口，不执行任何操作
    */
    HLOG_DECLARE int HLOG_API hlog_level(HLOG_LEVEL level);

    /** @fn void hlog_format(HLOG_LEVEL level, const char* module, const char* format, ...)
    *   @brief 写日志格式化数据
    *   @param level 日志级别
    *   @param module 日志模块
    *   @param format 日志格式化字符串，后接可变参数
    *   @note hlog_format会调用hlog_set_callback传入的回调函数
    */
    HLOG_DECLARE int HLOG_API hlog_format(HLOG_LEVEL level, const char* module, const char* format, ...) LOG_CHECK_FMT(3, 4);

    /** @fn void hlog_message(HLOG_LEVEL level, const char* module, const char* message)
    *   @brief 写日志字符串
    *   @param level 日志级别
    *   @param module 日志模块
    *   @param message 日志字符串
    *   @note hlog_message不会调用hlog_set_callback传入的回调函数
    */
    HLOG_DECLARE int HLOG_API hlog_message(HLOG_LEVEL level, const char* module, const char* message);

#ifdef __cplusplus
}
#endif

// 考虑兼容性，特增加log_printf_def和log_printf_dst
#if (defined _WIN32 || defined _WIN64)
#define log_printf_def(level, format, ...)      hlog_format(level, "", format, ##__VA_ARGS__)
#else
#define log_printf_def(level, format, args...)  hlog_format(level, "", format, ##args)
#endif
#define log_printf_dst hlog_format

// for defualt module
#if (defined _WIN32 || defined _WIN64)
    #define LOG_ERROR(fmt, ...)         hlog_format(HLOG_LEVEL_ERROR, "", "<%d>\t<%s>,"fmt, __LINE__, __FUNCTION__, ##__VA_ARGS__)
    #define LOG_WARN(fmt, ...)          hlog_format(HLOG_LEVEL_WARN, "", "<%d>\t<%s>,"fmt,  __LINE__, __FUNCTION__, ##__VA_ARGS__)    
    #define LOG_INFO(fmt, ...)          hlog_format(HLOG_LEVEL_INFO, "", "<%d>\t<%s>,"fmt,  __LINE__, __FUNCTION__, ##__VA_ARGS__)    
    #define LOG_DEBUG(fmt, ...)         hlog_format(HLOG_LEVEL_DEBUG, "", "<%d>\t<%s>,"fmt, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
    #define LOG_TRACE(fmt, ...)         hlog_format(HLOG_LEVEL_TRACE, "", "<%d>\t<%s>,"fmt, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#else
    #define LOG_ERROR(fmt, args...)     hlog_format(HLOG_LEVEL_ERROR, "", "<%s>|<%d>\t<%s>,"fmt, __FILE__, __LINE__, __FUNCTION__, ##args)
    #define LOG_WARN(fmt, args...)      hlog_format(HLOG_LEVEL_WARN, "", "<%s>|<%d>\t<%s>,"fmt,  __FILE__, __LINE__, __FUNCTION__, ##args)
    #define LOG_INFO(fmt, args...)      hlog_format(HLOG_LEVEL_INFO, "", "<%s>|<%d>\t<%s>,"fmt,  __FILE__, __LINE__, __FUNCTION__, ##args)
    #define LOG_DEBUG(fmt, args...)     hlog_format(HLOG_LEVEL_DEBUG, "", "<%s>|<%d>\t<%s>,"fmt, __FILE__, __LINE__, __FUNCTION__, ##args)
    #define LOG_TRACE(fmt, args...)     hlog_format(HLOG_LEVEL_TRACE, "", "<%s>|<%d>\t<%s>,"fmt, __FILE__, __LINE__, __FUNCTION__, ##args)
#endif

// for HPP module
#if (defined _WIN32 || defined _WIN64)
    #define HPP_ERROR(fmt, ...)         hlog_format(HLOG_LEVEL_ERROR, "HPP", "<%d>\t<%s>,"fmt, __LINE__, __FUNCTION__, ##__VA_ARGS__)
    #define HPP_WARN(fmt, ...)          hlog_format(HLOG_LEVEL_WARN, "HPP", "<%d>\t<%s>,"fmt,  __LINE__, __FUNCTION__, ##__VA_ARGS__)    
    #define HPP_INFO(fmt, ...)          hlog_format(HLOG_LEVEL_INFO, "HPP", "<%d>\t<%s>,"fmt,  __LINE__, __FUNCTION__, ##__VA_ARGS__)    
    #define HPP_DEBUG(fmt, ...)         hlog_format(HLOG_LEVEL_DEBUG, "HPP", "<%d>\t<%s>,"fmt, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
    #define HPP_TRACE(fmt, ...)         hlog_format(HLOG_LEVEL_TRACE, "HPP", "<%d>\t<%s>,"fmt, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#else
    #define HPP_ERROR(fmt, args...)     hlog_format(HLOG_LEVEL_ERROR, "HPP", "<%s>|<%d>\t<%s>,"fmt, __FILE__, __LINE__, __FUNCTION__, ##args)
    #define HPP_WARN(fmt, args...)      hlog_format(HLOG_LEVEL_WARN, "HPP", "<%s>|<%d>\t<%s>,"fmt,  __FILE__, __LINE__, __FUNCTION__, ##args)
    #define HPP_INFO(fmt, args...)      hlog_format(HLOG_LEVEL_INFO, "HPP", "<%s>|<%d>\t<%s>,"fmt,  __FILE__, __LINE__, __FUNCTION__, ##args)
    #define HPP_DEBUG(fmt, args...)     hlog_format(HLOG_LEVEL_DEBUG, "HPP", "<%s>|<%d>\t<%s>,"fmt, __FILE__, __LINE__, __FUNCTION__, ##args)
    #define HPP_TRACE(fmt, args...)     hlog_format(HLOG_LEVEL_TRACE, "HPP", "<%s>|<%d>\t<%s>,"fmt, __FILE__, __LINE__, __FUNCTION__, ##args)
#endif

// for MQ module
#if (defined _WIN32 || defined _WIN64)
    #define MQ_ERROR(fmt, ...)          hlog_format(HLOG_LEVEL_ERROR, "MQ", "<%d>\t<%s>,"fmt, __LINE__, __FUNCTION__, ##__VA_ARGS__)
    #define MQ_WARN(fmt, ...)           hlog_format(HLOG_LEVEL_WARN, "MQ", "<%d>\t<%s>,"fmt,  __LINE__, __FUNCTION__, ##__VA_ARGS__)    
    #define MQ_INFO(fmt, ...)           hlog_format(HLOG_LEVEL_INFO, "MQ", "<%d>\t<%s>,"fmt,  __LINE__, __FUNCTION__, ##__VA_ARGS__)    
    #define MQ_DEBUG(fmt, ...)          hlog_format(HLOG_LEVEL_DEBUG, "MQ", "<%d>\t<%s>,"fmt, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
    #define MQ_TRACE(fmt, ...)          hlog_format(HLOG_LEVEL_TRACE, "MQ", "<%d>\t<%s>,"fmt, __LINE__, __FUNCTION__, ##__VA_ARGS__)    
#else
    #define MQ_ERROR(fmt, args...)      hlog_format(HLOG_LEVEL_ERROR, "MQ", "<%s>|<%d>\t<%s>,"fmt, __FILE__, __LINE__, __FUNCTION__, ##args)
    #define MQ_WARN(fmt, args...)       hlog_format(HLOG_LEVEL_WARN, "MQ", "<%s>|<%d>\t<%s>,"fmt,  __FILE__, __LINE__, __FUNCTION__, ##args)
    #define MQ_INFO(fmt, args...)       hlog_format(HLOG_LEVEL_INFO, "MQ", "<%s>|<%d>\t<%s>,"fmt,  __FILE__, __LINE__, __FUNCTION__, ##args)
    #define MQ_DEBUG(fmt, args...)      hlog_format(HLOG_LEVEL_DEBUG, "MQ", "<%s>|<%d>\t<%s>,"fmt, __FILE__, __LINE__, __FUNCTION__, ##args)
    #define MQ_TRACE(fmt, args...)      hlog_format(HLOG_LEVEL_TRACE, "MQ", "<%s>|<%d>\t<%s>,"fmt, __FILE__, __LINE__, __FUNCTION__, ##args)
#endif

#endif
