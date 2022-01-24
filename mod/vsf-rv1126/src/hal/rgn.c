#include "log.h"
#include "common.h"
#include "media.h"
#include "inc/hal/rgn.h"
#include "inc/sdk_cfg.h"
#include "rkmedia_api.h"

#define CEILING_2_POWER(x,a)     ( ((x) + ((a) - 1) ) & ( ~((a) - 1) ) )

typedef enum {
    VSF_RGN_NONE,
    VSF_RGN_CREATE,
    VSF_RGN_ATTACH,
} vsf_rgn_status_e;

typedef struct {
    int id; 
    int status;

    BITMAP_S BitMap;

    COVER_INFO_S CoverInfo;

    OSD_REGION_INFO_S RngInfo;

} vsf_rgn_priv_t;

typedef struct {
    int num;
    vsf_rgn_t **objs;
} vsf_rgn_mod_t;

static vsf_rgn_mod_t s_mod;

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
    RK_S32 s32Ret        = 0;
    vsf_rgn_t *obj       = self;
    vsf_rgn_priv_t *priv = obj->priv;

    vsf_rgn_cfg_t *cfg = (vsf_rgn_cfg_t *)param;

    uint32_t PicWidth = sdk_cfg_get_member(astVencInfo[cfg->chn])->PicWidth ;
    uint32_t PicHeight = sdk_cfg_get_member(astVencInfo[cfg->chn])->PicHeight ;

    vsf_rgn_point_t points[4];
    for(int i = 0 ; i < ARRAY_SIZE(cfg->cover.points); i++)
    {
        points[i].x = cfg->cover.points[i].x * PicWidth / 8192;
        points[i].y = cfg->cover.points[i].y * PicHeight / 8192;
    }

    uint32_t realWidth  = CEILING_2_POWER((points[1].x - points[0].x) , 16);
    uint32_t realHeight = CEILING_2_POWER((points[1].y - points[0].y) , 16);

    // set cover info
    priv->CoverInfo.enPixelFormat = PIXEL_FORMAT_ARGB_8888;
    priv->CoverInfo.u32Color = cfg->cover.color;

    // set rgn info
    priv->RngInfo.enRegionId = cfg->layer > 7 ? REGION_ID_7 : cfg->layer;
    priv->RngInfo.u32PosX = CEILING_2_POWER(points[0].x , 16);
    priv->RngInfo.u32PosY = CEILING_2_POWER(points[0].y , 16);
    priv->RngInfo.u32Width = realWidth;
    priv->RngInfo.u32Height = realHeight;
    priv->RngInfo.u8Enable = cfg->enable;
    priv->RngInfo.u8Inverse = 0;

    // create;
    if (priv->status < VSF_RGN_CREATE) {
        s32Ret = RK_MPI_VENC_RGN_Init(cfg->chn, NULL);
        if (0 != s32Ret) {
            errorf("RK_MPI_VENC_RGN_Init faild with %d!", s32Ret);
        }
        priv->status = VSF_RGN_CREATE;
    }

    // set cover
    s32Ret = RK_MPI_VENC_RGN_SetCover(cfg->chn, &priv->RngInfo, &priv->CoverInfo);
    if (0 != s32Ret) {
            errorf("RK_MPI_VENC_RGN_SetBitMap faild with %d!", s32Ret);
    }
    return s32Ret;
}

static int __rgn_ctrl_bitmap(vsf_rgn_t *self, void *param)
{
    RK_S32 s32Ret        = 0;

    vsf_rgn_t *obj       = self;
    vsf_rgn_priv_t *priv = obj->priv;
    vsf_rgn_cfg_t *cfg = (vsf_rgn_cfg_t *)param;

    uint32_t Width = CEILING_2_POWER(sdk_cfg_get_member(astVencInfo[cfg->chn])->PicWidth , 16);
    uint32_t Height = CEILING_2_POWER(sdk_cfg_get_member(astVencInfo[cfg->chn])->PicHeight , 16);

    // set bitmap
    priv->BitMap.enPixelFormat = PIXEL_FORMAT_ARGB_8888;
    priv->BitMap.u32Width = Width;
    priv->BitMap.u32Height = Height;
    priv->BitMap.pData = malloc(Width * Height * 4);
    memset(priv->BitMap.pData , 0 , Width * Height * 4);

    vsf_rgn_bitmap_t bitmap = {
        .enPixelFmt = VIDEO_FRAME_FORMAT_ARGB32_8888,
        .u32Width   = Width,
        .u32Height  = Height,
        .u32Stride  = Width * 4,
        .pData      = priv->BitMap.pData,
    };
    cfg->bitmap.proc(&bitmap, cfg->bitmap.args);

    // set rgn info
    priv->RngInfo.enRegionId = cfg->layer > 7 ? REGION_ID_7 : cfg->layer;
    priv->RngInfo.u32PosX = 0;
    priv->RngInfo.u32PosY = 0;
    priv->RngInfo.u32Width = Width;
    priv->RngInfo.u32Height = Height;
    priv->RngInfo.u8Enable = cfg->enable;
    priv->RngInfo.u8Inverse = 0;

    // create;
    if (priv->status < VSF_RGN_CREATE) {
        s32Ret = RK_MPI_VENC_RGN_Init(cfg->chn, NULL);
        if (0 != s32Ret) {
            errorf("RK_MPI_VENC_RGN_Init faild with %d!", s32Ret);
        }
        priv->status = VSF_RGN_CREATE;
    }

    // set bitmap
    s32Ret = RK_MPI_VENC_RGN_SetBitMap(cfg->chn, &priv->RngInfo, &priv->BitMap);
    if (0 != s32Ret) {
            errorf("RK_MPI_VENC_RGN_SetBitMap faild with %d!", s32Ret);
    }
    free(priv->BitMap.pData);
    return s32Ret;
}

static int  __rgn_ctrl(vsf_rgn_t *self, vsf_rgn_cfg_t *cfg)
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

    mod->num  = 8;
    mod->objs = calloc(mod->num, sizeof(vsf_rgn_t *));

    assert(mod->objs);

}

static void __attribute__((destructor(VSF_RGN_PRIORITY))) sdk_rgn_destructor()
{
    vsf_rgn_mod_t *mod = &s_mod;
    free(mod->objs);
    mod->num = 0;
}