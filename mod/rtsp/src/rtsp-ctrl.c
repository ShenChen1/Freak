#include "common.h"
#include "log.h"
#include "proto.h"
#include "uthash.h"
#include "ufifo.h"
#include "inc/rtsp.h"

#define RTSP_CTRL_DATA_NUM  (8)
#define RTSP_CTRL_OPEN      (1<<8|1)
#define RTSP_CTRL_CLOSE     (1<<8|2)

typedef struct {
    int ctrl;
    int size;
    char data[512];
} rtsp_ctrl_data_t;

typedef struct {
    UT_hash_handle hh;
    char url[256];
    int  refcnt;
    void *client;
    void *fifo;
} rtsp_ctrl_ctx_t;

typedef struct {
    que_t *que;
    rtsp_ctrl_ctx_t *ctxs;
} rtsp_ctrl_t;

static rtsp_ctrl_t g_rtsp_ctrl = {};

static int rtsp_ctrl_req(int ctrl, char *data, int size)
{
    int ret;
    rtsp_ctrl_t *priv = &g_rtsp_ctrl;
    rtsp_ctrl_data_t *value = malloc(sizeof(rtsp_ctrl_data_t));
    if (value == NULL) {
        return -ENOMEM;
    }
    value->ctrl = ctrl;
    value->ctrl = ctrl;
    memcpy(value->data, data, size);

    ret = que_put(priv->que, value, 1);
    if (ret) {
        free(value);
        return ret;
    }

    return 0;
}

int rtsp_ctrl_open(char *url)
{
    return rtsp_ctrl_req(RTSP_CTRL_OPEN, url, strlen(url) + 1);
}

int rtsp_ctrl_close(char *url)
{
    return rtsp_ctrl_req(RTSP_CTRL_CLOSE, url, strlen(url) + 1);
}

static int __frame(void* param,
                   const char* encoding,
                   const void* packet,
                   int bytes,
                   uint32_t time,
                   int flags)
{
    uint8_t *data = (void *)packet;
    size_t len = bytes;
    rtsp_ctrl_ctx_t *ctx = param;
    tracef("client:%p len:%zu", ctx->client, len);

    debugf(">begin: 0x%02x 0x%02x 0x%02x", data[0], data[1], data[2]);
    debugf(">begin: 0x%02x 0x%02x 0x%02x", data[3], data[4], data[5]);
    debugf(">end: 0x%02x 0x%02x 0x%02x", data[len - 6], data[len - 5], data[len - 4]);
    debugf(">end: 0x%02x 0x%02x 0x%02x", data[len - 3], data[len - 2], data[len - 1]);

    return 0;
}

void *rtsp_ctrl_thread(void *arg)
{
    int ret;
    rtsp_ctrl_t *priv = &g_rtsp_ctrl;
    rtsp_ctrl_data_t *value = NULL;
    rtsp_ctrl_ctx_t *ctx = NULL;

    ret = que_get(priv->que, (ptr_t *)&value, 1);
    if (ret || value == NULL) {
        return NULL;
    }

    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    switch (value->ctrl) {
        case RTSP_CTRL_OPEN: {
            HASH_FIND_STR(priv->ctxs, value->data, ctx);
            if (ctx) {
                ctx->refcnt++;
                tracef("open refcnt:%d", ctx->refcnt);
                break;
            }

            ctx = malloc(sizeof(rtsp_ctrl_ctx_t));
            assert(ctx);
            memset(ctx, 0, sizeof(rtsp_ctrl_ctx_t));
            ctx->refcnt = 1;
            rtsp_client_callback_t cb = {ctx, __frame};
            ctx->client = rtsp_client_init(value->data, RTSP_PROTOCOL_UDP, &cb);
            strncpy(ctx->url, value->data, sizeof(ctx->url));
            HASH_ADD_STR(priv->ctxs, url, ctx);
            break;
        }
        case RTSP_CTRL_CLOSE: {
            HASH_FIND_STR(priv->ctxs, value->data, ctx);
            if (!ctx) {
                break;
            }

            tracef("close refcnt:%d", ctx->refcnt);
            if (--ctx->refcnt) {
                break;
            }

            HASH_DEL(priv->ctxs, ctx);
            rtsp_client_uninit(ctx->client);
            free(ctx);
            break;
        }
    }

    free(value);
    return NULL;
}

int rtsp_ctrl_init()
{
    rtsp_ctrl_t *priv = &g_rtsp_ctrl;
    que_create(RTSP_CTRL_DATA_NUM, &priv->que);
    return 0;
}

int rtsp_ctrl_uninit()
{
    rtsp_ctrl_t *priv = &g_rtsp_ctrl;
    rtsp_ctrl_data_t *value = NULL;

    while (!que_get(priv->que, (ptr_t *)&value, 0)) {
        free(value);
    }

    que_delete(priv->que);
    return 0;
}