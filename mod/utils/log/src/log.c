#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <time.h>
#include "nnm.h"
#include "log.h"
#include "node.h"

static log_lv_t s_level = LOG_LV_INFO;
static nnm_t s_nnm = NULL;

static int log_callback(void *buf, size_t len)
{
    puts((char *)buf);
    return len;
}

int log_init(int master)
{
    int ret;

    if (master) {
        // just keep it on the background
        ret = nnm_pull_create(GSF_LOG_COM_NODE, log_callback, &s_nnm);
        assert(ret == 0);
    }

    ret = nnm_push_create(GSF_LOG_COM_NODE, &s_nnm);
    assert(ret == 0);

    return 0;
}

int log_deinit()
{
    return nnm_push_destory(s_nnm);
}

int log_setlevel(log_lv_t level)
{
    if (level >= LOG_LV_MAX) {
        return -1;
    }

    s_level = level;
    return 0;
}

int log_printf(log_lv_t level, const char *fmt, ...)
{
    int n;
    va_list ap;
    char buf[256];

    if (level > s_level) {
        return 0;
    }

    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    n = snprintf(buf, sizeof(buf), LOG_CLR_NONE"%ld.%06ld ", (long)ts.tv_sec, (long)ts.tv_nsec);

    va_start(ap, fmt);
    n += vsnprintf(&buf[n], sizeof(buf) - n, fmt, ap);
    va_end(ap);

    return nnm_push_send(s_nnm, buf, n + 1);
}