/*
 * @Author: your name
 * @Date: 2021-11-09 14:16:38
 * @LastEditTime: 2021-11-15 16:28:46
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \Freak\mod\vsf-hi3516d\src\hal\vpss.c
 */
#include "common.h"
#include "log.h"
#include "media.h"
#include "inc/hal/vpss.h"
#include "inc/sdk_cfg.h"

typedef struct {
    sdk_vpss_info_t *info;
    pthread_t hq_thread;
    int quit;
    int seq;
    vsf_frame_cb_t cb;
} vsf_vpss_priv_t;

typedef struct {
    int num;
    vsf_vpss_t **objs;
} vsf_vpss_mod_t;

static vsf_vpss_mod_t s_mod;



static void __transfor_frame_info(MEDIA_BUFFER mb, video_frame_t *dst)
{
    MB_IMAGE_INFO_S stImageInfo = {0};
    RK_MPI_MB_GetImageInfo(mb, &stImageInfo);
    memset(dst, 0, sizeof(video_frame_t));
    dst->u32Width        = stImageInfo.u32Width;
    dst->u32Height       = stImageInfo.u32Height;
    dst->enPixelFormat   = stImageInfo.enImgType;
    dst->ptr  = RK_MPI_MB_GetPtr(mb);
}

static void *__vpss_get_chn_frame_proc(void *p)
{
    int s32Ret = 0;
    vsf_vpss_priv_t *priv = p;
    MEDIA_BUFFER mb = NULL;
    while (!priv->quit) {
        mb = RK_MPI_SYS_GetMediaBuffer(RK_ID_RGA, priv->info->RgaId, -1);

        if (priv->cb.func) {
            video_frame_t frame;
            __transfor_frame_info(mb, &frame);
            tracef("frame:%u", frame.u32TimeRef);
            s32Ret = priv->cb.func(&frame, priv->cb.args);
            if (0 != s32Ret) {
                errorf("proc frame %d-%d failed!", priv->info->ViChn, priv->info->RgaId);
            }
        }
        RK_MPI_MB_ReleaseBuffer(mb);
        if (0 != s32Ret) {
            errorf("ReleaseChnFrame %d-%d err:0x%x\n", priv->info->ViChn, priv->info->RgaId, s32Ret);
        }
    }

    return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

static int __vpss_init(vsf_vpss_t *self)
{
    int s32Ret;
    vsf_vpss_t *obj       = self;
    vsf_vpss_priv_t *priv = obj->priv;

    RGA_ATTR_S stRgaAttr;
    stRgaAttr.bEnBufPool = RK_TRUE;
    stRgaAttr.u16BufPoolCnt = 1;
    stRgaAttr.u16Rotaion = 0;
    stRgaAttr.stImgIn.u32X = 0;
    stRgaAttr.stImgIn.u32Y = 0;
    stRgaAttr.stImgIn.imgType = sdk_cfg_get_member(astViInfo[priv->info->ViChn])->stChnInfo.enPixFmt;
    stRgaAttr.stImgIn.u32Width = sdk_cfg_get_member(astViInfo[priv->info->ViChn])->stChnInfo.u32Width;
    stRgaAttr.stImgIn.u32Height = sdk_cfg_get_member(astViInfo[priv->info->ViChn])->stChnInfo.u32Height;
    stRgaAttr.stImgIn.u32HorStride = sdk_cfg_get_member(astViInfo[priv->info->ViChn])->stChnInfo.u32Width;
    stRgaAttr.stImgIn.u32VirStride = sdk_cfg_get_member(astViInfo[priv->info->ViChn])->stChnInfo.u32Height;
    stRgaAttr.stImgOut.u32X = 0;
    stRgaAttr.stImgOut.u32Y = 0;
    stRgaAttr.stImgOut.imgType = priv->info->imgType;
    stRgaAttr.stImgOut.u32Width =  priv->info->u32Width;
    stRgaAttr.stImgOut.u32Height = priv->info->u32Height;
    stRgaAttr.stImgOut.u32HorStride = priv->info->u32Width;
    stRgaAttr.stImgOut.u32VirStride = priv->info->u32Height;
    s32Ret = RK_MPI_RGA_CreateChn(0, &stRgaAttr);
    if (s32Ret) {
        printf("ERROR: Create rga[0] falied! ret=%d\n", s32Ret);
        return -1;
    }
    
    MPP_CHN_S stSrcChn;
    stSrcChn.enModId = RK_ID_VI;
    stSrcChn.s32DevId = priv->info->DevId;
    stSrcChn.s32ChnId = priv->info->ViChn;
    MPP_CHN_S stDestChn;
    stDestChn.enModId = RK_ID_RGA;
    stDestChn.s32DevId = priv->info->DevId;
    stDestChn.s32ChnId = priv->info->RgaId;
    s32Ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
    if (s32Ret) {
        printf("ERROR: Bind vi[0] and rga[0] failed! ret=%d\n", s32Ret);
        return -1;
    }

    pthread_create(&priv->hq_thread, NULL, __vpss_get_chn_frame_proc, priv);
    return s32Ret;
}

static int __vpss_destroy(vsf_vpss_t *self)
{
    vsf_vpss_mod_t *mod   = &s_mod;
    vsf_vpss_t *obj       = self;
    vsf_vpss_priv_t *priv = obj->priv;

    priv->quit = 1;
    pthread_join(priv->hq_thread, NULL);
    MPP_CHN_S stSrcChn;
    stSrcChn.enModId = RK_ID_VI;
    stSrcChn.s32ChnId = priv->info->ViChn;
    MPP_CHN_S stDestChn;
    stDestChn.enModId = RK_ID_RGA;
    stDestChn.s32ChnId = priv->info->RgaId;
    RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
    RK_MPI_RGA_DestroyChn(priv->info->RgaId);
    mod->objs[priv->info->RgaId] = NULL;
    free(priv);
    free(obj);

    return 0;
}

static int __vpss_ctrl(vsf_vpss_t *self, int id, vsf_frame_cfg_t *cfg)
{
//todo 优化，实时改变获取的分辨率

    return 0;
}

static int __vpss_regcallback(vsf_vpss_t *self, int id, vsf_frame_cb_t *cb)
{
    vsf_vpss_t *obj       = self;
    vsf_vpss_priv_t *priv = obj->priv;

    if (cb == NULL) {
        priv->cb.args  = NULL;
        priv->cb.func  = NULL;
    } else {
        priv->cb.args  = cb->args;
        priv->cb.func  = cb->func;
    }

    return 0;
}

vsf_vpss_t *VSF_createVpss(int id)
{
    vsf_vpss_mod_t *mod   = &s_mod;
    vsf_vpss_t *obj       = NULL;
    vsf_vpss_priv_t *priv = NULL;

    if (id >= mod->num) {
        return NULL;
    }

    obj = mod->objs[id];
    if (obj) {
        return obj;
    }

    priv = malloc(sizeof(vsf_vpss_priv_t));
    assert(priv);
    memset(priv, 0, sizeof(vsf_vpss_priv_t));
    priv->info   = sdk_cfg_get_member(astVpssInfo[id]);


    obj = malloc(sizeof(vsf_vpss_t));
    assert(obj);
    memset(obj, 0, sizeof(vsf_vpss_t));
    obj->priv        = priv;
    obj->init        = __vpss_init;
    obj->destroy     = __vpss_destroy;
    obj->ctrl        = __vpss_ctrl;
    obj->regcallback = __vpss_regcallback;

    mod->objs[id] = obj;
    return obj;
}

int VSF_getVpssNum(void)
{
    vsf_vpss_mod_t *mod = &s_mod;
    return mod->num;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

static void __attribute__((constructor(VSF_VPSS_PRIORITY))) sdk_vpss_constructor()
{
    int i;
    vsf_vpss_mod_t *mod = &s_mod;

    mod->num  = *sdk_cfg_get_member(s32VpssNum);
    mod->objs = calloc(mod->num, sizeof(vsf_vpss_t *));
    assert(mod->objs);

    for (i = 0; i < mod->num; i++) {
        vsf_vpss_t *obj = VSF_createVpss(i);
        assert(!obj->init(obj));
    }
}

static void __attribute__((destructor(VSF_VPSS_PRIORITY))) sdk_vpss_destructor()
{
    int i;
    vsf_vpss_mod_t *mod = &s_mod;

    for (i = 0; i < mod->num; i++) {
        vsf_vpss_t *obj = VSF_createVpss(i);
        assert(!obj->destroy(obj));
    }

    free(mod->objs);
    mod->num = 0;
}