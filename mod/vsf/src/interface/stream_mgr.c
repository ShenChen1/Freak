#include "vsf/stream_mgr.h"
#include "inc/cfg.h"
#include "inc/hal/sys.h"
#include "inc/hal/venc.h"
#include "inc/hal/vi.h"
#include "inc/hal/vpss.h"
#include "log.h"
#include "media.h"
#include "proto.h"

typedef struct {
    mfifo_t *fifo[VSF_STREAM_MAX];
    proto_vsf_stream_t *info;
} vsf_stream_mgr_priv_t;

static vsf_stream_mgr_t *s_mgr = NULL;

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

    if (priv->fifo[cfg->id]->len(priv->fifo[cfg->id]) + totalsize >
        priv->fifo[cfg->id]->size(priv->fifo[cfg->id])) {
        priv->fifo[cfg->id]->skip(priv->fifo[cfg->id]);
        priv->fifo[cfg->id]->oldest(priv->fifo[cfg->id]);
    }

    return priv->fifo[cfg->id]->put(priv->fifo[cfg->id], stream, totalsize, -1) != totalsize;
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
               1,
               pFile);
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

static int __vsf_stream_destroy(vsf_stream_mgr_t *self)
{
    // never destroy
    return 0;
}

static int __vsf_stream_set(vsf_stream_mgr_t *self, proto_vsf_stream_cfg_t *cfg)
{
    vsf_stream_mgr_t *mgr       = self;
    vsf_stream_mgr_priv_t *priv = mgr->priv;

    if (cfg->enable) {
        if (!priv->fifo[cfg->id]) {
            mfifo_init_t init = {
                .type = MEDIA_VIDEO_STREAM,
                .chn = priv->info->caps[cfg->id].chn,
                .subchn = priv->info->caps[cfg->id].subchn,
            };
            priv->fifo[cfg->id] = mfifo_create(&init, 4 * 1024 * 1024);
            assert(priv->fifo[cfg->id]);
        }
    } else {
        if (priv->fifo[cfg->id]) {
            priv->fifo[cfg->id]->destroy(priv->fifo[cfg->id]);
            priv->fifo[cfg->id] = NULL;
        }
    }

    vsf_stream_cb_t cb = { NULL, NULL };
    if (cfg->enable) {
        cb.args = &priv->info->cfgs[cfg->id];
        cb.func = __vsf_stream_proc;
    }

    vsf_venc_t *venc = VSF_createVenc(cfg->id);
    if (venc && venc->regcallback) {
        venc->regcallback(venc, &cb);
    }
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

    return priv->info->num.num;
}

vsf_stream_mgr_t *vsf_createStreamMgr()
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
    assert(mgr);
    memset(mgr, 0, sizeof(vsf_stream_mgr_t));
    mgr->priv    = priv;
    mgr->destroy = __vsf_stream_destroy;
    mgr->get     = __vsf_stream_get;
    mgr->set     = __vsf_stream_set;
    mgr->num     = __vsf_stream_num;
    mgr->cap     = __vsf_stream_cap;

    s_mgr = mgr;
    return mgr;
}