#include "inc/hal/vi.h"
#include "inc/sample_common.h"
#include "common.h"
#include "inc/sdk_cfg.h"
#include "log.h"

typedef struct {
    int camid;
    int chnid;
    VI_CHN_ATTR_S stViConfig;
} vsf_vi_priv_t;

typedef struct {
    int num;
    vsf_vi_t **objs;
} vsf_vi_mod_t;

static vsf_vi_mod_t s_mod;

////////////////////////////////////////////////////////////////////////////////////////////////////

static int __vi_init(vsf_vi_t *self)
{
    int s32Ret;
    vsf_vi_t *obj       = self;
    vsf_vi_priv_t *priv = obj->priv;
    s32Ret = RK_MPI_VI_SetChnAttr(priv->camid, priv->chnid, &priv->stViConfig);
    s32Ret |= RK_MPI_VI_EnableChn(priv->camid, priv->chnid);
    if (s32Ret) {
        printf("ERROR: create VI[0] error! ret=%d\n", s32Ret);
        return 0;
    }
    return s32Ret;
}

static int __vi_destroy(vsf_vi_t *self)
{
    vsf_vi_mod_t *mod   = &s_mod;
    vsf_vi_t *obj       = self;
    vsf_vi_priv_t *priv = obj->priv;

    RK_MPI_VI_DisableChn(priv->camid, priv->chnid);
    mod->objs[priv->chnid] = NULL;
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

    obj = malloc(sizeof(vsf_vi_t));
    assert(obj);
    memset(obj, 0, sizeof(vsf_vi_t));
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

static void __transfor_transfor_cfg(sdk_vi_info_t *pViCfg, vsf_vi_priv_t *priv)
{
    VI_CHN_ATTR_S *pstViConfig = &priv->stViConfig;
    memset(pstViConfig, 0, sizeof(VI_CHN_ATTR_S));
    priv->camid = pViCfg->pipeId;
    priv->chnid = pViCfg->chnId;
    pstViConfig->pcVideoNode = pViCfg->stChnInfo.pcVideoNode;
    pstViConfig->u32Width = pViCfg->stChnInfo.u32Width;
    pstViConfig->u32Height = pViCfg->stChnInfo.u32Height;
    pstViConfig->enPixFmt = pViCfg->stChnInfo.enPixFmt;
    pstViConfig->u32BufCnt = pViCfg->stChnInfo.u32BufCnt;
    pstViConfig->enBufType = pViCfg->stChnInfo.enBufType;
    pstViConfig->enWorkMode = pViCfg->stChnInfo.enWorkMode;
}

static void __attribute__((constructor(VSF_VI_PRIORITY))) sdk_vi_constructor()
{
    int i;
    vsf_vi_mod_t *mod = &s_mod;

    mod->num  = *sdk_cfg_get_member(s32ViNum);
    mod->objs = calloc(mod->num, sizeof(vsf_vi_t *));
    assert(mod->objs);

    rk_aiq_working_mode_t hdr_mode = RK_AIQ_WORKING_MODE_NORMAL;
    SAMPLE_COMM_ISP_Init(0, hdr_mode, 0, "/etc/iqfiles/");
    SAMPLE_COMM_ISP_Run(0);
    SAMPLE_COMM_ISP_SetFrameRate(0, 30);
    for (i = 0; i < mod->num; i++) {
        vsf_vi_t *obj = VSF_createVi(i);
        __transfor_transfor_cfg(*(sdk_cfg_get_member(astViInfo)+i),(vsf_vi_priv_t *)obj->priv);
        assert(!obj->init(obj));
    }
    //for test


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