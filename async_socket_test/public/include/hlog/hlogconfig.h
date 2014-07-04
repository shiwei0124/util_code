/** @file hlog.h
 *  @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *  @brief hlog公共配置
 *
 *  @author     wangjingyf
 */
#ifndef __HLOGCONFIG_H__
#define __HLOGCONFIG_H__

#ifndef NULL
#define NULL 0
#endif // NULL

#if defined(_MSC_VER) // 时间类型
    typedef signed __int64	EVENT_TIME;
#elif defined(__GNUC__) || defined(__SYMBIAN32__)
#   if defined(__LP64__)
        typedef signed long EVENT_TIME;
#   else
        typedef signed long long EVENT_TIME;
#   endif	//defined(__LP64__)
#endif

#if (defined _WIN32 || defined _WIN64)
#   ifdef HLOG_EXPORTS
#       define HLOG_DECLARE __declspec(dllexport)
#   else
#       define HLOG_DECLARE __declspec(dllimport)
#   endif
#   define HLOG_API __stdcall
#else
#   ifdef __linux__
#       define HLOG_DECLARE 
#   else
#       define HLOG_DECLARE
#   endif
#   define HLOG_API
#endif

/** @enum HLOG_LEVEL
 *  @brief 日志级别
 */
typedef enum {
    HLOG_LEVEL_OFF    = 7,
    HLOG_LEVEL_FATAL  = 6,
    HLOG_LEVEL_ERROR  = 5,
    HLOG_LEVEL_WARN   = 4,
    HLOG_LEVEL_INFO   = 3,
    HLOG_LEVEL_DEBUG  = 2,
    HLOG_LEVEL_TRACE  = 1,
    HLOG_LEVEL_ALL    = 0
} HLOG_LEVEL;

// 错误码定义。
#define HLOG_OK                             0 ///< 操作成功。
#define HLOG_ERR                            -1 ///< 操作失败。
#define HLOG_BASE                           ((int)0x00000000)   ///< 错误码基数
#define HLOG_E_InvalidParam                 (HLOG_BASE + 1) ///< 参数不合法
#define HLOG_E_OutOfMemory                  (HLOG_BASE + 2) ///< 申请内存失败
#define HLOG_E_TaskQueueFail                (HLOG_BASE + 3) ///< 向队列中添加任务失败
#define HLOG_E_NULLPointer                  (HLOG_BASE + 4) ///< 空指针
#define HLOG_E_LevelNotEnable               (HLOG_BASE + 5) ///< 日志级别没有达到输出的级别
#define HLOG_E_CheckConfigTimer             (HLOG_BASE + 6) ///< 启动检查配置文件线程失败
#define HLOG_E_OpenLoggerModuleFail         (HLOG_BASE + 9) ///< 打开日志句柄失败
#define HLOG_E_CreateThreadFail             (HLOG_BASE + 11) ///< 创建线程失败
#define HLOG_E_ConfigFileNotExist           (HLOG_BASE + 13) ///< 配置文件不存在
#define HLOG_E_BufferTooSmall               (HLOG_BASE + 16) ///< 缓冲区过小
#define HLOG_E_UnKnown                      (HLOG_BASE + 17) ///< 未知错误

// event error code
#define HLOG_E_DBOperateFail                (HLOG_BASE + 21) ///< db操作失败
#define HLOG_E_RecordEventFail              (HLOG_BASE + 22) ///< 记录事件失败

#define HLOG_IGNORE ""
#define HLOG_HANDLE void*

// 日志宏定义。
#if defined(_MSC_VER)
#  define __HLOG_FILE__ __FILE__
#  define __HLOG_FUNC__ __FUNCSIG__
#  define __HLOG_LINE__ __LINE__
#elif defined(__GNUC__)
#  define __HLOG_FILE__ __BASE_FILE__
#  define __HLOG_FUNC__ __PRETTY_FUNCTION__
#  define __HLOG_LINE__ __LINE__
#else
#  define __HLOG_FILE__ ""
#  define __HLOG_FUNC__ ""
#  define __HLOG_LINE__ 0
#endif

#ifdef __GNUC__
#define LOG_CHECK_FMT(a, b) __attribute__((format(printf, a, b)))
#define CALLBACK
#else
#define LOG_CHECK_FMT(a, b)
#endif

#endif // __HLOGCONFIG_H__
