#include "web-media.h"
#include "common.h"
#include "libflv/flv-header.h"
#include "libflv/flv-muxer.h"
#include "libflv/flv-writer.h"
#include "log.h"
#include "proto.h"
#include "sys/locker.h"
#include "sys/thread.h"
#include "ufifo.h"

enum {
    MEDIA_STATUS_SETUP = 0,
    MEDIA_STATUS_PLAY  = 1,
    MEDIA_STATUS_EXIT  = 2,
    MEDIA_STATUS_MAX,
};

typedef struct {
    ufifo_t *fifo;
    flv_muxer_t *muxer;
    void *writer;
    char path[64];
    uint8_t in_data[512 * 1024];
    uint8_t out_data[512 * 1024];
    int out_status;
    int out_len;
    pthread_t thread;
    web_media_cb_t snd;
    int status;
} web_media_priv_t;

static unsigned int recsize(unsigned char *p1, unsigned int n1, unsigned char *p2)
{
    unsigned int size = sizeof(media_record_t);

    if (n1 >= size) {
        media_record_t *rec = (media_record_t *)p1;
        size                = rec->size;
    } else {
        media_record_t rec;
        char *p = (char *)(&rec);
        memcpy(p, p1, n1);
        memcpy(p + n1, p2, size - n1);
        size = rec.size;
    }

    return size;
}

static unsigned int rectag(unsigned char *p1, unsigned int n1, unsigned char *p2)
{
    unsigned int tag;
    unsigned int size = sizeof(media_record_t);

    if (n1 >= size) {
        media_record_t *rec = (media_record_t *)p1;
        tag                 = rec->tag;
    } else {
        media_record_t rec;
        char *p = (char *)(&rec);
        memcpy(p, p1, n1);
        memcpy(p + n1, p2, size - n1);
        tag = rec.tag;
    }

    return tag;
}

static unsigned int recget(unsigned char *p1, unsigned int n1, unsigned char *p2, void *arg)
{
    media_record_t *rec = arg;
    unsigned int a = 0, l = 0, _n1 = n1;
    unsigned char *p = NULL, *_p1 = p1, *_p2 = p2;

    // copy header
    p = (unsigned char *)(rec);
    a = sizeof(media_record_t);
    l = min(a, _n1);
    memcpy(p, _p1, l);
    memcpy(p + _n1, _p2, a - l);
    _n1 -= l;
    _p1 += l;
    _p2 += a - l;

    // check header
    assert(rec->tag >= (0xdeadbeef << 8));

    // copy data
    p = (unsigned char *)(rec->buf);
    a = rec->size - sizeof(media_record_t);
    l = min(a, _n1);
    memcpy(p, _p1, l);
    memcpy(p + l, _p2, a - l);
    _n1 -= l;
    _p1 += l;
    _p2 += a - l;

    return rec->size;
}

static int on_flv_packet(void *param, int type, const void *data, size_t bytes, uint32_t timestamp)
{
    return flv_writer_input(param, type, data, bytes, timestamp);
}

static int web_media_write(void *param, const void *buf, int len)
{
    web_media_priv_t *priv = param;

    memcpy(&priv->out_data[priv->out_len], buf, len);
    priv->out_len += len;

    if (priv->out_status == 2 && priv->snd.func) {
        len = priv->snd.func(priv->out_data, priv->out_len, priv->snd.args);
        priv->out_len = priv->out_status = 0;
        return len;
    }

    priv->out_status++;
    return len;
}

static int flv_send_proc(void *param)
{
    int i;
    web_media_priv_t *priv = param;

    priv->writer = flv_writer_create2(web_media_write, priv);
    priv->muxer = flv_muxer_create(on_flv_packet, priv->writer);
    priv->status = MEDIA_STATUS_PLAY;

    while (1) {

        if (priv->status == MEDIA_STATUS_EXIT) {
            break;
        }

        media_record_t *rec = (media_record_t *)priv->in_data;
        memset(rec, 0, sizeof(media_record_t));

        ufifo_get_timeout(priv->fifo, priv->in_data, sizeof(priv->in_data), 1000);
        tracef("bytes:%zu tag:0x%x ts:%zu", rec->size, rec->tag, rec->ts);

        if (rec->size) {
            video_stream_t *stream = (void *)rec->buf;
            stream->pstPack = (void *)&rec->buf[sizeof(video_stream_t)];
            for (i = 0; i < stream->u32PackCount; i++) {
                if (i == 0) {
                    stream->pstPack[i].pu8Addr = (void *)&stream->pstPack[stream->u32PackCount];
                } else {
                    stream->pstPack[i].pu8Addr = stream->pstPack[i - 1].pu8Addr +
                                                 stream->pstPack[i - 1].u32Len -
                                                 stream->pstPack[i - 1].u32Offset;
                }

                flv_muxer_avc(priv->muxer,
                    stream->pstPack[i].pu8Addr,
                    stream->pstPack[i].u32Len - stream->pstPack[i].u32Offset,
                    stream->pstPack[i].u64PTS,
                    stream->pstPack[i].u64PTS);
            }
        }
    }

    return 0;
}

static int __media_init(web_media_t *self)
{
    web_media_t *obj       = self;
    web_media_priv_t *priv = obj->priv;

    ufifo_init_t init = {
        .lock = UFIFO_LOCK_NONE,
        .opt  = UFIFO_OPT_ATTACH,
        .attach = { .shared = 1, },
        .hook = { recsize, rectag, NULL, recget },
    };
    char name[64];
    snprintf(name, sizeof(name), PROTO_VSF_STREAM_FIFO "%s", &priv->path[5]);
    ufifo_open(name, &init, &priv->fifo);
    assert(priv->fifo);

    return thread_create(&priv->thread, flv_send_proc, priv);
}

static int __media_destroy(web_media_t *self)
{
    web_media_t *obj       = self;
    web_media_priv_t *priv = obj->priv;

    priv->status = MEDIA_STATUS_EXIT;
    thread_destroy(priv->thread);

    if (priv->fifo) {
        ufifo_close(priv->fifo);
    }

    if (priv->muxer) {
        flv_muxer_destroy(priv->muxer);
    }

    if (priv->writer) {
        flv_writer_destroy(priv->writer);
    }

    free(priv);
    free(obj);
    return 0;
}

static int __media_regcallback(web_media_t *self, web_media_cb_t *cb)
{
    web_media_t *obj       = self;
    web_media_priv_t *priv = obj->priv;

    if (cb) {
        priv->snd = *cb;
    }
    return 0;
}

web_media_t *web_createMedia(const char *path, int len)
{
    web_media_t *obj       = NULL;
    web_media_priv_t *priv = NULL;

    priv = malloc(sizeof(web_media_priv_t));
    assert(priv);
    memset(priv, 0, sizeof(web_media_priv_t));
    strncpy(priv->path, path, len);
    priv->path[len] = 0;
    priv->status = MEDIA_STATUS_SETUP;

    obj = malloc(sizeof(web_media_t));
    assert(obj);
    memset(obj, 0, sizeof(web_media_t));
    obj->priv        = priv;
    obj->init        = __media_init;
    obj->destroy     = __media_destroy;
    obj->regcallback = __media_regcallback;

    return obj;
}