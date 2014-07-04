/** @file hlogex.h
 *  @note Hangzhou Hikvision System Technology Co., Ltd. All Rights Reserved.
 *  @brief hlog 封装，提供类型安全的格式化方法。
 *
 *  @author Hu Tonghai
 *  @date 2010/11/09
 *
 *  @note 使用示例：
 *  @note HlogWrapper nvrlogger("NVR");
 *  @note LOCATION_LOGGER(nvrlogger, HLOG_LEVEL_ERROR)("error code {0}, dsc {1}, time {2:t}, xcode {0:x}", 10001, "error", time(NULL));
 *  @note LOCATION_EVENT_LOGGER(nvrlogger, HLOG_LEVEL_ERROR)("error code {0}, dsc {1}, time {2:t}, xcode {0:x}", 10001, "error", time(NULL));
 *  @note POLICY_LOCATION_LOGGER(nvrlogger, HLOG_LEVEL_ERROR)("nvrcore", "error code {0}, dsc {1}, time {2:t}, xcode {0:x}", 10001, "error", time(NULL));
 *  @note POLICY_LOCATION_EVENT_LOGGER(nvrlogger, HLOG_LEVEL_ERROR)("nvrcore", "error code {0}, dsc {1}, time {2:t}, xcode {0:x}", 10001, "error", time(NULL));
 *
 *  @note 历史记录：
 *  @note V1.0.0.0 创建
 */
#ifndef _HLOGEX_H__
#define _HLOGEX_H__

#include <string>
#include <sstream>
#include <stdarg.h>

#if (defined _WIN32 || defined _WIN64)
#  include <time.h>
#else
#  include <sys/time.h>
#endif

#include "hlog.h"
#include "hlog4cxx.h"
// 基本类型定义
typedef signed char     hlog_int8;
typedef unsigned char   hlog_uint8;
typedef signed short    hlog_int16;
typedef unsigned short  hlog_uint16;
typedef signed int      hlog_int32;
typedef unsigned int    hlog_uint32;
typedef signed long     hlog_long;
typedef unsigned long   hlog_ulong;

#if defined(_MSC_VER)
typedef signed __int64      hlog_int64;
typedef unsigned __int64    hlog_uint64;
#else
#if defined(__LP64__)
typedef signed long     hlog_int64;
typedef unsigned long   hlog_uint64;
#else
typedef signed long long    hlog_int64;
typedef unsigned long long  hlog_uint64;
#endif //defined(__LP64__)
#endif //defined(_MSC_VER)

// 缓冲区长度定义。
#define STD_BUFFER_SIZE (1024)

// 可变参数类型定义。
#define _HLW_YNAME(x, z)		x ## z
#define _HLW_ARG_0(x)			_HLW_YNAME(x, 0)
#define _HLW_ARG_1(x)			_HLW_ARG_0(x), _HLW_YNAME(x, 1)
#define _HLW_ARG_2(x)			_HLW_ARG_1(x), _HLW_YNAME(x, 2)
#define _HLW_ARG_3(x)			_HLW_ARG_2(x), _HLW_YNAME(x, 3)
#define _HLW_ARG_4(x)			_HLW_ARG_3(x), _HLW_YNAME(x, 4)
#define _HLW_ARG_5(x)			_HLW_ARG_4(x), _HLW_YNAME(x, 5)
#define _HLW_ARG_6(x)			_HLW_ARG_5(x), _HLW_YNAME(x, 6)
#define _HLW_ARG_7(x)			_HLW_ARG_6(x), _HLW_YNAME(x, 7)
#define _HLW_ARG_8(x)			_HLW_ARG_7(x), _HLW_YNAME(x, 8)
#define _HLW_ARG_9(x)			_HLW_ARG_8(x), _HLW_YNAME(x, 9)
#define _HLW_ARG_10(x)			_HLW_ARG_9(x), _HLW_YNAME(x, 10)
#define _HLW_ARG_11(x)			_HLW_ARG_10(x), _HLW_YNAME(x, 11)
#define _HLW_ARG_12(x)			_HLW_ARG_11(x), _HLW_YNAME(x, 12)
#define _HLW_ARG_13(x)			_HLW_ARG_12(x), _HLW_YNAME(x, 13)
#define _HLW_ARG_14(x)			_HLW_ARG_13(x), _HLW_YNAME(x, 14)
#define _HLW_ARG_15(x)			_HLW_ARG_14(x), _HLW_YNAME(x, 15)
#define _HLW_ARG_16(x)			_HLW_ARG_15(x), _HLW_YNAME(x, 16)
#define _HLW_ARG_17(x)			_HLW_ARG_16(x), _HLW_YNAME(x, 17)

// 可变参数定义。
#define _HLW_YNAME2(x, y, z)    _HLW_YNAME(x, z) _HLW_YNAME(y, z)
#define _HLW_ARG2_0(x, y)		_HLW_YNAME2(x, y, 0)
#define _HLW_ARG2_1(x, y)		_HLW_ARG2_0(x, y), _HLW_YNAME2(x, y, 1)
#define _HLW_ARG2_2(x, y)		_HLW_ARG2_1(x, y), _HLW_YNAME2(x, y, 2)
#define _HLW_ARG2_3(x, y)		_HLW_ARG2_2(x, y), _HLW_YNAME2(x, y, 3)
#define _HLW_ARG2_4(x, y)		_HLW_ARG2_3(x, y), _HLW_YNAME2(x, y, 4)
#define _HLW_ARG2_5(x, y)		_HLW_ARG2_4(x, y), _HLW_YNAME2(x, y, 5)
#define _HLW_ARG2_6(x, y)		_HLW_ARG2_5(x, y), _HLW_YNAME2(x, y, 6)
#define _HLW_ARG2_7(x, y)		_HLW_ARG2_6(x, y), _HLW_YNAME2(x, y, 7)
#define _HLW_ARG2_8(x, y)		_HLW_ARG2_7(x, y), _HLW_YNAME2(x, y, 8)
#define _HLW_ARG2_9(x, y)		_HLW_ARG2_8(x, y), _HLW_YNAME2(x, y, 9)
#define _HLW_ARG2_10(x, y)		_HLW_ARG2_9(x, y), _HLW_YNAME2(x, y, 10)
#define _HLW_ARG2_11(x, y)		_HLW_ARG2_10(x, y), _HLW_YNAME2(x, y, 11)
#define _HLW_ARG2_12(x, y)		_HLW_ARG2_11(x, y), _HLW_YNAME2(x, y, 12)
#define _HLW_ARG2_13(x, y)		_HLW_ARG2_12(x, y), _HLW_YNAME2(x, y, 13)
#define _HLW_ARG2_14(x, y)		_HLW_ARG2_13(x, y), _HLW_YNAME2(x, y, 14)
#define _HLW_ARG2_15(x, y)		_HLW_ARG2_14(x, y), _HLW_YNAME2(x, y, 15)
#define _HLW_ARG2_16(x, y)		_HLW_ARG2_15(x, y), _HLW_YNAME2(x, y, 16)
#define _HLW_ARG2_17(x, y)		_HLW_ARG2_16(x, y), _HLW_YNAME2(x, y, 17)

// CASE 语句定义。
#define _HLW_CASE(x, z)         case z: FormatParam(placeholderPattern, _HLW_YNAME(x, z), outBuffer); break;
#define _HLW_CASE_0(x)          _HLW_CASE(x, 0)
#define _HLW_CASE_1(x)          _HLW_CASE_0(x) _HLW_CASE(x, 1)
#define _HLW_CASE_2(x)          _HLW_CASE_1(x) _HLW_CASE(x, 2)
#define _HLW_CASE_3(x)          _HLW_CASE_2(x) _HLW_CASE(x, 3)
#define _HLW_CASE_4(x)          _HLW_CASE_3(x) _HLW_CASE(x, 4)
#define _HLW_CASE_5(x)          _HLW_CASE_4(x) _HLW_CASE(x, 5)
#define _HLW_CASE_6(x)          _HLW_CASE_5(x) _HLW_CASE(x, 6)
#define _HLW_CASE_7(x)          _HLW_CASE_6(x) _HLW_CASE(x, 7)
#define _HLW_CASE_8(x)          _HLW_CASE_7(x) _HLW_CASE(x, 8)
#define _HLW_CASE_9(x)          _HLW_CASE_8(x) _HLW_CASE(x, 9)
#define _HLW_CASE_10(x)         _HLW_CASE_9(x) _HLW_CASE(x, 10)
#define _HLW_CASE_11(x)         _HLW_CASE_10(x) _HLW_CASE(x, 11)
#define _HLW_CASE_12(x)         _HLW_CASE_11(x) _HLW_CASE(x, 12)
#define _HLW_CASE_13(x)         _HLW_CASE_12(x) _HLW_CASE(x, 13)
#define _HLW_CASE_14(x)         _HLW_CASE_13(x) _HLW_CASE(x, 14)
#define _HLW_CASE_15(x)         _HLW_CASE_14(x) _HLW_CASE(x, 15)
#define _HLW_CASE_16(x)         _HLW_CASE_15(x) _HLW_CASE(x, 16)
#define _HLW_CASE_17(x)         _HLW_CASE_16(x) _HLW_CASE(x, 17)

// LOCATION FORMAT 方法定义。
#define _HLW_LOCATION_FORMAT_FUNCTION \
template<_HLW_ARG_N(typename V)> \
void operator ()(const char* message2, _HLW_ARG2_N(const V, & v)) \
{ \
    if ((NULL == message2) || (false == _logger.IsEnabledFor(_level))) \
    { \
        return; \
    } \
    std::string outBuffer; \
    int placeholderNo = -1; \
    std::string placeholderPattern; \
    outBuffer.reserve((std::string::size_type)STD_BUFFER_SIZE); \
    do \
    { \
        message2 = ParsePattern(message2, placeholderNo, placeholderPattern, outBuffer); \
        if (placeholderNo >= 0) \
        { \
            switch (placeholderNo) \
            { \
            _HLW_CASE_N(v) \
            default: outBuffer.append(placeholderPattern); break; \
            } \
        } \
    } while (NULL != message2); \
    this->operator ()(outBuffer); \
}

// POLICY LOCATION FORMAT 方法定义。
#define _HLW_POLICY_LOCATION_FORMAT_FUNCTION \
template<typename T, _HLW_ARG_N(typename V)> \
void operator ()(const T& policy, const char* message2, _HLW_ARG2_N(const V, & v)) \
{ \
    if ((NULL == message2) || (false == _logger.IsEnabledFor(_level))) \
    { \
        return; \
    } \
    std::string outBuffer; \
    int placeholderNo = -1; \
    std::string placeholderPattern; \
    outBuffer.reserve((std::string::size_type)STD_BUFFER_SIZE); \
    std::string policy2; \
    HoldString(policy, policy2); \
    if (false == policy2.empty()) \
    { \
        outBuffer.append(1, '['); \
        outBuffer.append(policy2); \
        outBuffer.append(1, ']'); \
    } \
    do \
    { \
        message2 = ParsePattern(message2, placeholderNo, placeholderPattern, outBuffer); \
        if (placeholderNo >= 0) \
        { \
            switch (placeholderNo) \
            { \
            _HLW_CASE_N(v) \
            default: outBuffer.append(placeholderPattern); break; \
            } \
        } \
    } while (NULL != message2); \
    this->LocationLogger::operator ()(outBuffer); \
}

/** @class class HlogWrapper [hlogex.h]
 *  @brief 封装 hlog 的 C++ 接口。
 */
class HlogWrapper
{
public:
    /** @fn HlogWrapper()
     *  @brief 默认构造一个 root hlog 对象。
     *  @param void
     *  @return void
     */
    HlogWrapper()
    {
        _logger = hlog_OpenLoggerA(NULL);
    }

    /** @fn HlogWrapper(const std::string& loggerName)
     *  @brief 构造一个名称为 loggerName 的 hlog 对象。
     *  @param loggerName [in] hlog 对象名称。
     *  @return void
     */
    HlogWrapper(const std::string& loggerName)
    {
        _logger = hlog_OpenLoggerA(loggerName.c_str());
    }

    /** @fn HlogWrapper(const char* loggerName)
     *  @brief 构造一个名称为 loggerName 的 hlog 对象。
     *  @param loggerName [in] hlog 对象名称。
     *  @return void
     */
    HlogWrapper(const char* loggerName)
    {
        _logger = hlog_OpenLoggerA(loggerName);
    }

    /** @fn ~HlogWrapper()
     *  @brief 释放 hlog 对象资源。
     *  @param void
     *  @return void
     */
    ~HlogWrapper()
    {
        hlog_CloseLogger(_logger);
    }
    HLOG4CXX_HANDLE GetLogger()
    {
        return _logger;
    }
    /** @fn bool IsEnabledFor(HLOG_LEVEL level)
     *  @brief 判断 level 级别的日志是否允许输出。
     *  @param void
     *  @return 允许输出 level 级别的日志则返回 true ，否则返回 false 。
     */
    bool IsEnabledFor(HLOG_LEVEL level)
    {
        return (hlog_GetEffectiveLevel(_logger) <= level);
    }

    /** @fn void Log(HLOG_LEVEL level, const char* message)
     *  @brief 若允许输出 level 级别的日志，则输出 message 信息。
     *  @param level [in] message 的级别。
     *  @param message [in] 日志信息。
     *  @return void
     */
    void Log(HLOG_LEVEL level, const char* message)
    {
        if (true == IsEnabledFor(level))
        {
            hlog_LogA(_logger, level, message);
        }
    }

    /** @fn void Log(HLOG_LEVEL level, const std::string& message)
     *  @brief 若允许输出 level 级别的日志，则输出 message 信息。
     *  @param level [in] message 的级别。
     *  @param message [in] 日志信息。
     *  @return void
     */
    void Log(HLOG_LEVEL level, const std::string& message)
    {
        if (true == IsEnabledFor(level))
        {
            hlog_LogA(_logger, level, message.c_str());
        }
    }

    /** @fn void Log(HLOG_LEVEL level, const char* message, const char* filename, const char* functionName, int lineNumber)
     *  @brief 若允许输出 level 级别的日志，则输出 message、filename、functionName、lineNumber 等信息。
     *  @param level [in] message 的级别。
     *  @param message [in] 日志信息。
     *  @param filename [in] 输出日志的代码所在的文件名。
     *  @param functionName [in] 输出日志的代码所在的函数名称。
     *  @param lineNumber [in] 输出日志的代码所处文件的行号。
     *  @return void
     */
    void Log(HLOG_LEVEL level, const char* message,
        const char* filename, const char* functionName, int lineNumber)
    {
        if (true == IsEnabledFor(level))
        {
            hlog_LogExtA(_logger, level, message, filename, functionName, lineNumber);
        }
    }

    /** @fn void Log(HLOG_LEVEL level, const std::string& message, const char* filename, const char* functionName, int lineNumber)
     *  @brief 若允许输出 level 级别的日志，则输出 message、filename、functionName、lineNumber 等信息。
     *  @param level [in] message 的级别。
     *  @param message [in] 日志信息。
     *  @param filename [in] 输出日志的代码所在的文件名。
     *  @param functionName [in] 输出日志的代码所在的函数名称。
     *  @param lineNumber [in] 输出日志的代码所处文件的行号。
     *  @return void
     */
    void Log(HLOG_LEVEL level, const std::string& message,
        const char* filename, const char* functionName, int lineNumber)
    {
        if (true == IsEnabledFor(level))
        {
            hlog_LogExtA(_logger, level, message.c_str(), filename, functionName, lineNumber);
        }
    }

    /** @fn bool IsTraceEnabled()
     *  @brief 判断 HLOG_LEVEL_TRACE 级别的日志是否允许输出。
     *  @param void
     *  @return 允许输出 HLOG_LEVEL_TRACE 级别的日志则返回 true ，否则返回 false 。
     */
    bool IsTraceEnabled()
    {
        return (hlog_GetEffectiveLevel(_logger) <= HLOG_LEVEL_TRACE);
    }

    /** @fn bool IsDebugEnabled()
     *  @brief 判断 HLOG_LEVEL_DEBUG 级别的日志是否允许输出。
     *  @param void
     *  @return 允许输出 HLOG_LEVEL_DEBUG 级别的日志则返回 true ，否则返回 false 。
     */
    bool IsDebugEnabled()
    {
        return (hlog_GetEffectiveLevel(_logger) <= HLOG_LEVEL_DEBUG);
    }

    /** @fn bool IsInfoEnabled()
     *  @brief 判断 HLOG_LEVEL_INFO 级别的日志是否允许输出。
     *  @param void
     *  @return 允许输出 HLOG_LEVEL_INFO 级别的日志则返回 true ，否则返回 false 。
     */
    bool IsInfoEnabled()
    {
        return (hlog_GetEffectiveLevel(_logger) <= HLOG_LEVEL_INFO);
    }

    /** @fn bool IsWarnEnabled()
     *  @brief 判断 HLOG_LEVEL_WARN 级别的日志是否允许输出。
     *  @param void
     *  @return 允许输出 HLOG_LEVEL_WARN 级别的日志则返回 true ，否则返回 false 。
     */
    bool IsWarnEnabled()
    {
        return (hlog_GetEffectiveLevel(_logger) <= HLOG_LEVEL_WARN);
    }

    /** @fn bool IsErrorEnabled()
     *  @brief 判断 HLOG_LEVEL_ERROR 级别的日志是否允许输出。
     *  @param void
     *  @return 允许输出 HLOG_LEVEL_ERROR 级别的日志则返回 true ，否则返回 false 。
     */
    bool IsErrorEnabled()
    {
        return (hlog_GetEffectiveLevel(_logger) <= HLOG_LEVEL_ERROR);
    }

    /** @fn bool IsFatalEnabled()
     *  @brief 判断 HLOG_LEVEL_FATAL 级别的日志是否允许输出。
     *  @param void
     *  @return 允许输出 HLOG_LEVEL_FATAL 级别的日志则返回 true ，否则返回 false 。
     */
    bool IsFatalEnabled()
    {
        return (hlog_GetEffectiveLevel(_logger) <= HLOG_LEVEL_FATAL);
    }

protected:
    HlogWrapper(const HlogWrapper&);
    const HlogWrapper& operator =(const HlogWrapper&);

private:
    HLOG4CXX_HANDLE _logger; ///< hlog 日志句柄。
};

/** @class class LocationLogger [hlogex.h]
 *  @brief 简化日志对象使用方式。
 */
class LocationLogger
{
protected:
    HlogWrapper& _logger; ///< 日志对象。
    HLOG_LEVEL _level; ///< 日志级别。
    const char* _filename; ///< 创建 LocationLogger 对象代码所处的文件路径。
    const char* _functionName; ///< 创建 LocationLogger 对象代码所处的函数名称。
    int _lineNumber; ///< 创建 LocationLogger 对象代码所处文件的行号。

    //event
    bool _event_enable;
    const char* _event_module;///<事件模块
    int _event_time_out;///<事件超时时间
    const char* _event_conflict_tag;///<事件冲突标签
    const char* _event_filter;///<数据库过滤

public:
    LocationLogger(HlogWrapper& logger, HLOG_LEVEL level,
        const char* filename, const char* functionName, int lineNumber,
        bool event_enable, const char* event_module, int event_time_out, 
        const char* event_conflict_tag = "", const char* event_filter = "")
        : _logger(logger), _level(level), _filename(filename), _functionName(functionName)
        , _lineNumber(lineNumber), _event_enable(event_enable)
        , _event_module(event_module), _event_time_out(event_time_out)
        , _event_conflict_tag(event_conflict_tag), _event_filter(event_filter)
    {
    }

    ~LocationLogger()
    {
    }

private:
    LocationLogger(const LocationLogger&);
    const LocationLogger& operator =(const LocationLogger&);

protected:
    /** @fn bool IsNotNumber(char c)
     *  @brief 判断字符 c 是否是数字。
     *  @param c [in] 字符。
     *  @return 若字符 c 不是数字[0,9]则返回 true ，否则返回 false 。
     */
    bool IsNotNumber(char c)
    {
        return ((c > '9') || (c < '0'));
    }

    /** @fn bool IsNumber(char c)
     *  @brief 判断字符 c 是否是数字。
     *  @param c [in] 字符。
     *  @return 若字符 c 是数字[0,9]则返回 true ，否则返回 false 。
     */
    bool IsNumber(char c)
    {
        return !IsNotNumber(c);
    }

    /** @fn int Ssnprintf(char* buffer, size_t bufferSize, const char* fmt, ...)
     *  @brief 提供可变参数的格式化。
     *  @param buffer [in] 输出缓冲区。
     *  @param bufferSize [in] 输出缓冲区尺寸。
     *  @param fmt [in] 格式化字符串。
     *  @param ... [in] 不定数量的输入参数。
     *  @return 成功返回 buffer 中有效字符串的长度。
     */
    int Ssnprintf(char* buffer, size_t bufferSize, const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
#if (defined _WIN32 || defined _WIN64)
        int rlt = _vsnprintf_s(buffer, bufferSize, _TRUNCATE, fmt, args);
#else
        int rlt = vsnprintf(buffer, bufferSize, fmt, args);
#endif
        va_end(args);

        return rlt;
    }

    /** @fn template<typename V> void HoldString(const V& v, std::string& outBuffer)
     *  @brief 将 v 的值格式化为字符串。
     *  @param v [in] 源值。
     *  @param outBuffer [out] 字符串缓冲区。
     *  @return void
     */
    template<typename V> void HoldString(const V& v, std::string& outBuffer)
    {
        std::stringstream outStream;
        outStream << v;
        outBuffer.append(outStream.str());
    }
    template<typename V> void HoldString(V* const& v, std::string& outBuffer)
    {
        char buf[24] = { '\0' };
        Ssnprintf(buf, sizeof(buf), "%p", v);
        outBuffer.append(buf);
    }
    void HoldString(char** const& v, std::string& outBuffer)
    {
        if (NULL != v)
        {
            outBuffer.append(v[0]);
        }
    }
    void HoldString(const char** const& v, std::string& outBuffer)
    {
        if (NULL != v)
        {
            outBuffer.append(v[0]);
        }
    }
    void HoldString(char* const* const& v, std::string& outBuffer)
    {
        if (NULL != v)
        {
            outBuffer.append(v[0]);
        }
    }
    void HoldString(const char* const* const& v, std::string& outBuffer)
    {
        if (NULL != v)
        {
            outBuffer.append(v[0]);
        }
    }
    void HoldString(char* const& v, std::string& outBuffer)
    {
        if (NULL != v)
        {
            outBuffer.append(v);
        }
    }
    void HoldString(const char* const& v, std::string& outBuffer)
    {
        if (NULL != v)
        {
            outBuffer.append(v);
        }
    }
    void HoldString(const std::string& v, std::string& outBuffer)
    {
        outBuffer.append(v);
    }
    void HoldString(const char& v, std::string& outBuffer)
    {
        char buf[4] = { '\0' };
        Ssnprintf(buf, sizeof(buf), "%c", v);
        outBuffer.append(buf);
    }
    void HoldString(const hlog_uint8& v, std::string& outBuffer)
    {
        char buf[4] = { '\0' };
        Ssnprintf(buf, sizeof(buf), "%u", (hlog_uint32)v);
        outBuffer.append(buf);
    }
    void HoldString(const hlog_int16& v, std::string& outBuffer)
    {
        char buf[8] = { '\0' };
        Ssnprintf(buf, sizeof(buf), "%d", (hlog_int32)v);
        outBuffer.append(buf);
    }
    void HoldString(const hlog_uint16& v, std::string& outBuffer)
    {
        char buf[8] = { '\0' };
        Ssnprintf(buf, sizeof(buf), "%u", (hlog_uint32)v);
        outBuffer.append(buf);
    }
    void HoldString(const hlog_int32& v, std::string& outBuffer)
    {
        char buf[24] = { '\0' };
        Ssnprintf(buf, sizeof(buf), "%d", v);
        outBuffer.append(buf);
    }
    void HoldString(const hlog_uint32& v, std::string& outBuffer)
    {
        char buf[24] = { '\0' };
        Ssnprintf(buf, sizeof(buf), "%u", v);
        outBuffer.append(buf);
    }
    void HoldString(const hlog_int64& v, std::string& outBuffer)
    {
        char buf[24] = { '\0' };
#if defined(_MSC_VER)
        Ssnprintf(buf, sizeof(buf), "%I64d", v);
#elif defined(__LP64__)
        Ssnprintf(buf, sizeof(buf), "%ld", v);
#else
        Ssnprintf(buf, sizeof(buf), "%lld", v);
#endif
        outBuffer.append(buf);
    }
    void HoldString(const hlog_uint64& v, std::string& outBuffer)
    {
        char buf[24] = { '\0' };
#if defined(_MSC_VER)
        Ssnprintf(buf, sizeof(buf), "%I64u", v);
#elif defined(__LP64__)
        Ssnprintf(buf, sizeof(buf), "%lu", v);
#else
        Ssnprintf(buf, sizeof(buf), "%llu", v);
#endif
        outBuffer.append(buf);
    }
    void HoldString(const float& v, std::string& outBuffer)
    {
        char buf[128] = { '\0' };
        Ssnprintf(buf, sizeof(buf), "%g", v);
        outBuffer.append(buf);
    }
    void HoldString(const double& v, std::string& outBuffer)
    {
        char buf[128] = { '\0' };
        Ssnprintf(buf, sizeof(buf), "%g", v);
        outBuffer.append(buf);
    }
    void HoldString(const long double& v, std::string& outBuffer)
    {
        char buf[128] = { '\0' };
        Ssnprintf(buf, sizeof(buf), "%lg", v);
        outBuffer.append(buf);
    }
    void HoldString(const bool& v, std::string& outBuffer)
    {
        if (true == v)
        {
            outBuffer.append("true");
        }
        else
        {
            outBuffer.append("false");
        }
    }

#if (defined(_MSC_VER) || !defined(__LP64__)) // 防止类型定义与int64冲突。
    void HoldString(const hlog_long& v, std::string& outBuffer)
    {
        char buf[24] = { '\0' };
        Ssnprintf(buf, sizeof(buf), "%ld", v);
        outBuffer.append(buf);
    }
    void HoldString(const hlog_ulong& v, std::string& outBuffer)
    {
        char buf[24] = { '\0' };
        Ssnprintf(buf, sizeof(buf), "%lu", v);
        outBuffer.append(buf);
    }
#endif

    /** @fn template<typename V> bool HoldUnsigned(const V& v, hlog_uint64& u)
     *  @brief 提取类型可转换为无符号整形的 v 的值。
     *  @param v [in] 源值。
     *  @param u [out] 若 v 的类型可转换为无符号整形则用 v 值填充，否则忽略。
     *  @return 若 v 的类型可转换为无符号整形则返回 true ，否则返回 false 。
     */
    template<typename V> bool HoldUnsigned(const V& /*v*/, hlog_uint64& /*u*/) { /*(void)v; (void)u;*/ return false; }
    template<typename V> bool HoldUnsigned(V* const& v, hlog_uint64& u) { u = (hlog_uint64)v; return true; }
    bool HoldUnsigned(const hlog_int8& v, hlog_uint64& u) { u = (hlog_uint64)v; return true; }
    bool HoldUnsigned(const hlog_uint8& v, hlog_uint64& u) { u = (hlog_uint64)v; return true; }
    bool HoldUnsigned(const hlog_int16& v, hlog_uint64& u) { u = (hlog_uint64)v; return true; }
    bool HoldUnsigned(const hlog_uint16& v, hlog_uint64& u) { u = (hlog_uint64)v; return true; }
    bool HoldUnsigned(const hlog_int32& v, hlog_uint64& u) { u = (hlog_uint64)v; return true; }
    bool HoldUnsigned(const hlog_uint32& v, hlog_uint64& u) { u = (hlog_uint64)v; return true; }
    bool HoldUnsigned(const hlog_int64& v, hlog_uint64& u) { u = (hlog_uint64)v; return true; }
    bool HoldUnsigned(const hlog_uint64& v, hlog_uint64& u) { u = (hlog_uint64)v; return true; }
    bool HoldUnsigned(const float& v, hlog_uint64& u) { u = (hlog_uint64)v; return true; }
    bool HoldUnsigned(const double& v, hlog_uint64& u) { u = (hlog_uint64)v; return true; }
    bool HoldUnsigned(const long double& v, hlog_uint64& u) { u = (hlog_uint64)v; return true; }
    bool HoldUnsigned(const bool& v, hlog_uint64& u) { u = (hlog_uint64)v; return true; }

#if (defined(_MSC_VER) || !defined(__LP64__)) // 防止类型定义与int64冲突。
    bool HoldUnsigned(const hlog_long& v, hlog_uint64& u) { u = (hlog_uint64)v; return true; }
    bool HoldUnsigned(const hlog_ulong& v, hlog_uint64& u) { u = (hlog_uint64)v; return true; }
#endif

    /** @fn template<typename V> bool HoldTime(const V& v, time_t& t)
     *  @brief 提取类型为 time_t 的 v 的值。
     *  @param v [in] 源值。
     *  @param t [out] 若 v 的类型为 time_t 则用 v 值填充，否则忽略。
     *  @return 若 v 的类型为 time_t 则返回 true ，否则返回 false 。
     */
    template<typename V> bool HoldTime(const V& /*v*/, time_t& /*t*/) { /*(void)v; (void)t;*/ return false; }
    bool HoldTime(const time_t& v, time_t& t) { t = v; return true; }

    /** @fn void Vtox(hlog_uint64 v, std::string& outBuffer)
     *  @brief 将数字 v 格式化为 16 进制（小写），并输出至 outBuffer 中。
     *  @param v [in] 待格式化的数字。
     *  @param buffer [out] 接收格式化后的字符串的缓冲区。
     *  @return void
     */
    void Vtox(const hlog_uint64& v, std::string& outBuffer)
    {
        char buf[20] = { '\0' };
#if defined(_MSC_VER)
        Ssnprintf(buf, sizeof(buf), "%I64x", v);
#elif defined(__LP64__)
        Ssnprintf(buf, sizeof(buf), "%lx", v);
#else
        Ssnprintf(buf, sizeof(buf), "%llx", v);
#endif
        outBuffer.append(buf);
    }

    /** @fn void VtoX(hlog_uint64 v, std::string& outBuffer)
     *  @brief 将数字 v 格式化为 16 进制（大写），并输出至 outBuffer 中。
     *  @param v [in] 待格式化的数字。
     *  @param buffer [out] 接收格式化后的字符串的缓冲区。
     *  @return void
     */
    void VtoX(const hlog_uint64& v, std::string& outBuffer)
    {
        char buf[20] = { '\0' };
#if defined(_MSC_VER)
        Ssnprintf(buf, sizeof(buf), "%I64X", v);
#elif defined(__LP64__)
        Ssnprintf(buf, sizeof(buf), "%lX", v);
#else
        Ssnprintf(buf, sizeof(buf), "%llX", v);
#endif
        outBuffer.append(buf);
    }

    /** @fn void VtoTime(const time_t& t, std::string& outBuffer)
     *  @brief 将时间 t 格式化为 "yyyy-mm-dd HH:MM:SS" 格式，并输出至 outBuffer 中。
     *  @param t [in] 待格式化的时间。
     *  @param buffer [out] 接收格式化后的字符串的缓冲区。
     *  @return void
     */
    void VtoTime(const time_t& t, std::string& outBuffer)
    {
        tm rtm;
        tm* ltm = NULL;
#if (defined _WIN32 || defined _WIN64)
        if (!localtime_s(&rtm, &t))
        {
            ltm = &rtm;
        }
#else
        ltm = localtime_r(&t, &rtm);
#endif

        if (NULL != ltm)
        {
            char buffer[20] = { '\0' };
            strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ltm);
            outBuffer.append(buffer);
        }
        else
        {
            outBuffer.append("(invalid time ");
            HoldString((hlog_int64)t, outBuffer);
            outBuffer.append(")");
        }
    }

    /** @fn template<typename V> void FormatParam(const std::string& placeholderPattern, const V& v, std::string& outBuffer)
     *  @brief 根据 placeholderPattern 格式化输出 v 至 outStream 中。
     *  @param placeholderPattern [in] 格式化字符串。
     *  @param v [in] 待格式化的值。
     *  @param outBuffer [out] 输出缓冲区。
     *  @return void
     */
    template<typename V> void FormatParam(const std::string& placeholderPattern, const V& v,
        std::string& outBuffer)
    {
        // no pattern
        if (true == placeholderPattern.empty())
        {
            HoldString(v, outBuffer);

            return;
        }

        // unknown placeholderPattern
        if (placeholderPattern.size() >= 2)
        {
            goto unknown_pattern;
        }

        // x
        if ('x' == placeholderPattern.at(0))
        {
            hlog_uint64 hv = 0;
            if (true == HoldUnsigned(v, hv))
            {
               Vtox(hv, outBuffer);
            }
            else
            {
                outBuffer.append("(parse error)");
            }

            return;
        }

        // X
        if ('X' == placeholderPattern.at(0))
        {
            hlog_uint64 hv = 0;
            if (true == HoldUnsigned(v, hv))
            {
                VtoX(hv, outBuffer);
            }
            else
            {
                outBuffer.append("(parse error)");
            }

            return;
        }

        // t or T
        if (('t' == placeholderPattern.at(0))
            || ('T' == placeholderPattern.at(0)))
        {
            time_t tv = 0;
            if (true == HoldTime(v, tv))
            {
               VtoTime(tv, outBuffer);
            }
            else
            {
                outBuffer.append("(parse error)");
            }

            return;
        }

unknown_pattern:
        // error
        outBuffer.append(placeholderPattern);
    }

    /** @fn const char* ParsePattern(const char* message, int& placeholderNo, std::string& placeholderPattern, std::string& outBuffer)
     *  @brief 从格式化字符串 message 中解析出第一个占位符，拷贝第一个占位符之前非占位符字串至输出缓冲区 outBuffer 中。
     *  @param message [in] 格式化字符串。
     *  @param placeholderNo [out] message 中的第一个占位符的编号。
     *  @param placeholderPattern [out] message 中的第一个占位符的模式。
     *  @param outBuffer [out] 输出缓冲区。
     *  @return 返回 message 中剩余的字符串的首地址。
     */
    const char* ParsePattern(const char* message, int& placeholderNo, std::string& placeholderPattern,
        std::string& outBuffer)
    {
        placeholderNo = -1;
        placeholderPattern.clear();

        if (NULL == message)
        {
            return NULL;
        }

        for ( ; ; )
        {
            // over
            if ('\0' == message[0])
            {
                return NULL;
            }

            // not { or }
            if (('{' != message[0])
                && ('}' != message[0]))
            {
                outBuffer.append(1, message[0]);

                ++message;
                continue;
            }

            // {{ or }}
            if (message[1] == message[0])
            {
                outBuffer.append(1, message[0]);

                message += 2;
                continue;
            }

            // }x
            if ('}' == message[0])
            {
                // error
                ++message;
                continue;
            }

            // {n or {x
            ++message;

            // {x
            if (true == IsNotNumber(message[0]))
            {
                // error
                continue;
            }

            // {n
            ++message;

            // {n}
            if ('}' == message[0])
            {
                // got placeholder number
                const char* number = (const char*)(message - 1);
                char tmp[4] = { number[0], '\0' };
                placeholderNo = atoi(tmp);

                ++message;

                // over
                if ('\0' == message[0])
                {
                    return NULL;
                }
                else
                {
                    return message;
                }
            }

            // {n:
            if (':' == message[0])
            {
                // got placeholder number
                const char* number = (const char*)(message - 1);
                char tmp[4] = { number[0], '\0' };
                placeholderNo = atoi(tmp);

                ++message;
                goto placeholder_pattern;
            }

            // {nx
            if (true == IsNotNumber(message[0]))
            {
                // error
                continue;
            }

            // {nn
            ++message;

            // {nn}
            if ('}' == message[0])
            {
                // got placeholder number
                const char* number = (const char*)(message - 2);
                char tmp[4] = { number[0], number[1], '\0' };
                placeholderNo = atoi(tmp);

                ++message;

                // over
                if ('\0' == message[0])
                {
                    return NULL;
                }
                else
                {
                    return message;
                }
            }

            // {nn:
            if (':' == message[0])
            {
                // got placeholder number
                const char* number = (const char*)(message - 2);
                char tmp[4] = { number[0], number[1], '\0' };
                placeholderNo = atoi(tmp);

                ++message;
                goto placeholder_pattern;
            }

            // {nnx
            // error
            continue;

placeholder_pattern:
            // got placeholder pattern
            for ( ; ; )
            {
                // {n:...} or {nn:...}
                if ('}' == message[0])
                {
                    ++message;

                    // over
                    if ('\0' == message[0])
                    {
                        return NULL;
                    }
                    else
                    {
                        return message;
                    }
                }

                // {n:...{ or {nn:...{
                if ('{' == message[0])
                {
                    // error
                    placeholderNo = -1;
                    if (false == placeholderPattern.empty())
                    {
                        outBuffer.append(placeholderPattern);
                        placeholderPattern.clear();
                    }

                    break;
                }

                // {n:...\0 or {nn:...\0
                if ('\0' == message[0])
                {
                    // error
                    placeholderNo = -1;
                    if (false == placeholderPattern.empty())
                    {
                        outBuffer.append(placeholderPattern);
                        placeholderPattern.clear();
                    }

                    // over
                    return NULL;
                }

                // {n:... or {nn:...
                placeholderPattern.append(1, message[0]);
                ++message;
            }
        }
    }

public:
    /** @fn void operator ()(const char* message)
     *  @brief 输出日志信息 message 。
     *  @param message [in] 日志信息。
     *  @return void
     */
    void operator ()(const char* message)
    {
        if (true == _event_enable && NULL != message)
        {
            hlog_event(_level, _logger.GetLogger(), 0, "not use", 0, 0, _filename, _functionName, \
                       _lineNumber, _event_module, _event_conflict_tag, _event_filter, \
                       _event_time_out, message);
        }
        else if(NULL != message)
        {
            _logger.Log(_level, message, _filename, _functionName, _lineNumber);
        }
    }

    /** @fn void operator ()(const std::string& message)
     *  @brief 输出日志信息 message 。
     *  @param message [in] 日志信息。
     *  @return void
     */
    void operator ()(const std::string& message)
    {
        if (true == _event_enable && !message.empty())
        {
            hlog_event(_level, _logger.GetLogger(), 0, "not use", 0, 0, _filename, _functionName, \
                       _lineNumber, _event_module, _event_conflict_tag, _event_filter, \
                       _event_time_out, message.c_str());
        }
        else if(!message.empty())
        {
            _logger.Log(_level, message.c_str(), _filename, _functionName, _lineNumber);
        }
    }

// 可变参数方法定义。
#define _HLW_ARG_N _HLW_ARG_0
#define _HLW_ARG2_N _HLW_ARG2_0
#define _HLW_CASE_N _HLW_CASE_0
    _HLW_LOCATION_FORMAT_FUNCTION
#undef _HLW_ARG_N
#undef _HLW_ARG2_N
#undef _HLW_CASE_N

#define _HLW_ARG_N _HLW_ARG_1
#define _HLW_ARG2_N _HLW_ARG2_1
#define _HLW_CASE_N _HLW_CASE_1
    _HLW_LOCATION_FORMAT_FUNCTION
#undef _HLW_ARG_N
#undef _HLW_ARG2_N
#undef _HLW_CASE_N

#define _HLW_ARG_N _HLW_ARG_2
#define _HLW_ARG2_N _HLW_ARG2_2
#define _HLW_CASE_N _HLW_CASE_2
    _HLW_LOCATION_FORMAT_FUNCTION
#undef _HLW_ARG_N
#undef _HLW_ARG2_N
#undef _HLW_CASE_N

#define _HLW_ARG_N _HLW_ARG_3
#define _HLW_ARG2_N _HLW_ARG2_3
#define _HLW_CASE_N _HLW_CASE_3
    _HLW_LOCATION_FORMAT_FUNCTION
#undef _HLW_ARG_N
#undef _HLW_ARG2_N
#undef _HLW_CASE_N

#define _HLW_ARG_N _HLW_ARG_4
#define _HLW_ARG2_N _HLW_ARG2_4
#define _HLW_CASE_N _HLW_CASE_4
    _HLW_LOCATION_FORMAT_FUNCTION
#undef _HLW_ARG_N
#undef _HLW_ARG2_N
#undef _HLW_CASE_N

#define _HLW_ARG_N _HLW_ARG_5
#define _HLW_ARG2_N _HLW_ARG2_5
#define _HLW_CASE_N _HLW_CASE_5
    _HLW_LOCATION_FORMAT_FUNCTION
#undef _HLW_ARG_N
#undef _HLW_ARG2_N
#undef _HLW_CASE_N

#define _HLW_ARG_N _HLW_ARG_6
#define _HLW_ARG2_N _HLW_ARG2_6
#define _HLW_CASE_N _HLW_CASE_6
    _HLW_LOCATION_FORMAT_FUNCTION
#undef _HLW_ARG_N
#undef _HLW_ARG2_N
#undef _HLW_CASE_N

#define _HLW_ARG_N _HLW_ARG_7
#define _HLW_ARG2_N _HLW_ARG2_7
#define _HLW_CASE_N _HLW_CASE_7
    _HLW_LOCATION_FORMAT_FUNCTION
#undef _HLW_ARG_N
#undef _HLW_ARG2_N
#undef _HLW_CASE_N

#define _HLW_ARG_N _HLW_ARG_8
#define _HLW_ARG2_N _HLW_ARG2_8
#define _HLW_CASE_N _HLW_CASE_8
    _HLW_LOCATION_FORMAT_FUNCTION
#undef _HLW_ARG_N
#undef _HLW_ARG2_N
#undef _HLW_CASE_N

#define _HLW_ARG_N _HLW_ARG_9
#define _HLW_ARG2_N _HLW_ARG2_9
#define _HLW_CASE_N _HLW_CASE_9
    _HLW_LOCATION_FORMAT_FUNCTION
#undef _HLW_ARG_N
#undef _HLW_ARG2_N
#undef _HLW_CASE_N

#define _HLW_ARG_N _HLW_ARG_10
#define _HLW_ARG2_N _HLW_ARG2_10
#define _HLW_CASE_N _HLW_CASE_10
    _HLW_LOCATION_FORMAT_FUNCTION
#undef _HLW_ARG_N
#undef _HLW_ARG2_N
#undef _HLW_CASE_N

#define _HLW_ARG_N _HLW_ARG_11
#define _HLW_ARG2_N _HLW_ARG2_11
#define _HLW_CASE_N _HLW_CASE_11
    _HLW_LOCATION_FORMAT_FUNCTION
#undef _HLW_ARG_N
#undef _HLW_ARG2_N
#undef _HLW_CASE_N

#define _HLW_ARG_N _HLW_ARG_12
#define _HLW_ARG2_N _HLW_ARG2_12
#define _HLW_CASE_N _HLW_CASE_12
    _HLW_LOCATION_FORMAT_FUNCTION
#undef _HLW_ARG_N
#undef _HLW_ARG2_N
#undef _HLW_CASE_N

#define _HLW_ARG_N _HLW_ARG_13
#define _HLW_ARG2_N _HLW_ARG2_13
#define _HLW_CASE_N _HLW_CASE_13
    _HLW_LOCATION_FORMAT_FUNCTION
#undef _HLW_ARG_N
#undef _HLW_ARG2_N
#undef _HLW_CASE_N

#define _HLW_ARG_N _HLW_ARG_14
#define _HLW_ARG2_N _HLW_ARG2_14
#define _HLW_CASE_N _HLW_CASE_14
    _HLW_LOCATION_FORMAT_FUNCTION
#undef _HLW_ARG_N
#undef _HLW_ARG2_N
#undef _HLW_CASE_N

#define _HLW_ARG_N _HLW_ARG_15
#define _HLW_ARG2_N _HLW_ARG2_15
#define _HLW_CASE_N _HLW_CASE_15
    _HLW_LOCATION_FORMAT_FUNCTION
#undef _HLW_ARG_N
#undef _HLW_ARG2_N
#undef _HLW_CASE_N

#define _HLW_ARG_N _HLW_ARG_16
#define _HLW_ARG2_N _HLW_ARG2_16
#define _HLW_CASE_N _HLW_CASE_16
    _HLW_LOCATION_FORMAT_FUNCTION
#undef _HLW_ARG_N
#undef _HLW_ARG2_N
#undef _HLW_CASE_N

#define _HLW_ARG_N _HLW_ARG_17
#define _HLW_ARG2_N _HLW_ARG2_17
#define _HLW_CASE_N _HLW_CASE_17
    _HLW_LOCATION_FORMAT_FUNCTION
#undef _HLW_ARG_N
#undef _HLW_ARG2_N
#undef _HLW_CASE_N
};

/** @class class LocationLogger [hlogex.h]
 *  @brief 简化日志对象使用方式。
 */
class PolicyLocationLogger : protected LocationLogger
{
public:
    PolicyLocationLogger(HlogWrapper& logger, HLOG_LEVEL level,
        const char* filename, const char* functionName, int lineNumber, bool event_enable, 
        const char* event_module, int event_time_out, const char* event_conflict_tag, const char* event_filter)
        : LocationLogger(logger, level, filename, functionName, lineNumber, event_enable,
                         event_module, event_time_out, event_conflict_tag, event_filter)
    {
    }

    ~PolicyLocationLogger()
    {
    }

    /** @fn template<typename T> void operator ()(const std::string& policy, const char* message)
     *  @brief 输出日志信息 message ，附加标识 policy 。
     *  @param policy [in] 附加标识。
     *  @param message [in] 日志信息。
     *  @return void
     */
    template<typename T> void operator ()(const T& policy, const char* message)
    {
        if ((NULL == message) || (false == _logger.IsEnabledFor(_level)))
        {
            return;
        }

        std::string policy2;
        HoldString(policy, policy2);
        if (true == policy2.empty())
        {
            _logger.Log(_level, message, _filename, _functionName, _lineNumber);
        }
        else
        {
            size_t tmpLen = (size_t)policy2.size();
            tmpLen += strlen(message);
            tmpLen += 3;
            std::string tmp;
            tmp.reserve((std::string::size_type)tmpLen);

            tmp.append(1, '[');
            tmp.append(policy2);
            tmp.append(1, ']');
            tmp.append(message);

            _logger.Log(_level, tmp, _filename, _functionName, _lineNumber);
        }
    }

// 可变参数方法定义。
#define _HLW_ARG_N _HLW_ARG_0
#define _HLW_ARG2_N _HLW_ARG2_0
#define _HLW_CASE_N _HLW_CASE_0
    _HLW_POLICY_LOCATION_FORMAT_FUNCTION
#undef _HLW_ARG_N
#undef _HLW_ARG2_N
#undef _HLW_CASE_N

#define _HLW_ARG_N _HLW_ARG_1
#define _HLW_ARG2_N _HLW_ARG2_1
#define _HLW_CASE_N _HLW_CASE_1
    _HLW_POLICY_LOCATION_FORMAT_FUNCTION
#undef _HLW_ARG_N
#undef _HLW_ARG2_N
#undef _HLW_CASE_N

#define _HLW_ARG_N _HLW_ARG_2
#define _HLW_ARG2_N _HLW_ARG2_2
#define _HLW_CASE_N _HLW_CASE_2
    _HLW_POLICY_LOCATION_FORMAT_FUNCTION
#undef _HLW_ARG_N
#undef _HLW_ARG2_N
#undef _HLW_CASE_N

#define _HLW_ARG_N _HLW_ARG_3
#define _HLW_ARG2_N _HLW_ARG2_3
#define _HLW_CASE_N _HLW_CASE_3
    _HLW_POLICY_LOCATION_FORMAT_FUNCTION
#undef _HLW_ARG_N
#undef _HLW_ARG2_N
#undef _HLW_CASE_N

#define _HLW_ARG_N _HLW_ARG_4
#define _HLW_ARG2_N _HLW_ARG2_4
#define _HLW_CASE_N _HLW_CASE_4
    _HLW_POLICY_LOCATION_FORMAT_FUNCTION
#undef _HLW_ARG_N
#undef _HLW_ARG2_N
#undef _HLW_CASE_N

#define _HLW_ARG_N _HLW_ARG_5
#define _HLW_ARG2_N _HLW_ARG2_5
#define _HLW_CASE_N _HLW_CASE_5
    _HLW_POLICY_LOCATION_FORMAT_FUNCTION
#undef _HLW_ARG_N
#undef _HLW_ARG2_N
#undef _HLW_CASE_N

#define _HLW_ARG_N _HLW_ARG_6
#define _HLW_ARG2_N _HLW_ARG2_6
#define _HLW_CASE_N _HLW_CASE_6
    _HLW_POLICY_LOCATION_FORMAT_FUNCTION
#undef _HLW_ARG_N
#undef _HLW_ARG2_N
#undef _HLW_CASE_N

#define _HLW_ARG_N _HLW_ARG_7
#define _HLW_ARG2_N _HLW_ARG2_7
#define _HLW_CASE_N _HLW_CASE_7
    _HLW_POLICY_LOCATION_FORMAT_FUNCTION
#undef _HLW_ARG_N
#undef _HLW_ARG2_N
#undef _HLW_CASE_N

#define _HLW_ARG_N _HLW_ARG_8
#define _HLW_ARG2_N _HLW_ARG2_8
#define _HLW_CASE_N _HLW_CASE_8
    _HLW_POLICY_LOCATION_FORMAT_FUNCTION
#undef _HLW_ARG_N
#undef _HLW_ARG2_N
#undef _HLW_CASE_N

#define _HLW_ARG_N _HLW_ARG_9
#define _HLW_ARG2_N _HLW_ARG2_9
#define _HLW_CASE_N _HLW_CASE_9
    _HLW_POLICY_LOCATION_FORMAT_FUNCTION
#undef _HLW_ARG_N
#undef _HLW_ARG2_N
#undef _HLW_CASE_N

#define _HLW_ARG_N _HLW_ARG_10
#define _HLW_ARG2_N _HLW_ARG2_10
#define _HLW_CASE_N _HLW_CASE_10
    _HLW_POLICY_LOCATION_FORMAT_FUNCTION
#undef _HLW_ARG_N
#undef _HLW_ARG2_N
#undef _HLW_CASE_N

#define _HLW_ARG_N _HLW_ARG_11
#define _HLW_ARG2_N _HLW_ARG2_11
#define _HLW_CASE_N _HLW_CASE_11
    _HLW_POLICY_LOCATION_FORMAT_FUNCTION
#undef _HLW_ARG_N
#undef _HLW_ARG2_N
#undef _HLW_CASE_N

#define _HLW_ARG_N _HLW_ARG_12
#define _HLW_ARG2_N _HLW_ARG2_12
#define _HLW_CASE_N _HLW_CASE_12
    _HLW_POLICY_LOCATION_FORMAT_FUNCTION
#undef _HLW_ARG_N
#undef _HLW_ARG2_N
#undef _HLW_CASE_N

#define _HLW_ARG_N _HLW_ARG_13
#define _HLW_ARG2_N _HLW_ARG2_13
#define _HLW_CASE_N _HLW_CASE_13
    _HLW_POLICY_LOCATION_FORMAT_FUNCTION
#undef _HLW_ARG_N
#undef _HLW_ARG2_N
#undef _HLW_CASE_N

#define _HLW_ARG_N _HLW_ARG_14
#define _HLW_ARG2_N _HLW_ARG2_14
#define _HLW_CASE_N _HLW_CASE_14
    _HLW_POLICY_LOCATION_FORMAT_FUNCTION
#undef _HLW_ARG_N
#undef _HLW_ARG2_N
#undef _HLW_CASE_N

#define _HLW_ARG_N _HLW_ARG_15
#define _HLW_ARG2_N _HLW_ARG2_15
#define _HLW_CASE_N _HLW_CASE_15
    _HLW_POLICY_LOCATION_FORMAT_FUNCTION
#undef _HLW_ARG_N
#undef _HLW_ARG2_N
#undef _HLW_CASE_N

#define _HLW_ARG_N _HLW_ARG_16
#define _HLW_ARG2_N _HLW_ARG2_16
#define _HLW_CASE_N _HLW_CASE_16
    _HLW_POLICY_LOCATION_FORMAT_FUNCTION
#undef _HLW_ARG_N
#undef _HLW_ARG2_N
#undef _HLW_CASE_N

#define _HLW_ARG_N _HLW_ARG_17
#define _HLW_ARG2_N _HLW_ARG2_17
#define _HLW_CASE_N _HLW_CASE_17
    _HLW_POLICY_LOCATION_FORMAT_FUNCTION
#undef _HLW_ARG_N
#undef _HLW_ARG2_N
#undef _HLW_CASE_N
};

// 日志宏定义。
#if defined(_MSC_VER)
#  define __HLOG4CXX_FILE__ __FILE__
#  define __HLOG4CXX_FUNC__ __FUNCSIG__
#  define __HLOG4CXX_LINE__ __LINE__
#elif defined(__GNUC__)
#  define __HLOG4CXX_FILE__ __BASE_FILE__
#  define __HLOG4CXX_FUNC__ __PRETTY_FUNCTION__
#  define __HLOG4CXX_LINE__ __LINE__
#else
#  define __HLOG4CXX_FILE__ ""
#  define __HLOG4CXX_FUNC__ ""
#  define __HLOG4CXX_LINE__ 0
#endif

#define LOCATION_LOGGER(logger, level) \
    LocationLogger(logger, level, __HLOG4CXX_FILE__, __HLOG4CXX_FUNC__, __HLOG4CXX_LINE__,\
                   false, "", 0, "", "")

#define LOCATION_EVENT_LOGGER(logger, level, event_module, event_time_out, event_conflict_tag, event_filter) \
    LocationLogger(logger, level, __HLOG4CXX_FILE__, __HLOG4CXX_FUNC__, __HLOG4CXX_LINE__,\
                   true, event_module, event_time_out, event_conflict_tag, event_filter)

#define POLICY_LOCATION_LOGGER(logger, level) \
    PolicyLocationLogger(logger, level, __HLOG4CXX_FILE__, __HLOG4CXX_FUNC__, __HLOG4CXX_LINE__, \
                         false, "", 0, "", "")

#define POLICY_LOCATION_EVENT_LOGGER(logger, level, event_module, event_time_out, event_conflict_tag, event_filter) \
    PolicyLocationLogger(logger, level, __HLOG4CXX_FILE__, __HLOG4CXX_FUNC__, __HLOG4CXX_LINE__, \
                         true, event_module, event_time_out, event_conflict_tag, event_filter)
// ROOT LOGGER
inline HlogWrapper& GetRootLogger()
{
    static HlogWrapper instance;
    return instance;
}

#define ROOT_LOCATION_LOGGER(level) LOCATION_LOGGER(GetRootLogger(), level)
#define ROOT_POLICY_LOCATION_LOGGER(level) POLICY_LOCATION_LOGGER(GetRootLogger(), level)

#define ROOT_TRACE   ROOT_LOCATION_LOGGER(HLOG_LEVEL_TRACE)
#define ROOT_DEBUG   ROOT_LOCATION_LOGGER(HLOG_LEVEL_DEBUG)
#define ROOT_INFO    ROOT_LOCATION_LOGGER(HLOG_LEVEL_INFO)
#define ROOT_WARN    ROOT_LOCATION_LOGGER(HLOG_LEVEL_WARN)
#define ROOT_ERROR   ROOT_LOCATION_LOGGER(HLOG_LEVEL_ERROR)
#define ROOT_FATAL   ROOT_LOCATION_LOGGER(HLOG_LEVEL_FATAL)

#define ROOT_PTRACE   ROOT_POLICY_LOCATION_LOGGER(HLOG_LEVEL_TRACE)
#define ROOT_PDEBUG   ROOT_POLICY_LOCATION_LOGGER(HLOG_LEVEL_DEBUG)
#define ROOT_PINFO    ROOT_POLICY_LOCATION_LOGGER(HLOG_LEVEL_INFO)
#define ROOT_PWARN    ROOT_POLICY_LOCATION_LOGGER(HLOG_LEVEL_WARN)
#define ROOT_PERROR   ROOT_POLICY_LOCATION_LOGGER(HLOG_LEVEL_ERROR)
#define ROOT_PFATAL   ROOT_POLICY_LOCATION_LOGGER(HLOG_LEVEL_FATAL)

#endif // #ifndef _HLOGEX_H__
