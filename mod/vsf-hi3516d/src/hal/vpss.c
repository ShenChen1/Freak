#include "common.h"
#include "log.h"
#include "media.h"
#include "inc/hal/vpss.h"
#include "inc/sdk_cfg.h"

typedef struct {
    int virtid;
    int phyid;
    sdk_vpss_info_t *info;
} vsf_vpss_priv_t;

typedef struct {
    int num;
    vsf_vpss_t **objs;
} vsf_vpss_mod_t;

static vsf_vpss_mod_t s_mod;

static int __transfor_pixel_format(int type)
{
    assert(type == PIXEL_FORMAT_YVU_SEMIPLANAR_420);
    return VIDEO_FRAME_FORMAT_YUV420P_YVU;
}

static void __transfor_frame_info(VIDEO_FRAME_S *src, video_frame_t *dst)
{
    int i;

    dst->u32Width        = src->u32Height;
    dst->u32Height       = src->u32Height;
    dst->enPixelFormat   = __transfor_pixel_format(src->enPixelFormat);
    dst->s16OffsetTop    = src->s16OffsetTop;
    dst->s16OffsetBottom = src->s16OffsetBottom;
    dst->s16OffsetLeft   = src->s16OffsetLeft;
    dst->s16OffsetRight  = src->s16OffsetRight;
    dst->u32TimeRef      = src->u32TimeRef;
    dst->u64PTS          = src->u64PTS;
    dst->u64PrivateData  = (size_t)src;

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

static HI_S32 SAMPLE_VENC_VPSS_Init(VPSS_GRP VpssGrp,
                                    HI_BOOL *pabChnEnable,
                                    DYNAMIC_RANGE_E enDynamicRange,
                                    PIXEL_FORMAT_E enPixelFormat,
                                    SIZE_S stSize[],
                                    SAMPLE_SNS_TYPE_E enSnsType)
{
    HI_S32 i;
    HI_S32 s32Ret;
    PIC_SIZE_E enSnsSize;
    SIZE_S stSnsSize;
    VPSS_GRP_ATTR_S stVpssGrpAttr = { 0 };
    VPSS_CHN_ATTR_S stVpssChnAttr[VPSS_MAX_PHY_CHN_NUM];

    s32Ret = SAMPLE_COMM_VI_GetSizeBySensor(enSnsType, &enSnsSize);
    if (HI_SUCCESS != s32Ret) {
        SAMPLE_PRT("SAMPLE_COMM_VI_GetSizeBySensor failed!\n");
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_SYS_GetPicSize(enSnsSize, &stSnsSize);
    if (HI_SUCCESS != s32Ret) {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        return s32Ret;
    }

    stVpssGrpAttr.enDynamicRange              = enDynamicRange;
    stVpssGrpAttr.enPixelFormat               = enPixelFormat;
    stVpssGrpAttr.u32MaxW                     = stSnsSize.u32Width;
    stVpssGrpAttr.u32MaxH                     = stSnsSize.u32Height;
    stVpssGrpAttr.stFrameRate.s32SrcFrameRate = -1;
    stVpssGrpAttr.stFrameRate.s32DstFrameRate = -1;
    stVpssGrpAttr.bNrEn                       = HI_TRUE;
    stVpssGrpAttr.stNrAttr.enNrType           = VPSS_NR_TYPE_VIDEO;
    stVpssGrpAttr.stNrAttr.enNrMotionMode     = NR_MOTION_MODE_NORMAL;
    stVpssGrpAttr.stNrAttr.enCompressMode     = COMPRESS_MODE_FRAME;

    for (i = 0; i < VPSS_MAX_PHY_CHN_NUM; i++) {
        if (HI_TRUE == pabChnEnable[i]) {
            stVpssChnAttr[i].u32Width                    = stSize[i].u32Width;
            stVpssChnAttr[i].u32Height                   = stSize[i].u32Height;
            stVpssChnAttr[i].enChnMode                   = VPSS_CHN_MODE_USER;
            stVpssChnAttr[i].enCompressMode              = COMPRESS_MODE_NONE; // COMPRESS_MODE_SEG;
            stVpssChnAttr[i].enDynamicRange              = enDynamicRange;
            stVpssChnAttr[i].enPixelFormat               = enPixelFormat;
            stVpssChnAttr[i].stFrameRate.s32SrcFrameRate = -1;
            stVpssChnAttr[i].stFrameRate.s32DstFrameRate = -1;
            stVpssChnAttr[i].u32Depth                    = 0;
            stVpssChnAttr[i].bMirror                     = HI_FALSE;
            stVpssChnAttr[i].bFlip                       = HI_FALSE;
            stVpssChnAttr[i].enVideoFormat               = VIDEO_FORMAT_LINEAR;
            stVpssChnAttr[i].stAspectRatio.enMode        = ASPECT_RATIO_NONE;
        }
    }

    s32Ret = SAMPLE_COMM_VPSS_Start(VpssGrp, pabChnEnable, &stVpssGrpAttr, stVpssChnAttr);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("start VPSS fail for %#x!\n", s32Ret);
    }

    return s32Ret;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

static int __vpss_init(vsf_vpss_t *self)
{
    int i, s32Ret;
    vsf_vpss_t *obj       = self;
    vsf_vpss_priv_t *priv = obj->priv;

    SIZE_S astSize[VPSS_MAX_PHY_CHN_NUM];
    for (i = 0; i < ARRAY_SIZE(astSize); i++) {
        if (!priv->info->abChnEnable[i]) {
            continue;
        }

        s32Ret = SAMPLE_COMM_SYS_GetPicSize(priv->info->aenSize[i], &astSize[i]);
        if (HI_SUCCESS != s32Ret) {
            errorf("SAMPLE_COMM_SYS_GetPicSize failed!");
            return s32Ret;
        }
    }

    s32Ret = SAMPLE_VENC_VPSS_Init(priv->info->VpssGrp,
                                   priv->info->abChnEnable,
                                   DYNAMIC_RANGE_SDR8,
                                   PIXEL_FORMAT_YVU_SEMIPLANAR_420,
                                   astSize,
                                   priv->info->enSnsType);
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

static int __vpss_get_chn_frame(vsf_vpss_t *self, int chn, void *frame, int timeout)
{
    vsf_vpss_t *obj       = self;
    vsf_vpss_priv_t *priv = obj->priv;

    HI_S32 s32Ret;
    VIDEO_FRAME_INFO_S stExtFrmInfo;
    s32Ret = HI_MPI_VPSS_GetChnFrame(priv->info->VpssGrp, chn, &stExtFrmInfo, timeout);
    if (s32Ret == HI_SUCCESS) {
        __transfor_frame_info(&stExtFrmInfo.stVFrame, frame);
    }

    return s32Ret;
}

static int __vpss_free_chn_frame(vsf_vpss_t *self, int chn, void *frame)
{
    vsf_vpss_t *obj       = self;
    vsf_vpss_priv_t *priv = obj->priv;
    video_frame_t *pFrame = frame;
    return HI_MPI_VPSS_ReleaseChnFrame(priv->info->VpssGrp, chn, (void *)(size_t)pFrame->u64PrivateData);
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
    priv->virtid = id;
    priv->phyid  = *sdk_cfg_get_member(as32VpssId[id]);
    priv->info   = sdk_cfg_get_member(astVpssInfo[id]);

    obj = malloc(sizeof(vsf_vpss_priv_t));
    if (obj == NULL) {
        return NULL;
    }

    obj->priv    = priv;
    obj->init    = __vpss_init;
    obj->destroy = __vpss_destroy;
    obj->getChnFrame = __vpss_get_chn_frame;
    obj->freeChnFrame = __vpss_free_chn_frame;

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