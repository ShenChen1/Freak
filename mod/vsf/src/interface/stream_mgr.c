#include "inc/interface/stream_mgr.h"
#include "cfg/cfg.h"
#include "common.h"
#include "inc/hal/sys.h"
#include "inc/hal/venc.h"
#include "inc/hal/vi.h"
#include "inc/hal/vpss.h"
#include "log.h"
#include "ufifo.h"

typedef struct {
    ufifo_t *fifo[VSF_STREAM_MAX];
    proto_vsf_stream_t info[VSF_STREAM_MAX];
} vsf_stream_mgr_priv_t;

static vsf_stream_mgr_t *s_mgr = NULL;

static unsigned int recsize(unsigned char *p1, unsigned int n1, unsigned char *p2)
{
    unsigned int size = sizeof(media_record_t);

    if (n1 >= size) {
        media_record_t *rec = (media_record_t *)p1;
        size = rec->size;
    } else {
        media_record_t rec;
        void *p = (void *)(&rec);
        memcpy(p, p1, n1);
        memcpy(p + n1, p2, size - n1);
        size = rec.size;
    }

    tracef("size:%u", size);
    return size;
}

static unsigned int rectag(unsigned char *p1, unsigned int n1, unsigned char *p2)
{
    unsigned int tag;
    unsigned int size = sizeof(media_record_t);

    if (n1 >= size) {
        media_record_t *rec = (media_record_t *)p1;
        tag = rec->tag;
    } else {
        media_record_t rec;
        void *p = (void *)(&rec);
        memcpy(p, p1, n1);
        memcpy(p + n1, p2, size - n1);
        tag = rec.tag;
    }

    tracef("tag:%x", tag);
    return tag;
}

static unsigned int recput(unsigned char *p1, unsigned int n1, unsigned char *p2, void *arg)
{
    int i;
    size_t totalsize       = sizeof(media_record_t) + sizeof(video_stream_t);
    media_record_t rec     = {};
    video_stream_t *stream = arg;
    unsigned int a = 0, l = 0, _n1 = n1;
    unsigned char *p = NULL, *_p1 = p1, *_p2 = p2;

    for (i = 0; i < stream->u32PackCount; i++) {
        totalsize += sizeof(video_stream_pack_t);
        totalsize += stream->pstPack[i].u32Len - stream->pstPack[i].u32Offset;
    }
    rec.size = totalsize;
    rec.tag  = (0xdeadbeef << 8 | (stream->u32PackCount > 1));
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    rec.ts = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;

    // copy header
    p = (unsigned char *)(&rec);
    a = sizeof(media_record_t);
    l = min(a, _n1);
    memcpy(_p1, p, l);
    memcpy(_p2, p + l, a - l);
    _n1 -= l;
    _p1 += l;
    _p2 += a - l;

    // copy data
    p = (unsigned char *)(stream);
    a = sizeof(video_stream_t);
    l = min(a, _n1);
    memcpy(_p1, p, l);
    memcpy(_p2, p + l, a - l);
    _n1 -= l;
    _p1 += l;
    _p2 += a - l;

    for (i = 0; i < stream->u32PackCount; i++) {
        // copy data
        p = (unsigned char *)(&stream->pstPack[i]);
        a = sizeof(video_stream_pack_t);
        l = min(a, _n1);
        memcpy(_p1, p, l);
        memcpy(_p2, p + l, a - l);
        _n1 -= l;
        _p1 += l;
        _p2 += a - l;
    }

    for (i = 0; i < stream->u32PackCount; i++) {
        // copy data
        p = (unsigned char *)(stream->pstPack[i].pu8Addr);
        a = stream->pstPack[i].u32Len - stream->pstPack[i].u32Offset;
        l = min(a, _n1);
        memcpy(_p1, p, l);
        memcpy(_p2, p + l, a - l);
        _n1 -= l;
        _p1 += l;
        _p2 += a - l;
    }

    tracef("totalsize:%u", totalsize);
    return totalsize;
}

static int __vsf_stream_h264_proc(int id, void *data, void *args)
{
    int i;
    size_t totalsize            = sizeof(media_record_t) + sizeof(video_stream_t);
    video_stream_t *stream      = data;
    vsf_stream_mgr_priv_t *priv = s_mgr->priv;

    for (i = 0; i < stream->u32PackCount; i++) {
        totalsize += sizeof(video_stream_pack_t);
        totalsize += stream->pstPack[i].u32Len - stream->pstPack[i].u32Offset;
    }

    if (ufifo_len(priv->fifo[id]) + totalsize > ufifo_size(priv->fifo[id])) {
        ufifo_skip(priv->fifo[id]);
        ufifo_oldest(priv->fifo[id], (0xdeadbeef << 8) | 1);
    }

    return ufifo_put(priv->fifo[id], stream, totalsize) != totalsize;
}

static int __vsf_stream_jpeg_proc(int id, void *data, void *args)
{
    int i;
    char acFile[128];
    FILE *pFile                 = NULL;
    video_stream_t *stream      = data;
    vsf_stream_mgr_priv_t *priv = s_mgr->priv;

    /* Obtain current time. */
    struct timeval tv;
    struct tm *timeinfo;
    gettimeofday(&tv, NULL);
    timeinfo = localtime(&tv.tv_sec);
    snprintf(acFile,
             sizeof(acFile),
             "snap_chn%d_%4d_%02d_%02d_%02d_%02d_%02d_%06ld.jpg",
             priv->info[id].chn,
             1900 + timeinfo->tm_year,
             1 + timeinfo->tm_mon,
             timeinfo->tm_mday,
             timeinfo->tm_hour,
             timeinfo->tm_min,
             timeinfo->tm_sec,
             tv.tv_usec);
    pFile = fopen(acFile, "wb");
    for (i = 0; i < stream->u32PackCount; i++) {
        fwrite(stream->pstPack[i].pu8Addr + stream->pstPack[i].u32Offset,
               stream->pstPack[i].u32Len - stream->pstPack[i].u32Offset,
               1, pFile);
        fflush(pFile);
    }
    fclose(pFile);
    return 0;
}

static int __vsf_stream_proc(int id, int type, void *data, void *args)
{
    switch (type) {
    case VIDEO_ENCODE_TYPE_H264:
        return __vsf_stream_h264_proc(id, data, args);
    case VIDEO_ENCODE_TYPE_JPEG:
        return __vsf_stream_jpeg_proc(id, data, args);
    }

    return 0;
}

static int __vsf_stream_ctrl(vsf_stream_mgr_t *self, proto_vsf_stream_t *info)
{
    vsf_stream_mgr_t *mgr       = self;
    vsf_stream_mgr_priv_t *priv = mgr->priv;

    char name[64];
    snprintf(name, sizeof(name), PROTO_VSF_MEDIA_FIFO "%d-%d", info->chn, info->subchn);

    if (info->enable) {
        if (!priv->fifo[info->id]) {
            ufifo_init_t init = {
                .lock  = UFIFO_LOCK_NONE,
                .opt   = UFIFO_OPT_ALLOC,
                .alloc = { 1024 * 1024 },
                .hook  = { recsize, rectag, recput },
            };
            ufifo_open(name, &init, &priv->fifo[info->id]);
        }
    } else {
        if (priv->fifo[info->id]) {
            ufifo_close(priv->fifo[info->id]);
            priv->fifo[info->id] = NULL;
        }
    }

    memcpy(&priv->info[info->id], info, sizeof(proto_vsf_stream_t));

    vsf_stream_cb_t cb = { NULL, NULL };
    if (info->enable) {
        cb.args = &priv->info[info->id];
        cb.func = __vsf_stream_proc;
    }

    vsf_venc_t *venc = VSF_createVenc(info->id);
    assert(venc);
    venc->regcallback(venc, &cb);
    return 0;
}

vsf_stream_mgr_t *VSF_createStreamMgr()
{
    vsf_stream_mgr_t *mgr       = s_mgr;
    vsf_stream_mgr_priv_t *priv = NULL;

    if (mgr) {
        return mgr;
    }

    priv = malloc(sizeof(vsf_stream_mgr_priv_t));
    assert(priv);
    memset(priv, 0, sizeof(vsf_stream_mgr_priv_t));

    mgr = malloc(sizeof(vsf_stream_mgr_t));
    if (mgr == NULL) {
        return NULL;
    }

    mgr->priv = priv;
    mgr->ctrl = __vsf_stream_ctrl;

    s_mgr = mgr;
    return mgr;
}