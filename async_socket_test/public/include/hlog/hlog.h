/** @file   hlog.h
 *  @note   HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *  @brief  日志库接口，只有两个接口：hlog_set_log_callback， hlog_print，旧的日志库接口在hlog1.h中定义
 *          
 *  @使用举例：
 *      1，定义模块：HLOG_HANLDE_MODULE(NVR)（在文件下方已经默认定义了default HPP MQ 三个模块）
 *                   
 *      2，定义宏写日志
 *         #define NVR_ERROR(error_code, error_description, bussiness_id, module_id, fmt, ...) \
 *         hlog_print(HLOG_LEVEL_ERROR, HLOGGER(NVR), error_code, error_description, bussiness_id, module_id, __HLOG_FILE__, __HLOG_FUNC__, __HLOG_LINE__, fmt, ##__VA_ARGS__);
 *
 *      3，调用宏写日志
 *  @author wangjingyf@hikvision.com
 *  @date   2012/09/05 15:48:14
 *
 *  @change log
 *
 */
#ifndef __HLOG_H__
#define __HLOG_H__

#include "hlogconfig.h"
#include "hlog4cxx.h"
#include "hlog1.h"

#ifdef __cplusplus
extern "C"
{
#endif

/** @fn typedef void (HLOG_API * HlogCallBack()
 *  @brief 日志回调函数
 *  @@return 
 */
typedef void (HLOG_API *HlogCallBack)(HLOG_LEVEL level,  
                                      int error_code, 
                                      int bussiness_id,
                                      int module_id,
                                      const char* message,
                                      void* user_data);

/** @fn HLOG_DECLARE int HLOG_API hlog_set_callback2(HlogCallBack callback)
 *  @brief 设置日志回调函数，不再打印日志，日志信息通过回调函数返回
 *  @param callback[in] 回调函数指针
 *  @param user_data[in] 用户数据
 *  @@return HLOG_OK
 */
HLOG_DECLARE int HLOG_API hlog_set_log_callback(HlogCallBack callback, void* user_data);

/** @fn HLOG_DECLARE int HLOG_API hlog_print(HLOG_LEVEL level, HLOG_HANDLE module, int error_code, const char* error_description, int bussiness_id, int module_id, const char* filename, const char* funcname, int lineno, const char* format, ...)
 *  @brief 记录日志
 *  @param level[in] 日志级别
 *  @param module_handle[in] 日志模块句柄
 *  @param error_code[in] 错误码
 *  @param error_description[in] 错误描述
 *  @param bussiness_id[in] 所属 bussiness id
 *  @param module_id[in] 所属 mdule id
 *  @param filename[in] 代码文件名
 *  @param funcname[in] 代码函数名
 *  @param lineno[in] 行号
 *  @param format[in] 格式化字符串
 *  @@return 成功返回 HLOG_OK 失败返回相应错误码
 */
HLOG_DECLARE int HLOG_API hlog_print(HLOG_LEVEL level,
                                     HLOG_HANDLE module_handle, 
                                     int error_code,
                                     const char* error_description,
                                     int module_id,
                                     int bussiness_id,
                                     const char* filename, 
                                     const char* funcname, 
                                     int lineno,
                                     const char* format, ...) LOG_CHECK_FMT(10, 11);

/** @fn typedef void(HLOG_API * HlogEventCallBack(HLOG_API *HlogEventCallBack)
 *  @brief 事件回调函数
 *  @param level[in] 日志级别
 *  @param error_code[in] 错误码
 *  @param bussiness_id[in] bussiness id
 *  @param module_id[in] module id
 *  @param event_module[in] 事件模块
 *  @param message[in] 标准日志格式扩展部分
 *  @param conflict_tag[in] 冲突标签
 *  @param filter[in] 数据库过滤
 *  @param time_out[in] 超时时间
 *  @param event_type[in] 事件类型：0：开始； 1：结束；2：单独事件
 *  @param occur_time[in] 发生时间
 *  @param user_data[in] 用户数据
 *  @@return 
 */
typedef void(HLOG_API *HlogEventCallBack)(HLOG_LEVEL level,       
                                          int error_code,
                                          int bussiness_id,
                                          int module_id,
                                          const char* event_module,
                                          const char* message,           
                                          const char* conflict_tag,    
                                          const char* filter,
                                          const int time_out,
                                          const int   event_type,       
                                          const char*  occur_time,  
                                          void* user_data);      

/** @fn HLOG_DECLARE int HLOG_API hlog_set_event_callback(HlogEventCallBack listen_call_back, void* user_data)
 *  @brief 设置事件回调函数。事件的每一个动作（开始，结束）都将触发此回调
 *  @param listen_call_back[in] 回调函数
 *  @param user_data[in] 用户数据
 *  @@return HLOG_OK
 */
HLOG_DECLARE int HLOG_API hlog_set_event_callback(HlogEventCallBack listen_call_back, void* user_data);   

/** @fn HLOG_DECLARE int HLOG_API hlog_event(HLOG_LEVEL level, const char* module, int error_code, const char* error_description, int bussiness_id, int module_id, const char* filename, const char* funcname, int lineno, const char* event_module, const char* conflict_tag, const char* filter, const int time_out, const char* format, ...)
 *  @brief 记录日志，事件过滤
 *  @param level[in] 日志级别
 *  @param module_handle[in] 模块句柄，用于写日志
 *  @param error_code[in] 错误码
 *  @param error_description[in] 错误描述 
 *  @param bussiness_id[in] 所属 bussiness_id
 *  @param module_id[in] 所属 module_id
 *  @param filename[in] 文件名
 *  @param funcname[in] 函数名
 *  @param lineno[in] 行号
 *  @param event_module[in] 事件模块
 *  @param conflict_tag[in] 冲突标签
 *  @param filter[in] 数据库过滤字段
 *  @param time_out[in] 超时时间
 *  @param format[in] 格式化字符串
 *  @@return success:HCOUNTER_OK fail:错误码
 */
HLOG_DECLARE int HLOG_API hlog_event(HLOG_LEVEL level,
                                     HLOG_HANDLE module_handle,
                                     int error_code,
                                     const char* error_description,
                                     int bussiness_id,
                                     int module_id,
                                     const char* filename, 
                                     const char* funcname, 
                                     int lineno,
                                     const char* event_module, 
                                     const char* conflict_tag, 
                                     const char* filter, 
                                     const int   time_out,
                                     const char* format, ...) LOG_CHECK_FMT(14, 15);

#ifdef __cplusplus
} // __cplusplus
#endif

#define HLOGCAT(start, end) start##end
#define HLOGSTR(str) #str

#define HLOGGER(module) HLOGCAT(hlog_handle_, module)

#define HLOG_HANLDE_MODULE(module) static HLOG_HANDLE HLOGCAT(hlog_handle_, module) = hlog_OpenLoggerA(HLOGSTR(module));

#endif
