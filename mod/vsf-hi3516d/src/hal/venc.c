#include "common.h"
#include "log.h"
#include "inc/hal/venc.h"
#include "inc/sdk_cfg.h"

typedef struct {
    int virtid;
    int phyid;
    sdk_venc_info_t *info;
} vsf_venc_priv_t;

typedef struct {
    int num;
    vsf_venc_t **objs;
} vsf_venc_mod_t;

static vsf_venc_mod_t s_mod;

////////////////////////////////////////////////////////////////////////////////////////////////////

static int __venc_init(vsf_venc_t *self)
{
    int s32Ret;
    vsf_venc_t *obj       = self;
    vsf_venc_priv_t *priv = obj->priv;

    VENC_GOP_ATTR_S stGopAttr;
    s32Ret = SAMPLE_COMM_VENC_GetGopAttr(priv->info->enGopMode, &stGopAttr);
    if (HI_SUCCESS != s32Ret) {
        errorf("Venc Get GopAttr for %#x!", s32Ret);
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_VENC_Start(priv->info->VencChn,
                                    priv->info->enPayLoad,
                                    priv->info->enSize,
                                    priv->info->enRcMode,
                                    priv->info->u32Profile,
                                    priv->info->bRcnRefShareBuf,
                                    &stGopAttr);
    if (HI_SUCCESS != s32Ret) {
        errorf("Venc Start failed for %#x!", s32Ret);
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_VPSS_Bind_VENC(priv->info->VpssGrp, priv->info->VpssChn, priv->info->VencChn);
    if (HI_SUCCESS != s32Ret) {
        errorf("Venc Get GopAttr failed for %#x!", s32Ret);
        SAMPLE_COMM_VENC_Stop(priv->info->VencChn);
        return s32Ret;
    }

    return s32Ret;
}

static int __venc_destroy(vsf_venc_t *self)
{
    vsf_venc_mod_t *mod   = &s_mod;
    vsf_venc_t *obj       = self;
    vsf_venc_priv_t *priv = obj->priv;

    SAMPLE_COMM_VPSS_UnBind_VENC(priv->info->VpssGrp, priv->info->VpssChn, priv->info->VencChn);
    SAMPLE_COMM_VENC_Stop(priv->info->VencChn);
    mod->objs[priv->virtid] = NULL;
    free(priv);
    free(obj);

    return 0;
}

vsf_venc_t *VSF_createVenc(int id)
{
    vsf_venc_mod_t *mod   = &s_mod;
    vsf_venc_t *obj       = NULL;
    vsf_venc_priv_t *priv = NULL;

    if (id >= mod->num) {
        return NULL;
    }

    obj = mod->objs[id];
    if (obj) {
        return obj;
    }

    priv = malloc(sizeof(vsf_venc_priv_t));
    assert(priv);
    memset(priv, 0, sizeof(vsf_venc_priv_t));
    priv->virtid = id;
    priv->phyid  = *sdk_cfg_get_member(as32VencId[id]);
    priv->info   = sdk_cfg_get_member(astVencInfo[id]);

    obj = malloc(sizeof(vsf_venc_priv_t));
    if (priv == NULL) {
        return NULL;
    }

    obj->priv    = priv;
    obj->init    = __venc_init;
    obj->destroy = __venc_destroy;

    mod->objs[id] = obj;
    return obj;
}

int VSF_getVencNum(void)
{
    vsf_venc_mod_t *mod = &s_mod;
    return mod->num;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

static void __attribute__((constructor(VSF_VPSS_PRIORITY))) sdk_vpss_constructor()
{
    int i;
    vsf_venc_mod_t *mod = &s_mod;

    mod->num  = *sdk_cfg_get_member(s32VencNum);
    mod->objs = calloc(mod->num, sizeof(vsf_venc_t *));
    assert(mod->objs);

    for (i = 0; i < mod->num; i++) {
        vsf_venc_t *obj = VSF_createVenc(i);
        assert(!obj->init(obj));
    }
}

static void __attribute__((destructor(VSF_VPSS_PRIORITY))) sdk_vpss_destructor()
{
    int i;
    vsf_venc_mod_t *mod = &s_mod;

    for (i = 0; i < mod->num; i++) {
        vsf_venc_t *obj = VSF_createVenc(i);
        assert(!obj->destroy(obj));
    }

    free(mod->objs);
    mod->num = 0;
}