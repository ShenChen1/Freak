
#include "inc/hal/vpss.h"
#include "common.h"
#include "inc/sdk_cfg.h"
#include "log.h"

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

////////////////////////////////////////////////////////////////////////////////////////////////////

static int __vpss_init(vsf_vpss_t *self)
{
    int s32Ret;
    vsf_vpss_t *obj       = self;
    vsf_vpss_priv_t *priv = obj->priv;

    s32Ret = SAMPLE_VENC_VPSS_Init(VpssGrp,
                                   abChnEnable,
                                   DYNAMIC_RANGE_SDR8,
                                   PIXEL_FORMAT_YVU_SEMIPLANAR_420,
                                   stSize,
                                   stViConfig.astViInfo[0].stSnsInfo.enSnsType);
    if (HI_SUCCESS != s32Ret) {
        SAMPLE_PRT("Init VPSS err for %#x!\n", s32Ret);
        goto EXIT_VI_STOP;
    }

    s32Ret = SAMPLE_COMM_VI_Bind_VPSS(ViPipe, ViChn, VpssGrp);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("VI Bind VPSS err for %#x!\n", s32Ret);
        goto EXIT_VPSS_STOP;
    }

    return s32Ret;
}

static int __vpss_destroy(vsf_vpss_t *self)
{
    vsf_vpss_mod_t *mod   = &s_mod;
    vsf_vpss_t *obj       = self;
    vsf_vpss_priv_t *priv = obj->priv;

    SAMPLE_COMM_SYS_Exit();
    mod->objs[priv->virtid] = NULL;
    free(priv);
    free(obj);

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
    priv->virtid = id;
    priv->phyid = *sdk_cfg_get_member(as32VpssId[id]);
    priv->info = sdk_cfg_get_member(astVpssInfo[id]);

    obj = malloc(sizeof(vsf_vpss_priv_t));
    if (priv == NULL) {
        return NULL;
    }

    obj->priv    = priv;
    obj->init    = __vpss_init;
    obj->destroy = __vpss_destroy;

    mod->objs[id] = obj;
    return obj;
}

int VSF_getViNum(void)
{
    vsf_vpss_mod_t *mod = &s_mod;
    return mod->num;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

static void __attribute__((constructor(VSF_VI_PRIORITY))) sdk_vi_constructor()
{
    int i;
    vsf_vpss_mod_t *mod = &s_mod;

    mod->num  = *sdk_cfg_get_member(s32ViNum);
    mod->objs = calloc(mod->num, sizeof(vsf_vpss_t *));
    assert(mod->objs);
    __transfor_transfor_cfg(sdk_cfg_get(), &mod->stViConfig);

    for (i = 0; i < mod->num; i++) {
        vsf_vpss_t *obj = VSF_createVi(i);
        assert(!obj->init(obj));
    }
}

static void __attribute__((destructor(VSF_VI_PRIORITY))) sdk_vi_destructor()
{
    int i;
    vsf_vpss_mod_t *mod = &s_mod;

    for (i = 0; i < mod->num; i++) {
        vsf_vpss_t *obj = VSF_createVi(i);
        assert(!obj->destroy(obj));
    }

    free(mod->objs);
    mod->num = 0;
}