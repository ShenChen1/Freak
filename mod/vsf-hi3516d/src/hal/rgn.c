#include "inc/hal/rgn.h"
#include "common.h"
#include "inc/sdk_cfg.h"
#include "log.h"

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

static int __rgn_ctrl_mask(vsf_rgn_t *self, void *param) {
    HI_S32 s32Ret        = 0;
    vsf_rgn_t *obj       = self;
    vsf_rgn_priv_t *priv = obj->priv;
    proto_vsf_osd_t *osd = param;

    priv->stRegion.enType = COVER_RGN;

    priv->stChn.enModId  = HI_ID_VPSS;
    priv->stChn.s32DevId = osd->chn;
    priv->stChn.s32ChnId = 0;

    VPSS_CHN_ATTR_S stChnAttr;
    s32Ret = HI_MPI_VPSS_GetChnAttr(osd->chn, 0, &stChnAttr);
    assert(s32Ret == HI_SUCCESS);

    priv->stChnAttr.bShow                                             = osd->enable;
    priv->stChnAttr.enType                                            = COVER_RGN;
    priv->stChnAttr.unChnAttr.stCoverChn.enCoverType                  = AREA_QUAD_RANGLE;
    priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.bSolid          = HI_TRUE;
    priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.u32Thick        = 2;
    priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[0].s32X = osd->info.mask.points[0].x * stChnAttr.u32Width / 8192;
    priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[0].s32X = ALIGN_UP(priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[0].s32X, 2);
    priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[0].s32Y = osd->info.mask.points[0].y * stChnAttr.u32Height / 8192;
    priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[0].s32Y = ALIGN_UP(priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[0].s32Y, 2);
    priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[1].s32X = osd->info.mask.points[1].x * stChnAttr.u32Width / 8192;
    priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[1].s32X = ALIGN_UP(priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[1].s32X, 2);
    priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[1].s32Y = osd->info.mask.points[1].y * stChnAttr.u32Height / 8192;
    priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[1].s32Y = ALIGN_UP(priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[1].s32Y, 2);
    priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[2].s32X = osd->info.mask.points[2].x * stChnAttr.u32Width / 8192;
    priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[2].s32X = ALIGN_UP(priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[2].s32X, 2);
    priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[2].s32Y = osd->info.mask.points[2].y * stChnAttr.u32Height / 8192;
    priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[2].s32Y = ALIGN_UP(priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[2].s32Y, 2);
    priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[3].s32X = osd->info.mask.points[3].x * stChnAttr.u32Width / 8192;
    priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[3].s32X = ALIGN_UP(priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[3].s32X, 2);
    priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[3].s32Y = osd->info.mask.points[3].y * stChnAttr.u32Height / 8192;
    priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[3].s32Y = ALIGN_UP(priv->stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[3].s32Y, 2);
    priv->stChnAttr.unChnAttr.stCoverChn.u32Layer                     = priv->id;
    priv->stChnAttr.unChnAttr.stCoverChn.u32Color                     = osd->info.mask.color;

    // create;
    if (priv->status < VSF_RGN_CREATE) {
        s32Ret |= HI_MPI_RGN_Create(priv->id, &priv->stRegion);
        priv->status = VSF_RGN_CREATE;
    }
    // attach;
    if (priv->status < VSF_RGN_ATTACH) {
        s32Ret |= HI_MPI_RGN_AttachToChn(priv->id, &priv->stChn, &priv->stChnAttr);
        priv->status = VSF_RGN_ATTACH;
    }
    // display;
    s32Ret |= HI_MPI_RGN_SetDisplayAttr(priv->id, &priv->stChn, &priv->stChnAttr);

    return s32Ret;
}

static int __rgn_ctrl(vsf_rgn_t *self, void *param)
{
    vsf_rgn_t *obj       = self;
    vsf_rgn_priv_t *priv = obj->priv;
    proto_vsf_osd_t *osd = param;

    assert(priv->id == osd->id);
    if (!strncmp(osd->info.condition, "mask", sizeof("mask"))) {
        return __rgn_ctrl_mask(self, param);
    } else if (!strncmp(osd->info.condition, "text", sizeof("text"))) {
        return -1;
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

    obj = malloc(sizeof(vsf_rgn_priv_t));
    if (obj == NULL) {
        return NULL;
    }

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