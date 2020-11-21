#include "common.h"
#include "log.h"
#include "media.h"
#include "inc/hal/vpss.h"
#include "inc/sdk_cfg.h"
typedef struct {
    int chn;
    vsf_frame_cb_t cb[VSF_FRAME_CB_MAX];
    pthread_t thread[VSF_FRAME_CB_MAX];
    void *group;
} vsf_vpss_chn_t;

typedef struct {
    int virtid;
    int phyid;
    sdk_vpss_info_t *info;
    vsf_vpss_chn_t chn[VPSS_MAX_PHY_CHN_NUM];
} vsf_vpss_priv_t;

typedef struct {
    int num;
    vsf_vpss_t **objs;
} vsf_vpss_mod_t;

static vsf_vpss_mod_t s_mod;

static int __transfor_pixel_format(int type)
{
    assert(type == PIXEL_FORMAT_YVU_SEMIPLANAR_420);
    return VIDEO_FRAME_FORMAT_YUV420SP_VU;
}

static void __transfor_frame_info(VIDEO_FRAME_S *src, video_frame_t *dst, void *priv)
{
    int i;

    memset(dst, 0, sizeof(video_frame_t));
    dst->u32Width        = src->u32Width;
    dst->u32Height       = src->u32Height;
    dst->enPixelFormat   = __transfor_pixel_format(src->enPixelFormat);
    dst->s16OffsetTop    = src->s16OffsetTop;
    dst->s16OffsetBottom = src->s16OffsetBottom;
    dst->s16OffsetLeft   = src->s16OffsetLeft;
    dst->s16OffsetRight  = src->s16OffsetRight;
    dst->u32TimeRef      = src->u32TimeRef;
    dst->u64PTS          = src->u64PTS;
    dst->u64PrivateData  = (size_t)priv;

    for (i = 0; i < 3; i++) {
        dst->u32HeaderStride[i]  = src->u32HeaderStride[i];
        dst->u32Stride[i]        = src->u32Stride[i];
        dst->u32ExtStride[i]     = src->u32ExtStride[i];
        dst->u64HeaderPhyAddr[i] = src->u64HeaderPhyAddr[i];
        dst->u64HeaderVirAddr[i] = src->u64HeaderVirAddr[i];
        dst->u64PhyAddr[i]       = src->u64PhyAddr[i];
        dst->u64VirAddr[i]       = src->u64VirAddr[i];
        dst->u64ExtPhyAddr[i]    = src->u64ExtPhyAddr[i];
        dst->u64ExtVirAddr[i]    = src->u64ExtVirAddr[i];
    }
}

static void *__vpss_get_chn_frame_proc(void *p)
{
    HI_S32 s32Ret;
    vsf_vpss_chn_t *chn   = p;
    vsf_vpss_priv_t *priv = chn->group;

    while (1) {
        VIDEO_FRAME_INFO_S *pstVideoFrame = malloc(sizeof(VIDEO_FRAME_INFO_S));
        s32Ret = HI_MPI_VPSS_GetChnFrame(priv->info->VpssGrp, chn->chn, pstVideoFrame, 1000);
        if (HI_SUCCESS != s32Ret) {
            errorf("HI_MPI_VPSS_GetChnFrame %d-%d err:0x%x\n", priv->info->VpssGrp, chn->chn, s32Ret);
            continue;
        }

        if (chn->cb[VSF_FRAME_CB_GET].func) {
            video_frame_t frame;
            __transfor_frame_info(&pstVideoFrame->stVFrame, &frame, pstVideoFrame);
            tracef("frame:%u", frame.u32TimeRef);
            s32Ret = chn->cb[VSF_FRAME_CB_GET].func(&frame, chn->cb[VSF_FRAME_CB_GET].args);
            if (HI_SUCCESS != s32Ret) {
                errorf("proc frame %d-%d failed!", priv->info->VpssGrp, chn->chn);
            } else {
                continue;
            }
        }

        s32Ret = HI_MPI_VPSS_ReleaseChnFrame(priv->info->VpssGrp, chn->chn, pstVideoFrame);
        if (HI_SUCCESS != s32Ret) {
            errorf("HI_MPI_VPSS_ReleaseChnFrame %d-%d err:0x%x\n", priv->info->VpssGrp, chn->chn, s32Ret);
        }
        free(pstVideoFrame);
    }

    return NULL;
}

static void *__vpss_free_chn_frame_proc(void *p)
{
    HI_S32 s32Ret;
    vsf_vpss_chn_t *chn   = p;
    vsf_vpss_priv_t *priv = chn->group;

    while (1) {
        if (chn->cb[VSF_FRAME_CB_FREE].func) {
            video_frame_t frame;
            s32Ret = chn->cb[VSF_FRAME_CB_FREE].func(&frame, chn->cb[VSF_FRAME_CB_FREE].args);
            if (HI_SUCCESS != s32Ret) {
                errorf("proc frame %d-%d failed!", priv->info->VpssGrp, chn->chn);
                continue;
            }

            tracef("frame:%u", frame.u32TimeRef);
            VIDEO_FRAME_INFO_S *pstVideoFrame = (void *)(size_t)frame.u64PrivateData;
            s32Ret = HI_MPI_VPSS_ReleaseChnFrame(priv->info->VpssGrp, chn->chn, pstVideoFrame);
            if (HI_SUCCESS != s32Ret) {
                errorf("HI_MPI_VPSS_ReleaseChnFrame %d-%d err:0x%x\n", priv->info->VpssGrp, chn->chn, s32Ret);
            }
            free(pstVideoFrame);
        } else {
            usleep(1000 * 1000);
        }
    }

    return NULL;
}

static HI_S32 SAMPLE_VENC_VPSS_Init(sdk_vpss_info_t *info)
{
    HI_S32 i;
    HI_S32 s32Ret;
    SIZE_S stSize;
    VPSS_GRP_ATTR_S stVpssGrpAttr = { 0 };
    VPSS_CHN_ATTR_S stVpssChnAttr[VPSS_MAX_PHY_CHN_NUM];

    s32Ret = SAMPLE_COMM_SYS_GetPicSize(info->enSnsSize, &stSize);
    if (HI_SUCCESS != s32Ret) {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        return s32Ret;
    }

    stVpssGrpAttr.enDynamicRange              = DYNAMIC_RANGE_SDR8;
    stVpssGrpAttr.enPixelFormat               = PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    stVpssGrpAttr.u32MaxW                     = stSize.u32Width;
    stVpssGrpAttr.u32MaxH                     = stSize.u32Height;
    stVpssGrpAttr.stFrameRate.s32SrcFrameRate = -1;
    stVpssGrpAttr.stFrameRate.s32DstFrameRate = -1;
    stVpssGrpAttr.bNrEn                       = HI_TRUE;
    stVpssGrpAttr.stNrAttr.enNrType           = VPSS_NR_TYPE_VIDEO;
    stVpssGrpAttr.stNrAttr.enNrMotionMode     = NR_MOTION_MODE_NORMAL;
    stVpssGrpAttr.stNrAttr.enCompressMode     = COMPRESS_MODE_FRAME;

    for (i = 0; i < VPSS_MAX_PHY_CHN_NUM; i++) {
        if (info->abChnEnable[i]) {

            s32Ret = SAMPLE_COMM_SYS_GetPicSize(info->aenSize[i], &stSize);
            if (HI_SUCCESS != s32Ret) {
                errorf("SAMPLE_COMM_SYS_GetPicSize failed!");
                return s32Ret;
            }

            stVpssChnAttr[i].u32Width                    = stSize.u32Width;
            stVpssChnAttr[i].u32Height                   = stSize.u32Height;
            stVpssChnAttr[i].enChnMode                   = VPSS_CHN_MODE_USER;
            stVpssChnAttr[i].enCompressMode              = COMPRESS_MODE_NONE; // COMPRESS_MODE_SEG;
            stVpssChnAttr[i].enDynamicRange              = DYNAMIC_RANGE_SDR8;
            stVpssChnAttr[i].enPixelFormat               = PIXEL_FORMAT_YVU_SEMIPLANAR_420;
            stVpssChnAttr[i].stFrameRate.s32SrcFrameRate = -1;
            stVpssChnAttr[i].stFrameRate.s32DstFrameRate = -1;
            stVpssChnAttr[i].u32Depth                    = info->au32Depth[i];
            stVpssChnAttr[i].bMirror                     = HI_FALSE;
            stVpssChnAttr[i].bFlip                       = HI_FALSE;
            stVpssChnAttr[i].enVideoFormat               = VIDEO_FORMAT_LINEAR;
            stVpssChnAttr[i].stAspectRatio.enMode        = ASPECT_RATIO_NONE;
        }
    }

    s32Ret = SAMPLE_COMM_VPSS_Start(info->VpssGrp, info->abChnEnable, &stVpssGrpAttr, stVpssChnAttr);
    if (s32Ret != HI_SUCCESS) {
        errorf("start VPSS fail for %#x!\n", s32Ret);
    }

    return s32Ret;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

static int __vpss_init(vsf_vpss_t *self)
{
    int s32Ret, i;
    vsf_vpss_t *obj       = self;
    vsf_vpss_priv_t *priv = obj->priv;

    s32Ret = SAMPLE_VENC_VPSS_Init(priv->info);
    if (HI_SUCCESS != s32Ret) {
        errorf("Init VPSS err for %#x!", s32Ret);
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_VI_Bind_VPSS(priv->info->ViPipe, priv->info->ViChn, priv->info->VpssGrp);
    if (s32Ret != HI_SUCCESS) {
        errorf("VI Bind VPSS err for %#x!", s32Ret);
        SAMPLE_COMM_VPSS_Stop(priv->info->VpssGrp, priv->info->abChnEnable);
        return s32Ret;
    }

    for (i = 0; i < VPSS_MAX_PHY_CHN_NUM; i++) {
        pthread_create(&priv->chn[i].thread[VSF_FRAME_CB_GET], NULL, __vpss_get_chn_frame_proc, &priv->chn[i]);
        pthread_create(&priv->chn[i].thread[VSF_FRAME_CB_FREE], NULL, __vpss_free_chn_frame_proc, &priv->chn[i]);
    }

    return s32Ret;
}

static int __vpss_destroy(vsf_vpss_t *self)
{
    vsf_vpss_mod_t *mod   = &s_mod;
    vsf_vpss_t *obj       = self;
    vsf_vpss_priv_t *priv = obj->priv;

    SAMPLE_COMM_VI_UnBind_VPSS(priv->info->ViPipe, priv->info->ViChn, priv->info->VpssGrp);
    SAMPLE_COMM_VPSS_Stop(priv->info->VpssGrp, priv->info->abChnEnable);
    mod->objs[priv->virtid] = NULL;
    free(priv);
    free(obj);

    return 0;
}

static int __vpss_regcallback(vsf_vpss_t *self, int id, vsf_frame_cb_t *cb)
{
    vsf_vpss_t *obj       = self;
    vsf_vpss_priv_t *priv = obj->priv;

    if (cb == NULL) {
        priv->chn[id].cb[VSF_FRAME_CB_GET].args  = NULL;
        priv->chn[id].cb[VSF_FRAME_CB_GET].func  = NULL;
        priv->chn[id].cb[VSF_FRAME_CB_FREE].args = NULL;
        priv->chn[id].cb[VSF_FRAME_CB_FREE].func = NULL;
    } else {
        priv->chn[id].cb[VSF_FRAME_CB_GET].args  = cb[VSF_FRAME_CB_GET].args;
        priv->chn[id].cb[VSF_FRAME_CB_GET].func  = cb[VSF_FRAME_CB_GET].func;
        priv->chn[id].cb[VSF_FRAME_CB_FREE].args = cb[VSF_FRAME_CB_FREE].args;
        priv->chn[id].cb[VSF_FRAME_CB_FREE].func = cb[VSF_FRAME_CB_FREE].func;
    }

    return 0;
}

vsf_vpss_t *VSF_createVpss(int id)
{
    int i;
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
    priv->virtid = id;
    priv->phyid  = *sdk_cfg_get_member(as32VpssId[id]);
    priv->info   = sdk_cfg_get_member(astVpssInfo[id]);
    for (i = 0; i < VPSS_MAX_PHY_CHN_NUM; i++) {
        priv->chn[i].chn   = i;
        priv->chn[i].group = priv;
    }

    obj = malloc(sizeof(vsf_vpss_priv_t));
    if (obj == NULL) {
        return NULL;
    }

    obj->priv        = priv;
    obj->init        = __vpss_init;
    obj->destroy     = __vpss_destroy;
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