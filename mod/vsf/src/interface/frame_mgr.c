#include "vsf/frame_mgr.h"
#include "inc/cfg.h"
#include "inc/hal/sys.h"
#include "inc/hal/venc.h"
#include "inc/hal/vi.h"
#include "inc/hal/vpss.h"
#include "log.h"
#include "mm.h"
#include "media.h"
#include "proto.h"

typedef struct {
    mfifo_t *fifo[VSF_FRAME_MAX];
    proto_vsf_frame_t *info;
} vsf_frame_mgr_priv_t;

static vsf_frame_mgr_t *s_mgr = NULL;

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

    return priv->fifo[cfg->id]->put(priv->fifo[cfg->id], frame, totalsize, -1) != totalsize;
}

static int __vsf_frame_destroy(vsf_frame_mgr_t *self)
{
    // never destroy
    return 0;
}

static int __vsf_frame_set(vsf_frame_mgr_t *self, proto_vsf_frame_cfg_t *cfg)
{
    vsf_frame_mgr_t *mgr       = self;
    vsf_frame_mgr_priv_t *priv = mgr->priv;

    if (cfg->enable) {
        if (!priv->fifo[cfg->id]) {
            mfifo_init_t init = {
                .type = MEDIA_VIDEO_FRAME,
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
