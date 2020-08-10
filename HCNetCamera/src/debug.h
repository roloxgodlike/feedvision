/*
用于调试用的宏和函数。
*/

#ifndef __DEBUG_H_
#define __DEBUG_H_

#include "public.h"

#define log(tag, fmt, args...)       \
    do { \
        printf("[%s][%s:%d] ", tag, __FUNCTION__, __LINE__); \
        printf(fmt, ##args); \
    } while(0)

#define LOG_INFO(fmt, args...)      \
    do { \
        log("INFO", fmt, ##args); \
    } while(0)

#define LOG_ERR(fmt, args...)      \
    do { \
        log("ERR", fmt, ##args); \
    } while(0)

#define ShowSDK_LastErrMsg()    LOG_ERR("errno = %d, %s\n", NET_DVR_GetLastError(), NET_DVR_GetErrorMsg(NULL))

#define FUNC_ENTRY()        LOG_INFO("Entry!\n")
#define FUNC_EXIT()         LOG_INFO("Exit!\n")

#define LOG_VAR_INT(var)    LOG_INFO("%s = %d\n", #var, var)

/* require 测试宏 */
#ifndef likely
#define likely(x)       __builtin_expect(!!(x), 1)
#endif

#ifndef unlikely
#define unlikely(x)     __builtin_expect(!!(x), 0)
#endif

#define require_action(cond, action) \
    do { \
        if (!unlikely(cond)) { \
            LOG_ERR("condition *%s* not required!!!\n", #cond); \
            { action; } \
        } \
    } while(0)

#define require_condition(cond) \
    do { \
        if (!unlikely(cond)) { \
            LOG_ERR("condition *%s* not required!!!\n", #cond); \
            return HPR_ERROR; \
        } \
    } while(0)

#define require_action_go_through(cond, action) \
    do { \
        if (!unlikely(cond)) { \
            LOG_ERR("condition *%s* not required, but go through\n", #cond); \
            { action; } \
        } \
    } while(0)

#define require_condition_go_through(cond) \
    do { \
        if (!unlikely(cond)) { \
            LOG_ERR("condition *%s* not required, but go through\n", #cond); \
        } \
    } while(0)

#define require_action_log(cond, action, fmt, args...) \
        do { \
            if (!unlikely(cond)) { \
                LOG_ERR("condition *%s* not required\n", #cond); \
                LOG_ERR(fmt, ##args); \
                { action; } \
            } \
        } while(0)


#define require_condition_log(cond, fmt, args...) \
    do { \
        if (!unlikely(cond)) { \
            LOG_ERR("condition *%s* not required\n", #cond); \
            LOG_ERR(fmt, ##args); \
            return HPR_ERROR; \
        } \
    } while(0)

#endif
