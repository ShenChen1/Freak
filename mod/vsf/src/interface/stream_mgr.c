#include "vsf/stream_mgr.h"
#include "inc/cfg.h"
#include "inc/hal/sys.h"
#include "inc/hal/venc.h"
#include "inc/hal/vi.h"
#include "inc/hal/vpss.h"
#include "log.h"
#include "proto.h"
#include "ufifo.h"

typedef struct {
    ufifo_t *fifo[VSF_STREAM_MAX];
    proto_vsf_stream_t *info;
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

static int __vsf_stream_h264_proc(video_stream_t *stream, void *args)
{
    int i;
    size_t totalsize            = sizeof(media_record_t) + sizeof(video_stream_t);
    vsf_stream_mgr_priv_t *priv = s_mgr->priv;
    proto_vsf_stream_cfg_t *cfg = args;

    for (i = 0; i < stream->u32PackCount; i++) {
        totalsize += sizeof(video_stream_pack_t);
        totalsize += stream->pstPack[i].u32Len - stream->pstPack[i].u32Offset;
    }

    if (ufifo_len(priv->fifo[cfg->id]) + totalsize > ufifo_size(priv->fifo[cfg->id])) {
        ufifo_skip(priv->fifo[cfg->id]);
        ufifo_oldest(priv->fifo[cfg->id], (0xdeadbeef << 8) | 1);
    }

    return ufifo_put(priv->fifo[cfg->id], stream, totalsize) != totalsize;
}

static int __vsf_stream_jpeg_proc(video_stream_t *stream, void *args)
{
    int i;
    char acFile[128];
    FILE *pFile                 = NULL;
    vsf_stream_mgr_priv_t *priv = s_mgr->priv;
    proto_vsf_stream_cfg_t *cfg = args;

    /* Obtain current time. */
    struct timeval tv;
    struct tm *timeinfo;
    gettimeofday(&tv, NULL);
    timeinfo = localtime(&tv.tv_sec);
    snprintf(acFile,
             sizeof(acFile),
             "snap_chn%d_%4d_%02d_%02d_%02d_%02d_%02d_%06ld.jpg",
             priv->info->caps[cfg->id].chn,
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

static int __vsf_stream_proc(void *data, void *args)
{
    video_stream_t *stream = data;

    switch (stream->enType) {
        case VIDEO_STREAM_TYPE_H264:
            return __vsf_stream_h264_proc(stream, args);
        case VIDEO_STREAM_TYPE_JPEG:
            return __vsf_stream_jpeg_proc(stream, args);
        default:
            return -1;
    }

    return 0;
}

static int __vsf_stream_set(vsf_stream_mgr_t *self, proto_vsf_stream_cfg_t *cfg)
{
    int ret;
    vsf_stream_mgr_t *mgr       = self;
    vsf_stream_mgr_priv_t *priv = mgr->priv;

    char name[64];
    snprintf(name, sizeof(name), PROTO_VSF_STREAM_FIFO "%d-%d",
        priv->info->caps[cfg->id].chn, priv->info->caps[cfg->id].subchn);

    if (cfg->enable) {
        if (!priv->fifo[cfg->id]) {
            ufifo_init_t init = {
                .lock  = UFIFO_LOCK_NONE,
                .opt   = UFIFO_OPT_ALLOC,
                .alloc = { 4 * 1024 * 1024 },
                .hook  = { recsize, rectag, recput },
            };
            ret = ufifo_open(name, &init, &priv->fifo[cfg->id]);
            assert(!ret);
            assert(priv->fifo[cfg->id]);
        }
    } else {
        if (priv->fifo[cfg->id]) {
            ufifo_close(priv->fifo[cfg->id]);
            priv->fifo[cfg->id] = NULL;
        }
    }

    vsf_stream_cb_t cb = { NULL, NULL };
    if (cfg->enable) {
        cb.args = &priv->info[cfg->id];
        cb.func = __vsf_stream_proc;
    }

    vsf_venc_t *venc = VSF_createVenc(cfg->id);
    // reg hook
    assert(venc->regcallback);
    venc->regcallback(venc, &cb);

    return 0;
}

static int __vsf_stream_get(vsf_stream_mgr_t *self, proto_vsf_stream_cfg_t *cfg)
{
    vsf_stream_mgr_t *mgr       = self;
    vsf_stream_mgr_priv_t *priv = mgr->priv;

    *cfg = priv->info->cfgs[cfg->id];
    return 0;
}

static int __vsf_stream_cap(vsf_stream_mgr_t *self, proto_vsf_stream_cap_t *cap)
{
    vsf_stream_mgr_t *mgr       = self;
    vsf_stream_mgr_priv_t *priv = mgr->priv;

    *cap = priv->info->caps[cap->id];
    return 0;
}

static int __vsf_stream_num(vsf_stream_mgr_t *self)
{
    vsf_stream_mgr_t *mgr       = self;
    vsf_stream_mgr_priv_t *priv = mgr->priv;

    return priv->info->num;
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
    priv->info = cfg_get_member(stream);

    mgr = malloc(sizeof(vsf_stream_mgr_t));
    if (mgr == NULL) {
        return NULL;
    }

    mgr->priv = priv;
    mgr->get  = __vsf_stream_get;
    mgr->set  = __vsf_stream_set;
    mgr->num  = __vsf_stream_num;
    mgr->cap  = __vsf_stream_cap;

    s_mgr = mgr;
    return mgr;
}