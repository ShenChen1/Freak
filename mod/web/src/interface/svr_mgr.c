#include "web/svr_mgr.h"
#include "inc/cfg.h"
#include "common.h"
#include "log.h"

extern void *http_server_init(const char *ip, int port);
extern int http_server_uninit(void *web);

typedef struct {
    void *server[WEB_ITEM_MAX];
    proto_web_svr_t *info;
} web_svr_mgr_priv_t;

static web_svr_mgr_t *s_mgr = NULL;

static int __web_svr_destroy(web_svr_mgr_t *self)
{
    // never destroy
    return 0;
}

static int __web_svr_set(web_svr_mgr_t *self, proto_web_svr_cfg_t *cfg)
{
    int ret;
    web_svr_mgr_t *mgr       = self;
    web_svr_mgr_priv_t *priv = mgr->priv;

    if (cfg->enable) {
        if (priv->server[cfg->id] == NULL) {
            priv->server[cfg->id] = http_server_init(cfg->ip, cfg->port);
            assert(priv->server[cfg->id]);
        }
    } else {
        if (priv->server[cfg->id]) {
            ret = http_server_uninit(priv->server[cfg->id]);
            assert(ret == 0);
            priv->server[cfg->id] = NULL;
        }
    }

    return 0;
}

static int __web_svr_get(web_svr_mgr_t *self, proto_web_svr_cfg_t *cfg)
{
    web_svr_mgr_t *mgr       = self;
    web_svr_mgr_priv_t *priv = mgr->priv;

    *cfg = priv->info->cfgs[cfg->id];
    return 0;
}

static int __web_svr_cap(web_svr_mgr_t *self, proto_web_svr_cap_t *cap)
{
    web_svr_mgr_t *mgr       = self;
    web_svr_mgr_priv_t *priv = mgr->priv;

    *cap = priv->info->caps[cap->id];
    return 0;
}

static int __web_svr_num(web_svr_mgr_t *self)
{
    web_svr_mgr_t *mgr       = self;
    web_svr_mgr_priv_t *priv = mgr->priv;

    return priv->info->num;
}

web_svr_mgr_t *web_createSvrMgr(void)
{
    web_svr_mgr_t *mgr       = s_mgr;
    web_svr_mgr_priv_t *priv = NULL;

    if (mgr) {
        return mgr;
    }

    priv = malloc(sizeof(web_svr_mgr_priv_t));
    assert(priv);
    memset(priv, 0, sizeof(web_svr_mgr_priv_t));
    priv->info = cfg_get_member(server);

    mgr = malloc(sizeof(web_svr_mgr_t));
    assert(mgr);
    memset(mgr, 0, sizeof(web_svr_mgr_t));
    mgr->priv    = priv;
    mgr->destroy = __web_svr_destroy;
    mgr->get     = __web_svr_get;
    mgr->set     = __web_svr_set;
    mgr->cap     = __web_svr_cap;
    mgr->num     = __web_svr_num;

    s_mgr = mgr;
    return mgr;
}