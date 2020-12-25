#include "rtsp/svr_mgr.h"
#include "inc/cfg.h"
#include "common.h"
#include "log.h"

extern void *rtsp_server_init(const char *ip, int port);
extern int rtsp_server_uninit(void *rtsp);

typedef struct {
    void *server[RTSP_ITEM_MAX];
    proto_rtsp_svr_t *info;
} rtsp_svr_mgr_priv_t;

static rtsp_svr_mgr_t *s_mgr = NULL;

static int __rtsp_svr_destroy(rtsp_svr_mgr_t *self)
{
    // never destroy
    return 0;
}

static int __rtsp_svr_set(rtsp_svr_mgr_t *self, proto_rtsp_svr_cfg_t *cfg)
{
    int ret;
    rtsp_svr_mgr_t *mgr       = self;
    rtsp_svr_mgr_priv_t *priv = mgr->priv;

    if (cfg->enable) {
        if (priv->server[cfg->id] == NULL) {
            priv->server[cfg->id] = rtsp_server_init(cfg->ip, cfg->port);
            assert(priv->server[cfg->id]);
        }
    } else {
        if (priv->server[cfg->id]) {
            ret = rtsp_server_uninit(priv->server[cfg->id]);
            assert(ret == 0);
            priv->server[cfg->id] = NULL;
        }
    }

    return 0;
}

static int __rtsp_svr_get(rtsp_svr_mgr_t *self, proto_rtsp_svr_cfg_t *cfg)
{
    rtsp_svr_mgr_t *mgr       = self;
    rtsp_svr_mgr_priv_t *priv = mgr->priv;

    *cfg = priv->info->cfgs[cfg->id];
    return 0;
}

static int __rtsp_svr_cap(rtsp_svr_mgr_t *self, proto_rtsp_svr_cap_t *cap)
{
    rtsp_svr_mgr_t *mgr       = self;
    rtsp_svr_mgr_priv_t *priv = mgr->priv;

    *cap = priv->info->caps[cap->id];
    return 0;
}

static int __rtsp_svr_num(rtsp_svr_mgr_t *self)
{
    rtsp_svr_mgr_t *mgr       = self;
    rtsp_svr_mgr_priv_t *priv = mgr->priv;

    return priv->info->num;
}

rtsp_svr_mgr_t *rtsp_createSvrMgr(void)
{
    rtsp_svr_mgr_t *mgr       = s_mgr;
    rtsp_svr_mgr_priv_t *priv = NULL;

    if (mgr) {
        return mgr;
    }

    priv = malloc(sizeof(rtsp_svr_mgr_priv_t));
    assert(priv);
    memset(priv, 0, sizeof(rtsp_svr_mgr_priv_t));
    priv->info = cfg_get_member(server);

    mgr = malloc(sizeof(rtsp_svr_mgr_t));
    assert(mgr);
    memset(mgr, 0, sizeof(rtsp_svr_mgr_t));
    mgr->priv    = priv;
    mgr->destroy = __rtsp_svr_destroy;
    mgr->get     = __rtsp_svr_get;
    mgr->set     = __rtsp_svr_set;
    mgr->cap     = __rtsp_svr_cap;
    mgr->num     = __rtsp_svr_num;

    s_mgr = mgr;
    return mgr;
}