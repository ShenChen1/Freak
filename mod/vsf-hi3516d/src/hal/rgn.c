#include "inc/hal/rgn.h"
#include "common.h"
#include "inc/sdk_cfg.h"
#include "log.h"
#include "proto_vsf.h"

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

static unsigned short argb8888_1555(unsigned int color)
{
    unsigned char a = (color >> 24) & 0xff;
    unsigned char r = (color >> 16) & 0xff;
    unsigned char g = (color >> 8) & 0xff;
    unsigned char b = color & 0xff;

    a = a ? 1 : 0;
    r >>= 3;
    g >>= 3;
    b >>= 3;

    return (unsigned short)(a << 15 | r << 10 | g << 5 | b);
}

static unsigned int argb8888_888(unsigned int color)
{
    return color & 0xffffff;
}

static int __rgn_ctrl_mask(vsf_rgn_t *self, int chn, void *param)
{
    int i;
    HI_S32 s32Ret            = 0;
    vsf_rgn_t *obj           = self;
    vsf_rgn_priv_t *priv     = obj->priv;
    proto_vsf_osd_cfg_t *cfg = param;

    priv->stChn.enModId  = HI_ID_VPSS;
    priv->stChn.s32DevId = chn; // VpssGrp
    priv->stChn.s32ChnId = 0;

    VPSS_CHN_ATTR_S stChnAttr;
    s32Ret = HI_MPI_VPSS_GetChnAttr(priv->stChn.s32DevId, priv->stChn.s32ChnId, &stChnAttr);
    assert(s32Ret == HI_SUCCESS);

    priv->stRegion.enType = COVER_RGN;

    priv->stChnAttr.bShow                                      = cfg->enable;
    priv->stChnAttr.enType                                     = COVER_RGN;
    priv->stChnAttr.unChnAttr.stCoverChn.enCoverType           = AREA_QUAD_RANGLE;
    priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.bSolid   = HI_TRUE;
    priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.u32Thick = 2;
    for (i = 0; i < 4; i++) {
        cfg->info.mask.points[i].x = max(cfg->info.mask.points[i].x, 0);
        cfg->info.mask.points[i].x = min(cfg->info.mask.points[i].x, 8192);
        priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[i].s32X =
            cfg->info.mask.points[i].x * stChnAttr.u32Width / 8192;
        priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[i].s32X =
            ALIGN_UP(priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[i].s32X, 2);

        cfg->info.mask.points[i].y = max(cfg->info.mask.points[i].y, 0);
        cfg->info.mask.points[i].y = min(cfg->info.mask.points[i].y, 8192);
        priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[i].s32Y =
            cfg->info.mask.points[i].y * stChnAttr.u32Height / 8192;
        priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[i].s32Y =
            ALIGN_UP(priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[i].s32Y, 2);
    }
    priv->stChnAttr.unChnAttr.stCoverChn.u32Layer = 0;
    priv->stChnAttr.unChnAttr.stCoverChn.u32Color = argb8888_888(cfg->info.mask.color); // PIXEL_FORMAT_RGB_888

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

static int __rgn_ctrl_objs(vsf_rgn_t *self, int chn, void *param)
{
    HI_S32 s32Ret            = 0;
    vsf_rgn_t *obj           = self;
    vsf_rgn_priv_t *priv     = obj->priv;
    proto_vsf_osd_tgr_t *tgr = param;

    priv->stChn.enModId  = HI_ID_VENC;
    priv->stChn.s32DevId = 0;
    priv->stChn.s32ChnId = chn;

    VENC_CHN_ATTR_S stChnAttr;
    s32Ret = HI_MPI_VENC_GetChnAttr(priv->stChn.s32ChnId, &stChnAttr);
    assert(s32Ret == HI_SUCCESS);

    priv->stRegion.enType                            = OVERLAY_RGN;
    priv->stRegion.unAttr.stOverlay.enPixelFmt       = PIXEL_FORMAT_ARGB_1555;
    priv->stRegion.unAttr.stOverlay.u32BgColor       = 0x00000000;
    priv->stRegion.unAttr.stOverlay.stSize.u32Width  = stChnAttr.stVencAttr.u32PicWidth;
    priv->stRegion.unAttr.stOverlay.stSize.u32Height = stChnAttr.stVencAttr.u32PicHeight;
    priv->stRegion.unAttr.stOverlay.u32CanvasNum     = 2;

    priv->stChnAttr.bShow                               = tgr->info.objs.num > 0;
    priv->stChnAttr.enType                              = OVERLAY_RGN;
    priv->stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = 0;
    priv->stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = 0;
    priv->stChnAttr.unChnAttr.stOverlayChn.u32Layer     = 4;
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
#if 1
    {
        int n, i;
        uint16_t color  = argb8888_1555(0x01FF0000);
        uint16_t *data  = (uint16_t *)(size_t)stRgnCanvasInfo.u64VirtAddr;
        uint32_t width  = stRgnCanvasInfo.stSize.u32Width;
        uint32_t height = stRgnCanvasInfo.stSize.u32Height;
        uint32_t stride = stRgnCanvasInfo.u32Stride;
        memset(data, 0, stRgnCanvasInfo.stSize.u32Height * stRgnCanvasInfo.u32Stride);

        for (n = 0; n < tgr->info.objs.num; n++) {

            tgr->info.objs.rects[n].x = max(tgr->info.objs.rects[n].x, 0);
            tgr->info.objs.rects[n].x = min(tgr->info.objs.rects[n].x, 8192);
            tgr->info.objs.rects[n].y = max(tgr->info.objs.rects[n].y, 0);
            tgr->info.objs.rects[n].y = min(tgr->info.objs.rects[n].y, 8192);
            tgr->info.objs.rects[n].w = max(tgr->info.objs.rects[n].w, 0);
            tgr->info.objs.rects[n].w = min(tgr->info.objs.rects[n].w, 8192 - tgr->info.objs.rects[n].x);
            tgr->info.objs.rects[n].h = max(tgr->info.objs.rects[n].h, 0);
            tgr->info.objs.rects[n].h = min(tgr->info.objs.rects[n].h, 8192 - tgr->info.objs.rects[n].y);

            int box_x     = tgr->info.objs.rects[n].x * width / 8192;
            int box_y     = tgr->info.objs.rects[n].y * height / 8192;
            int box_w     = tgr->info.objs.rects[n].w * width / 8192;
            int box_h     = tgr->info.objs.rects[n].h * height / 8192;
            uint16_t *box = &data[box_y * stride / 2 + box_x];

            debugf("[%d]: %d %d %d %d", n, box_x, box_y, box_w, box_h);
            if (box_w * box_h == 0) {
                continue;
            }

            for (i = 0; i < box_w; i++) {
                box[0 * stride / 2 + i]           = color;
                box[1 * stride / 2 + i]           = color;
                box[2 * stride / 2 + i]           = color;
                box[3 * stride / 2 + i]           = color;
                box[(box_h - 1) * stride / 2 + i] = color;
                box[(box_h - 2) * stride / 2 + i] = color;
                box[(box_h - 3) * stride / 2 + i] = color;
                box[(box_h - 4) * stride / 2 + i] = color;
            }
            for (i = 0; i < box_h; i++) {
                box[i * stride / 2 + 0]           = color;
                box[i * stride / 2 + 1]           = color;
                box[i * stride / 2 + 2]           = color;
                box[i * stride / 2 + 3]           = color;
                box[i * stride / 2 + (box_w - 1)] = color;
                box[i * stride / 2 + (box_w - 2)] = color;
                box[i * stride / 2 + (box_w - 3)] = color;
                box[i * stride / 2 + (box_w - 4)] = color;
            }
        }
    }
#endif
    s32Ret |= HI_MPI_RGN_UpdateCanvas(priv->id);
    if (HI_SUCCESS != s32Ret) {
        errorf("HI_MPI_RGN_UpdateCanvas faild with%#x!", s32Ret);
    }

    return s32Ret;
}

int __rgn_ctrl(vsf_rgn_t *self, int chn, void *param)
{
    vsf_rgn_t *obj           = self;
    vsf_rgn_priv_t *priv     = obj->priv;
    proto_vsf_osd_cfg_t *cfg = param;

    assert(priv->id == cfg->id);
    if (!strncmp(cfg->info.condition, "mask", sizeof("mask"))) {
        return __rgn_ctrl_mask(self, chn, param);
    } else if (!strncmp(cfg->info.condition, "text", sizeof("text"))) {
        return -1;
    }

    return -1;
}

int __rgn_trigger(vsf_rgn_t *self, int chn, void *param)
{
    vsf_rgn_t *obj           = self;
    vsf_rgn_priv_t *priv     = obj->priv;
    proto_vsf_osd_tgr_t *tgr = param;

    assert(priv->id == tgr->id);
    if (!strncmp(tgr->info.condition, "objs", sizeof("objs"))) {
        return __rgn_ctrl_objs(self, chn, param);
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
    obj->trigger = __rgn_trigger;

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