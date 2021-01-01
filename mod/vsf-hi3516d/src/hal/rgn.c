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

static int __rgn_ctrl(vsf_rgn_t *self, void *param)
{
    return 0;
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