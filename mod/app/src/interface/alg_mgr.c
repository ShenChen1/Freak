#include "app/alg_mgr.h"
#include "common.h"
#include "inc/cfg.h"
#include "log.h"

typedef struct {
    proto_app_alg_t *info;
} app_alg_mgr_priv_t;

static app_alg_mgr_t *a_mgr = NULL;

static int __app_alg_destroy(app_alg_mgr_t *self)
{
    // never destroy
    return 0;
}

static int __app_alg_set(app_alg_mgr_t *self, proto_app_alg_cfg_t *cfg)
{
    return 0;
}

static int __app_alg_get(app_alg_mgr_t *self, proto_app_alg_cfg_t *cfg)
{
    app_alg_mgr_t *mgr       = self;
    app_alg_mgr_priv_t *priv = mgr->priv;

    *cfg = priv->info->cfgs[cfg->id];
    return 0;
}

static int __app_alg_cap(app_alg_mgr_t *self, proto_app_alg_cap_t *cap)
{
    app_alg_mgr_t *mgr       = self;
    app_alg_mgr_priv_t *priv = mgr->priv;

    *cap = priv->info->caps[cap->id];
    return 0;
}

static int __app_alg_num(app_alg_mgr_t *self)
{
    app_alg_mgr_t *mgr       = self;
    app_alg_mgr_priv_t *priv = mgr->priv;

    return priv->info->num.num;
}

app_alg_mgr_t *app_createAlgMgr(void)
{
    app_alg_mgr_t *mgr       = a_mgr;
    app_alg_mgr_priv_t *priv = NULL;

    if (mgr) {
        return mgr;
    }

    priv = malloc(sizeof(app_alg_mgr_priv_t));
    assert(priv);
    memset(priv, 0, sizeof(app_alg_mgr_priv_t));
    priv->info = cfg_get_member(alginfo);

    mgr = malloc(sizeof(app_alg_mgr_t));
    assert(mgr);
    memset(mgr, 0, sizeof(app_alg_mgr_t));
    mgr->priv    = priv;
    mgr->destroy = __app_alg_destroy;
    mgr->num     = __app_alg_num;
    mgr->cap     = __app_alg_cap;
    mgr->get     = __app_alg_get;
    mgr->set     = __app_alg_set;

    a_mgr = mgr;
    return mgr;
}
