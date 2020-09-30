#include "common.h"
#include "nnm.h"
#include "log.h"
#include <sys/syscall.h>

static log_lv_t s_level = LOG_LV_INFO;
static uint32_t s_dirmask = LOG_DIR_LOCAL;
static nnm_t s_nnm = NULL;
static nnm_t s_server = NULL;

static int log_callback(void *buf, size_t len, void *arg)
{
    puts((char *)buf);
    return len;
}

int log_init(const char *url, int server)
{
    int ret;

    if (server) {
        // just keep it on the background
        nnm_pull_init_t init = {log_callback, NULL};
        ret = nnm_pull_create(url, &init, &s_server);
        assert(ret == 0);
    }

    ret = nnm_push_create(url, &s_nnm);
    assert(ret == 0);

    return 0;
}

int log_deinit()
{
    int ret;

    ret = nnm_push_destory(s_nnm);
    assert(ret == 0);

    if (s_server) {
        ret = nnm_pull_destory(s_server);
        assert(ret == 0);
    }

    return ret;
}

int log_setlevel(log_lv_t level)
{
    if (level >= LOG_LV_MAX) {
        return -1;
    }

    s_level = level;
    return 0;
}

int log_setdir(unsigned int mask)
{
    if (mask >= LOG_DIR_MAX) {
        return -1;
    }

    s_dirmask = mask;
    return 0;
}

int log_printf(log_lv_t level, const char *fmt, ...)
{
    int n;
    va_list ap;
    char buf[1024];

    if (level > s_level) {
        return 0;
    }

    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    n = snprintf(buf, sizeof(buf), LOG_CLR_NONE"%ld.%09ld [%d-%ld]",
        (long)ts.tv_sec, (long)ts.tv_nsec, getpid(), syscall(SYS_gettid));

    va_start(ap, fmt);
    n += vsnprintf(&buf[n], sizeof(buf) - n, fmt, ap);
    va_end(ap);

    buf[sizeof(buf)-1] = '\0';
    if (s_dirmask & LOG_DIR_LOCAL) {
        puts(buf);
    }

    if (s_dirmask & LOG_DIR_REMOTE) {
        nnm_push_send(s_nnm, buf, n + 1);
    }

    return n+1;
}