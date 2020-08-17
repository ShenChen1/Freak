#ifndef _LOG_H_
#define _LOG_H_

#ifndef MODULE_NAME
#error Not define MODULE_NAME !!!
#endif

#define LOG_CLR_NONE "\033[m"
#define LOG_CLR_RED "\033[0;32;31m"
#define LOG_CLR_GREEN "\033[0;32;32m"
#define LOG_CLR_BLUE "\033[0;32;34m"
#define LOG_CLR_YELLOW "\033[1;33m"
#define LOG_CLR_CYAN "\033[0;36m"
#define LOG_CLR_WHITE "\033[1;37m"

typedef enum {
    LOG_LV_FATAL,
    LOG_LV_ERROR,
    LOG_LV_WARN,
    LOG_LV_INFO,
    LOG_LV_TRACE,
    LOG_LV_DEBUG,

    LOG_LV_MAX
} log_lv_t;

#define debugf(fmt, ...)                                             \
    do                                                               \
    {                                                                \
        log_printf(LOG_LV_DEBUG,                                     \
                   LOG_CLR_WHITE "[" MODULE_NAME "] "                \
                                "DEBUG (%s:%s|%d): " fmt,            \
                   __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } while (0)

#define tracef(fmt, ...)                                             \
    do                                                               \
    {                                                                \
        log_printf(LOG_LV_TRACE,                                     \
                   LOG_CLR_CYAN "[" MODULE_NAME "] "                 \
                                "TRACE (%s:%s|%d): " fmt,            \
                   __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } while (0)

#define infof(fmt, ...)                               \
    do                                                \
    {                                                 \
        log_printf(LOG_LV_INFO,                       \
                   LOG_CLR_GREEN "[" MODULE_NAME "] " \
                                 "INFO : " fmt,       \
                   ##__VA_ARGS__);                    \
    } while (0)

#define warnf(fmt, ...)                                \
    do                                                 \
    {                                                  \
        log_printf(LOG_LV_WARN,                        \
                   LOG_CLR_YELLOW "[" MODULE_NAME "] " \
                                  "WARN : " fmt,       \
                   ##__VA_ARGS__);                     \
    } while (0)

#define errorf(fmt, ...)                                             \
    do                                                               \
    {                                                                \
        log_printf(LOG_LV_ERROR,                                     \
                   LOG_CLR_RED "[" MODULE_NAME "] "                  \
                               "ERROR (%s:%s|%d): " fmt,             \
                   __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } while (0)

#define fatalf(fmt, ...)                                             \
    do                                                               \
    {                                                                \
        log_printf(LOG_LV_ERROR,                                     \
                   LOG_CLR_BLUE "[" MODULE_NAME "] "                 \
                                "FATAL (%s:%s|%d): " fmt,            \
                   __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
        asset(0);                                                    \
    } while (0)

int log_init(int master);
int log_deinit();
int log_setlevel(log_lv_t level);
int log_printf(log_lv_t level, const char *fmt, ...);

#endif //_LOG_H_