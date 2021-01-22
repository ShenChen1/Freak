#include "vsf/frame_mgr.h"
#include "inc/cfg.h"
#include "inc/hal/sys.h"
#include "inc/hal/venc.h"
#include "inc/hal/vi.h"
#include "inc/hal/vpss.h"
#include "log.h"
#include "mm.h"
#include "proto.h"
#include "ufifo.h"

typedef struct {
    ufifo_t *fifo[VSF_FRAME_MAX];
    proto_vsf_frame_t *info;
} vsf_frame_mgr_priv_t;

static vsf_frame_mgr_t *s_mgr = NULL;

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

    tracef("size:%u", size);
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

    tracef("tag:%x", tag);
    return tag;
}

static unsigned int recput(unsigned char *p1, unsigned int n1, unsigned char *p2, void *arg)
{
    video_frame_t *frame = arg;
    size_t totalsize     = sizeof(media_record_t) + sizeof(video_frame_t) + (frame->u64ExtVirAddr[0] - frame->u64HeaderVirAddr[0]);
    media_record_t rec   = {};
    unsigned int a = 0, l = 0, _n1 = n1;
    unsigned char *p = NULL, *_p1 = p1, *_p2 = p2;

    rec.size = totalsize;
    rec.tag  = (0xdeadbeef << 8 | 1);
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

    // copy info
    p = (unsigned char *)(frame);
    a = sizeof(video_frame_t);
    l = min(a, _n1);
    memcpy(_p1, p, l);
    memcpy(_p2, p + l, a - l);
    _n1 -= l;
    _p1 += l;
    _p2 += a - l;

    // copy data
    uint32_t uSize = frame->u64ExtVirAddr[0] - frame->u64HeaderVirAddr[0];
    uint8_t *pMap = physmap(frame->u64PhyAddr[0], uSize);
    p = pMap;
    a = uSize;
    l = min(a, _n1);
    memcpy(_p1, p, l);
    memcpy(_p2, p + l, a - l);
    _n1 -= l;
    _p1 += l;
    _p2 += a - l;
    physunmap(pMap, uSize);

    tracef("frame:%u", frame->u32TimeRef);
    return totalsize;
}

#ifdef DEBUG
static void __vsf_frame_SaveYUVFile(FILE *pfd, video_frame_t *pVBuf)
{
    uint8_t *pY_map = NULL;
    uint8_t *pC_map = NULL;
    unsigned int w, h;
    uint8_t *pMemContent;
    uint8_t *pTmpBuff = NULL;
    uint64_t phy_addr;
    uint32_t u32Size;
    video_frame_format_e enPixelFormat = pVBuf->enPixelFormat;
    uint32_t u32UvHeight;
    bool bUvInvert = false;

    if (VIDEO_FRAME_FORMAT_YUV420SP_UV == enPixelFormat || VIDEO_FRAME_FORMAT_YUV422SP_UV == enPixelFormat) {
        bUvInvert = true;
    }

    if (VIDEO_FRAME_FORMAT_YUV420SP_VU == enPixelFormat || VIDEO_FRAME_FORMAT_YUV420SP_UV == enPixelFormat) {
        u32Size     = pVBuf->u32Stride[0] * pVBuf->u32Height * 3 / 2;
        u32UvHeight = pVBuf->u32Height / 2;
    } else if (VIDEO_FRAME_FORMAT_YUV422SP_VU == enPixelFormat || VIDEO_FRAME_FORMAT_YUV422SP_UV == enPixelFormat) {
        u32Size     = pVBuf->u32Stride[0] * pVBuf->u32Height * 2;
        u32UvHeight = pVBuf->u32Height;
    } else if (VIDEO_FRAME_FORMAT_Y == enPixelFormat) {
        u32Size     = pVBuf->u32Stride[0] * pVBuf->u32Height;
        u32UvHeight = pVBuf->u32Height;
    } else {
        errorf("This YUV format is not support!");
        return;
    }

    phy_addr = pVBuf->u64PhyAddr[0];
    pY_map   = physmap(phy_addr, u32Size);
    if (NULL == pY_map) {
        errorf("physmap for pY_map fail!!\n");
        return;
    }
    pC_map = pY_map + pVBuf->u32Stride[0] * pVBuf->u32Height;

    fprintf(stderr, "saving......Y......");
    fflush(stderr);
    for (h = 0; h < pVBuf->u32Height; h++) {
        pMemContent = pY_map + h * pVBuf->u32Stride[0];
        fwrite(pMemContent, pVBuf->u32Width, 1, pfd);
    }

    if (VIDEO_FRAME_FORMAT_Y != enPixelFormat) {
        fflush(pfd);
        fprintf(stderr, "U......");
        fflush(stderr);

        pTmpBuff = (uint8_t *)malloc(pVBuf->u32Stride[0]);
        if (NULL == pTmpBuff) {
            errorf("malloc pTmpBuff (size=%d) fail!!!\n", pVBuf->u32Stride[0]);
            return;
        }
        for (h = 0; h < u32UvHeight; h++) {
            pMemContent = pC_map + h * pVBuf->u32Stride[1];
            if (!bUvInvert) {
                pMemContent += 1;
            }

            for (w = 0; w < pVBuf->u32Width / 2; w++) {
                pTmpBuff[w] = *pMemContent;
                pMemContent += 2;
            }
            fwrite(pTmpBuff, pVBuf->u32Width / 2, 1, pfd);
        }
        fflush(pfd);

        fprintf(stderr, "V......");
        fflush(stderr);
        for (h = 0; h < u32UvHeight; h++) {
            pMemContent = pC_map + h * pVBuf->u32Stride[1];
            if (bUvInvert) {
                pMemContent += 1;
            }

            for (w = 0; w < pVBuf->u32Width / 2; w++) {
                pTmpBuff[w] = *pMemContent;
                pMemContent += 2;
            }
            fwrite(pTmpBuff, pVBuf->u32Width / 2, 1, pfd);
        }
        free(pTmpBuff);
        pTmpBuff = NULL;
    }
    fflush(pfd);

    fprintf(stderr, "done %d!\n", pVBuf->u32TimeRef);
    fflush(stderr);

    physunmap(pY_map, u32Size);
    pY_map = NULL;
}
#endif

static int __vsf_get_frame_proc(void *data, void *args)
{
    video_frame_t *frame       = data;
    size_t totalsize           = sizeof(media_record_t) + sizeof(video_frame_t) + (frame->u64ExtVirAddr[0] - frame->u64HeaderVirAddr[0]);
    vsf_frame_mgr_priv_t *priv = s_mgr->priv;
    proto_vsf_frame_cfg_t *cfg = args;

#ifdef DEBUG
    /* Obtain current time. */
    char acFile[128];
    struct timeval tv;
    struct tm *timeinfo;
    gettimeofday(&tv, NULL);
    timeinfo = localtime(&tv.tv_sec);
    snprintf(acFile,
             sizeof(acFile),
             "frame_chn%d_%4d_%02d_%02d_%02d_%02d_%02d_%06ld.yuv",
             priv->info->caps[cfg->id].chn,
             1900 + timeinfo->tm_year,
             1 + timeinfo->tm_mon,
             timeinfo->tm_mday,
             timeinfo->tm_hour,
             timeinfo->tm_min,
             timeinfo->tm_sec,
             tv.tv_usec);
    FILE *pFile = fopen(acFile, "wb");
    __vsf_frame_SaveYUVFile(pFile, frame);
    fclose(pFile);
#endif

    return ufifo_put(priv->fifo[cfg->id], frame, totalsize) != totalsize;
}

static int __vsf_frame_destroy(vsf_frame_mgr_t *self)
{
    // never destroy
    return 0;
}

static int __vsf_frame_set(vsf_frame_mgr_t *self, proto_vsf_frame_cfg_t *cfg)
{
    int ret;
    vsf_frame_mgr_t *mgr       = self;
    vsf_frame_mgr_priv_t *priv = mgr->priv;

    if (cfg->enable) {
        char name[64];
        ufifo_init_t init = {
            .lock  = UFIFO_LOCK_NONE,
            .opt   = UFIFO_OPT_ALLOC,
            .alloc = { 3 * cfg->width * cfg->height * cfg->fps * 2 + 1024 },
            .hook  = { recsize, rectag, recput },
        };

        if (!priv->fifo[cfg->id]) {
            snprintf(name,
                     sizeof(name),
                     PROTO_VSF_FRAME_FIFO "%d-%d",
                     priv->info->caps[cfg->id].chn,
                     priv->info->caps[cfg->id].subchn);
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

    vsf_vpss_t *vpss = VSF_createVpss(priv->info->caps[cfg->id].chn);
    if (vpss && vpss->ctrl) {
        vsf_frame_cfg_t _cfg = {
            .format = cfg->format,
            .width = cfg->width,
            .height = cfg->height,
            .fps = cfg->fps,
        };
        vpss->ctrl(vpss, priv->info->caps[cfg->id].subchn, &_cfg);
    }

    vsf_frame_cb_t cb = {};
    if (cfg->enable) {
        cb.args  = &priv->info->cfgs[cfg->id];
        cb.func  = __vsf_get_frame_proc;
    }

    if (vpss && vpss->regcallback) {
        vpss->regcallback(vpss, priv->info->caps[cfg->id].subchn, &cb);
    }

    return 0;
}

static int __vsf_frame_get(vsf_frame_mgr_t *self, proto_vsf_frame_cfg_t *cfg)
{
    vsf_frame_mgr_t *mgr       = self;
    vsf_frame_mgr_priv_t *priv = mgr->priv;

    *cfg = priv->info->cfgs[cfg->id];
    return 0;
}

static int __vsf_frame_cap(vsf_frame_mgr_t *self, proto_vsf_frame_cap_t *cap)
{
    vsf_frame_mgr_t *mgr       = self;
    vsf_frame_mgr_priv_t *priv = mgr->priv;

    *cap = priv->info->caps[cap->id];
    return 0;
}

static int __vsf_frame_num(vsf_frame_mgr_t *self)
{
    vsf_frame_mgr_t *mgr       = self;
    vsf_frame_mgr_priv_t *priv = mgr->priv;

    return priv->info->num.num;
}

vsf_frame_mgr_t *vsf_createFrameMgr()
{
    vsf_frame_mgr_t *mgr       = s_mgr;
    vsf_frame_mgr_priv_t *priv = NULL;

    if (mgr) {
        return mgr;
    }

    priv = malloc(sizeof(vsf_frame_mgr_priv_t));
    assert(priv);
    memset(priv, 0, sizeof(vsf_frame_mgr_priv_t));
    priv->info = cfg_get_member(frame);

    mgr = malloc(sizeof(vsf_frame_mgr_t));
    assert(mgr);
    memset(mgr, 0, sizeof(vsf_frame_mgr_t));
    mgr->priv    = priv;
    mgr->destroy = __vsf_frame_destroy;
    mgr->get     = __vsf_frame_get;
    mgr->set     = __vsf_frame_set;
    mgr->cap     = __vsf_frame_cap;
    mgr->num     = __vsf_frame_num;

    s_mgr = mgr;
    return mgr;
}
