#include "inc/hal/vi.h"
#include "common.h"
#include "inc/sdk_cfg.h"
#include "log.h"

typedef struct {
    int virtid;
    int phyid;
    SAMPLE_VI_CONFIG_S *pstViConfig;
} vsf_vi_priv_t;

typedef struct {
    int num;
    vsf_vi_t **objs;
    SAMPLE_VI_CONFIG_S stViConfig;
} vsf_vi_mod_t;

static vsf_vi_mod_t s_mod;

////////////////////////////////////////////////////////////////////////////////////////////////////

static int __vi_init(vsf_vi_t *self)
{
    int s32Ret;
    vsf_vi_t *obj       = self;
    vsf_vi_priv_t *priv = obj->priv;

    s32Ret = SAMPLE_COMM_VI_SetParam(priv->pstViConfig);
    if (s32Ret != HI_SUCCESS) {
        errorf("SAMPLE_COMM_VI_SetParam failed with %d!", s32Ret);
        return s32Ret;
    }

    HI_U32 u32FrameRate             = 0;
    ISP_CTRL_PARAM_S stIspCtrlParam = {};

    SAMPLE_COMM_VI_GetFrameRateBySensor(priv->pstViConfig->astViInfo[priv->virtid].stSnsInfo.enSnsType, &u32FrameRate);
    s32Ret = HI_MPI_ISP_GetCtrlParam(priv->pstViConfig->astViInfo[priv->virtid].stPipeInfo.aPipe[0], &stIspCtrlParam);
    if (s32Ret != HI_SUCCESS) {
        errorf("HI_MPI_ISP_GetCtrlParam failed with %d!", s32Ret);
        return s32Ret;
    }

    stIspCtrlParam.u32StatIntvl = u32FrameRate / 30;
    s32Ret = HI_MPI_ISP_SetCtrlParam(priv->pstViConfig->astViInfo[priv->virtid].stPipeInfo.aPipe[0], &stIspCtrlParam);
    if (s32Ret != HI_SUCCESS) {
        errorf("HI_MPI_ISP_SetCtrlParam failed with %d!", s32Ret);
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_VI_StartVi(priv->pstViConfig);
    if (s32Ret != HI_SUCCESS) {
        errorf("SAMPLE_COMM_VI_StartVi failed with %d!", s32Ret);
        return s32Ret;
    }

    return s32Ret;
}

static int __vi_destroy(vsf_vi_t *self)
{
    vsf_vi_mod_t *mod   = &s_mod;
    vsf_vi_t *obj       = self;
    vsf_vi_priv_t *priv = obj->priv;

    SAMPLE_COMM_VI_StopVi(priv->pstViConfig);
    mod->objs[priv->virtid] = NULL;
    free(priv);
    free(obj);

    return 0;
}

vsf_vi_t *VSF_createVi(int id)
{
    vsf_vi_mod_t *mod   = &s_mod;
    vsf_vi_t *obj       = NULL;
    vsf_vi_priv_t *priv = NULL;

    if (id >= mod->num) {
        return NULL;
    }

    obj = mod->objs[id];
    if (obj) {
        return obj;
    }

    priv = malloc(sizeof(vsf_vi_priv_t));
    assert(priv);
    memset(priv, 0, sizeof(vsf_vi_priv_t));
    priv->virtid      = id;
    priv->phyid       = mod->stViConfig.as32WorkingViId[id];
    priv->pstViConfig = &mod->stViConfig;

    obj = malloc(sizeof(vsf_vi_priv_t));
    if (obj == NULL) {
        return NULL;
    }

    obj->priv    = priv;
    obj->init    = __vi_init;
    obj->destroy = __vi_destroy;

    mod->objs[id] = obj;
    return obj;
}

int VSF_getViNum(void)
{
    vsf_vi_mod_t *mod = &s_mod;
    return mod->num;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

static void __transfor_transfor_cfg(sdk_cfg_t *cfg, SAMPLE_VI_CONFIG_S *pstViConfig)
{
    int i;

    memset(pstViConfig, 0, sizeof(SAMPLE_VI_CONFIG_S));
    pstViConfig->s32WorkingViNum = cfg->s32ViNum;

    for (i = 0; i < cfg->s32ViNum; i++) {
        pstViConfig->as32WorkingViId[i] = cfg->as32ViId[i];

        pstViConfig->astViInfo[i].stSnsInfo.enSnsType = cfg->astViInfo[i].stSnsInfo.enSnsType;
        pstViConfig->astViInfo[i].stSnsInfo.s32SnsId  = cfg->astViInfo[i].stSnsInfo.s32SnsId;
        pstViConfig->astViInfo[i].stSnsInfo.s32BusId  = cfg->astViInfo[i].stSnsInfo.s32BusId;
        pstViConfig->astViInfo[i].stSnsInfo.MipiDev =
            SAMPLE_COMM_VI_GetComboDevBySensor(pstViConfig->astViInfo[i].stSnsInfo.enSnsType, 0);

        pstViConfig->astViInfo[i].stDevInfo.ViDev     = cfg->astViInfo[i].stDevInfo.ViDev;
        pstViConfig->astViInfo[i].stDevInfo.enWDRMode = WDR_MODE_NONE;

        memcpy(pstViConfig->astViInfo[i].stPipeInfo.aPipe,
               cfg->astViInfo[i].stPipeInfo.aPipe,
               WDR_MAX_PIPE_NUM * sizeof(VI_PIPE));
        pstViConfig->astViInfo[i].stPipeInfo.enMastPipeMode = cfg->astViInfo[i].stPipeInfo.enMastPipeMode;
        pstViConfig->astViInfo[i].stPipeInfo.bMultiPipe     = cfg->astViInfo[i].stPipeInfo.bMultiPipe;
        pstViConfig->astViInfo[i].stPipeInfo.bVcNumCfged    = cfg->astViInfo[i].stPipeInfo.bVcNumCfged;
        pstViConfig->astViInfo[i].stPipeInfo.bIspBypass     = cfg->astViInfo[i].stPipeInfo.bIspBypass;
        pstViConfig->astViInfo[i].stPipeInfo.enPixFmt       = cfg->astViInfo[i].stPipeInfo.enPixFmt;
        memcpy(pstViConfig->astViInfo[i].stPipeInfo.u32VCNum,
               cfg->astViInfo[i].stPipeInfo.u32VCNum,
               WDR_MAX_PIPE_NUM * sizeof(HI_U32));

        pstViConfig->astViInfo[i].stChnInfo.ViChn          = cfg->astViInfo[i].stChnInfo.ViChn;
        pstViConfig->astViInfo[i].stChnInfo.enPixFormat    = cfg->astViInfo[i].stChnInfo.enPixFormat;
        pstViConfig->astViInfo[i].stChnInfo.enDynamicRange = cfg->astViInfo[i].stChnInfo.enDynamicRange;
        pstViConfig->astViInfo[i].stChnInfo.enVideoFormat  = cfg->astViInfo[i].stChnInfo.enVideoFormat;
        pstViConfig->astViInfo[i].stChnInfo.enCompressMode = cfg->astViInfo[i].stChnInfo.enCompressMode;
    }
}

static void __attribute__((constructor(VSF_VI_PRIORITY))) sdk_vi_constructor()
{
    int i;
    vsf_vi_mod_t *mod = &s_mod;

    mod->num  = *sdk_cfg_get_member(s32ViNum);
    mod->objs = calloc(mod->num, sizeof(vsf_vi_t *));
    assert(mod->objs);
    __transfor_transfor_cfg(sdk_cfg_get(), &mod->stViConfig);

    for (i = 0; i < mod->num; i++) {
        vsf_vi_t *obj = VSF_createVi(i);
        assert(!obj->init(obj));
    }
}

static void __attribute__((destructor(VSF_VI_PRIORITY))) sdk_vi_destructor()
{
    int i;
    vsf_vi_mod_t *mod = &s_mod;

    for (i = 0; i < mod->num; i++) {
        vsf_vi_t *obj = VSF_createVi(i);
        assert(!obj->destroy(obj));
    }

    free(mod->objs);
    mod->num = 0;
}