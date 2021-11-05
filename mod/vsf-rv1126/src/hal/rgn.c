#include "log.h"
#include "common.h"
#include "media.h"
#include "inc/hal/rgn.h"
#include "inc/sdk_cfg.h"

typedef enum {
    VSF_RGN_NONE,
    VSF_RGN_CREATE,
    VSF_RGN_ATTACH,
} vsf_rgn_status_e;

typedef struct {
    int id;
    int status;

    // create;
    RGN_ATTR_S stRegion;
    // attach;
    MPP_CHN_S stChn;
    // modify;
    RGN_CHN_ATTR_S stChnAttr;
} vsf_rgn_priv_t;

typedef struct {
    int num;
    vsf_rgn_t **objs;
} vsf_rgn_mod_t;

static vsf_rgn_mod_t s_mod;

////////////////////////////////////////////////////////////////////////////////////////////////////

static inline unsigned int argb8888_888(unsigned int color)
{
    return color & 0xffffff;
}

static int __rgn_init(vsf_rgn_t *self)
{
    return 0;
}

static int __rgn_destroy(vsf_rgn_t *self)
{
    vsf_rgn_mod_t *mod   = &s_mod;
    vsf_rgn_t *obj       = self;
    vsf_rgn_priv_t *priv = obj->priv;

    mod->objs[priv->id] = NULL;
    free(priv);
    free(obj);

    return 0;
}

static int __rgn_ctrl_cover(vsf_rgn_t *self, void *param)
{
    int i;
    HI_S32 s32Ret        = 0;
    vsf_rgn_t *obj       = self;
    vsf_rgn_priv_t *priv = obj->priv;
    vsf_rgn_cfg_t *cfg   = param;

    priv->stChn.enModId  = HI_ID_VPSS;
    priv->stChn.s32DevId = sdk_cfg_get_member(astVpssInfo[cfg->chn])->VpssGrp;
    priv->stChn.s32ChnId = sdk_cfg_get_member(astVpssInfo[cfg->chn])->ViPipe;

    VPSS_CHN_ATTR_S stChnAttr;
    s32Ret = HI_MPI_VPSS_GetChnAttr(priv->stChn.s32DevId, priv->stChn.s32ChnId, &stChnAttr);
    assert(s32Ret == HI_SUCCESS);

    priv->stRegion.enType = COVER_RGN;

    priv->stChnAttr.bShow                                      = cfg->enable;
    priv->stChnAttr.enType                                     = COVER_RGN;
    priv->stChnAttr.unChnAttr.stCoverChn.enCoverType           = AREA_QUAD_RANGLE;
    priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.bSolid   = HI_TRUE;
    priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.u32Thick = 2;
    for (i = 0; i < ARRAY_SIZE(cfg->cover.points); i++) {
        cfg->cover.points[i].x = range(cfg->cover.points[i].x, 0, 8191);
        priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[i].s32X =
            cfg->cover.points[i].x * stChnAttr.u32Width / 8192;
        priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[i].s32X =
            ALIGN_UP(priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[i].s32X, 2);

        cfg->cover.points[i].y = range(cfg->cover.points[i].y, 0, 8191);
        priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[i].s32Y =
            cfg->cover.points[i].y * stChnAttr.u32Height / 8192;
        priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[i].s32Y =
            ALIGN_UP(priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[i].s32Y, 2);
    }
    priv->stChnAttr.unChnAttr.stCoverChn.u32Layer = cfg->layer;
    priv->stChnAttr.unChnAttr.stCoverChn.u32Color = argb8888_888(cfg->cover.color); // PIXEL_FORMAT_RGB_888

    // create;
    if (priv->status < VSF_RGN_CREATE) {
        s32Ret |= HI_MPI_RGN_Create(priv->id, &priv->stRegion);
        if (HI_SUCCESS != s32Ret) {
            errorf("HI_MPI_RGN_Create faild with%#x!", s32Ret);
        }
        priv->status = VSF_RGN_CREATE;
    }
    // attach;
    if (priv->status < VSF_RGN_ATTACH) {
        s32Ret |= HI_MPI_RGN_AttachToChn(priv->id, &priv->stChn, &priv->stChnAttr);
        if (HI_SUCCESS != s32Ret) {
            errorf("HI_MPI_RGN_AttachToChn faild with%#x!", s32Ret);
        }
        priv->status = VSF_RGN_ATTACH;
    }
    // display;
    s32Ret |= HI_MPI_RGN_SetDisplayAttr(priv->id, &priv->stChn, &priv->stChnAttr);
    if (HI_SUCCESS != s32Ret) {
        errorf("HI_MPI_RGN_SetDisplayAttr faild with%#x!", s32Ret);
    }

    return s32Ret;
}

static int __rgn_ctrl_bitmap(vsf_rgn_t *self, void *param)
{
    HI_S32 s32Ret        = 0;
    vsf_rgn_t *obj       = self;
    vsf_rgn_priv_t *priv = obj->priv;
    vsf_rgn_cfg_t *cfg   = param;

    priv->stChn.enModId  = HI_ID_VENC;
    priv->stChn.s32DevId = 0;
    priv->stChn.s32ChnId = sdk_cfg_get_member(astVencInfo[cfg->chn])->VencChn;

    VENC_CHN_ATTR_S stChnAttr;
    s32Ret = HI_MPI_VENC_GetChnAttr(priv->stChn.s32ChnId, &stChnAttr);
    assert(s32Ret == HI_SUCCESS);

    priv->stRegion.enType                            = OVERLAY_RGN;
    priv->stRegion.unAttr.stOverlay.enPixelFmt       = PIXEL_FORMAT_ARGB_1555;
    priv->stRegion.unAttr.stOverlay.u32BgColor       = 0x00000000;
    priv->stRegion.unAttr.stOverlay.stSize.u32Width  = stChnAttr.stVencAttr.u32PicWidth;
    priv->stRegion.unAttr.stOverlay.stSize.u32Height = stChnAttr.stVencAttr.u32PicHeight;
    priv->stRegion.unAttr.stOverlay.u32CanvasNum     = 2;

    priv->stChnAttr.bShow                               = HI_TRUE;
    priv->stChnAttr.enType                              = OVERLAY_RGN;
    priv->stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = 0;
    priv->stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = 0;
    priv->stChnAttr.unChnAttr.stOverlayChn.u32Layer     = cfg->layer;
    priv->stChnAttr.unChnAttr.stOverlayChn.u32BgAlpha   = 0;
    priv->stChnAttr.unChnAttr.stOverlayChn.u32FgAlpha   = 128;

    // create;
    if (priv->status < VSF_RGN_CREATE) {
        s32Ret |= HI_MPI_RGN_Create(priv->id, &priv->stRegion);
        if (HI_SUCCESS != s32Ret) {
            errorf("HI_MPI_RGN_Create faild with%#x!", s32Ret);
        }
        s32Ret |= HI_MPI_RGN_SetAttr(priv->id, &priv->stRegion);
        if (HI_SUCCESS != s32Ret) {
            errorf("HI_MPI_RGN_SetAttr faild with%#x!", s32Ret);
        }
        priv->status = VSF_RGN_CREATE;
    }
    // attach;
    if (priv->status < VSF_RGN_ATTACH) {
        s32Ret |= HI_MPI_RGN_AttachToChn(priv->id, &priv->stChn, &priv->stChnAttr);
        if (HI_SUCCESS != s32Ret) {
            errorf("HI_MPI_RGN_AttachToChn faild with%#x!", s32Ret);
        }
        priv->status = VSF_RGN_ATTACH;
    }

    RGN_CANVAS_INFO_S stRgnCanvasInfo = {};
    s32Ret |= HI_MPI_RGN_GetCanvasInfo(priv->id, &stRgnCanvasInfo);
    if (HI_SUCCESS != s32Ret) {
        errorf("HI_MPI_RGN_GetCanvasInfo faild with%#x!", s32Ret);
    }

    vsf_rgn_bitmap_t bitmap = {
        .enPixelFmt = VIDEO_FRAME_FORMAT_ABGR16_1555,
        .u32Width = stRgnCanvasInfo.stSize.u32Width,
        .u32Height = stRgnCanvasInfo.stSize.u32Height,
        .u32Stride = stRgnCanvasInfo.u32Stride,
        .pData = (uint16_t *)(size_t)stRgnCanvasInfo.u64VirtAddr,
    };
    cfg->bitmap.proc(&bitmap, cfg->bitmap.args);

    s32Ret |= HI_MPI_RGN_UpdateCanvas(priv->id);
    if (HI_SUCCESS != s32Ret) {
        errorf("HI_MPI_RGN_UpdateCanvas faild with%#x!", s32Ret);
    }

    return s32Ret;
}

static int __rgn_ctrl(vsf_rgn_t *self, vsf_rgn_cfg_t *cfg)
{
    if (cfg->type == VSF_RGN_COVER) {
        return __rgn_ctrl_cover(self, cfg);
    } else if (cfg->type == VSF_RGN_BITMAP) {
        return __rgn_ctrl_bitmap(self, cfg);
    }

    return -1;
}

vsf_rgn_t *VSF_createRgn(int id)
{
    vsf_rgn_mod_t *mod   = &s_mod;
    vsf_rgn_t *obj       = NULL;
    vsf_rgn_priv_t *priv = NULL;

    if (id >= mod->num) {
        return NULL;
    }

    obj = mod->objs[id];
    if (obj) {
        return obj;
    }

    priv = malloc(sizeof(vsf_rgn_priv_t));
    assert(priv);
    memset(priv, 0, sizeof(vsf_rgn_priv_t));
    priv->id     = id;
    priv->status = VSF_RGN_NONE;

    obj = malloc(sizeof(vsf_rgn_t));
    assert(obj);
    memset(obj, 0, sizeof(vsf_rgn_t));
    obj->priv    = priv;
    obj->init    = __rgn_init;
    obj->destroy = __rgn_destroy;
    obj->ctrl    = __rgn_ctrl;

    mod->objs[id] = obj;
    return obj;
}

int VSF_getRgnNum(void)
{
    vsf_rgn_mod_t *mod = &s_mod;
    return mod->num;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

static void __attribute__((constructor(VSF_RGN_PRIORITY))) sdk_rgn_constructor()
{
    vsf_rgn_mod_t *mod = &s_mod;

    mod->num  = RGN_HANDLE_MAX;
    mod->objs = calloc(mod->num, sizeof(vsf_rgn_t *));
    assert(mod->objs);
}

static void __attribute__((destructor(VSF_RGN_PRIORITY))) sdk_rgn_destructor()
{
    vsf_rgn_mod_t *mod = &s_mod;

    free(mod->objs);
    mod->num = 0;
}