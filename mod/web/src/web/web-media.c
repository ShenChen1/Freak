#include "web-media.h"
#include "libflv/flv-header.h"
#include "libflv/flv-muxer.h"
#include "libflv/flv-writer.h"
#include "log.h"
#include "media.h"
#include "proto.h"
#include "sys/locker.h"
#include "sys/thread.h"

enum {
    MEDIA_STATUS_SETUP = 0,
    MEDIA_STATUS_PLAY  = 1,
    MEDIA_STATUS_EXIT  = 2,
    MEDIA_STATUS_MAX,
};

typedef struct {
    mfifo_t *in_fifo;
    uint8_t in_data[512 * 1024];
    uint8_t out_data[512 * 1024];
#ifdef DEBUG
    FILE *out_file;
#endif
    uint64_t pts;
    pthread_t thread;
    int status;
    web_media_cb_t snd;
    flv_muxer_t *muxer;
    void *writer;
} web_media_priv_t;

static int on_flv_packet(void *param, int type, const void *data, size_t bytes, uint32_t timestamp)
{
    return flv_writer_input(param, type, data, bytes, timestamp);
}

static int web_media_write(void *param, const struct flv_vec_t *vec, int n)
{
    int i, out_len = 0;
    web_media_priv_t *priv = param;

    for(i = 0; i < n; i++) {
        memcpy(&priv->out_data[out_len], vec[i].ptr, vec[i].len);
        out_len += vec[i].len;
    }

    if (priv->snd.proc) {
        priv->snd.proc(priv->out_data, out_len, priv->snd.args);
    }
#ifdef DEBUG
    fwrite(priv->out_data, out_len, 1, priv->out_file);
#endif

    return 0;
}

static int flv_send_proc(void *param)
{
    int i;
    web_media_priv_t *priv = param;

    thread_detach(priv->thread);
    while (priv->status == MEDIA_STATUS_SETUP) {
        usleep(100);
        break;
    }

    if (priv->snd.pre) {
        priv->snd.pre(priv->snd.args);
    }

    priv->writer = flv_writer_create2(0, 1, web_media_write, priv);
    priv->muxer = flv_muxer_create(on_flv_packet, priv->writer);
    priv->in_fifo->newest(priv->in_fifo);

    while (1) {
        if (priv->status == MEDIA_STATUS_EXIT) {
            break;
        }

        media_record_t *rec = (media_record_t *)priv->in_data;
        memset(rec, 0, sizeof(media_record_t));

        priv->in_fifo->get(priv->in_fifo, priv->in_data, sizeof(priv->in_data), 1000);
        tracef("bytes:%u tag:0x%x ts:%llu", rec->size, rec->tag, rec->ts);

        if (rec->size) {
            video_stream_t *stream = (void *)rec->buf;
            stream->pstPack = (void *)&rec->buf[sizeof(video_stream_t)];
            for (i = 0; i < stream->u32PackCount; i++) {
                if (i == 0) {
                    priv->pts = priv->pts ? priv->pts : rec->ts;
                    stream->pstPack[i].pu8Addr = (void *)&stream->pstPack[stream->u32PackCount];
                } else {
                    stream->pstPack[i].pu8Addr = stream->pstPack[i - 1].pu8Addr +
                                                 stream->pstPack[i - 1].u32Len -
                                                 stream->pstPack[i - 1].u32Offset;
                }

                flv_muxer_avc(priv->muxer,
                    stream->pstPack[i].pu8Addr,
                    stream->pstPack[i].u32Len - stream->pstPack[i].u32Offset,
                    rec->ts - priv->pts,
                    rec->ts - priv->pts);
            }
        }
    }

    flv_muxer_destroy(priv->muxer);
    flv_writer_destroy(priv->writer);
    priv->in_fifo->destroy(priv->in_fifo);
#ifdef DEBUG
    fclose(priv->out_file);
#endif

    if (priv->snd.post) {
        priv->snd.post(priv->snd.args);
    }

    free(priv);
    return 0;
}

static int __media_destroy(web_media_t *self)
{
    web_media_t *obj       = self;
    web_media_priv_t *priv = obj->priv;

    // set flag here, let worker free resource
    priv->status = MEDIA_STATUS_EXIT;

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

    priv->status = MEDIA_STATUS_PLAY;
    return 0;
}

web_media_t *web_createMedia(const char *path)
{
    web_media_t *obj       = NULL;
    web_media_priv_t *priv = NULL;

    priv = malloc(sizeof(web_media_priv_t));
    assert(priv);
    memset(priv, 0, sizeof(web_media_priv_t));

    mfifo_init_t init = { .type = MEDIA_VIDEO_STREAM };
    int ret = sscanf(path, "/flv-%d-%d", &init.chn, &init.subchn);
    if (ret != 2) {
        free(priv);
        return NULL;
    }
    priv->in_fifo = mfifo_attach(&init, 1);
    if (priv->in_fifo == NULL) {
        free(priv);
        return NULL;
    }

#ifdef DEBUG
    priv->out_file = fopen(&path[1], "wb");
#endif
    priv->status = MEDIA_STATUS_SETUP;
    thread_create(&priv->thread, flv_send_proc, priv);

    obj = malloc(sizeof(web_media_t));
    assert(obj);
    memset(obj, 0, sizeof(web_media_t));
    obj->priv        = priv;
    obj->destroy     = __media_destroy;
    obj->regcallback = __media_regcallback;

    return obj;
}
