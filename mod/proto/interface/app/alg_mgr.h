#ifndef __APP_ALG_MGR_H__
#define __APP_ALG_MGR_H__

#include "proto_app.h"

typedef struct alg_mgr {
    void *priv;
    int (*destroy)(struct alg_mgr *self);

    int (*num)(struct alg_mgr *self);
    int (*cap)(struct alg_mgr *self, proto_app_alg_cap_t *cap);
    int (*get)(struct alg_mgr *self, proto_app_alg_cfg_t *cfg);
    int (*set)(struct alg_mgr *self, proto_app_alg_cfg_t *cfg);

} app_alg_mgr_t;

app_alg_mgr_t *app_createAlgMgr(void);

#endif /* __APP_ALG_MGR_H__ */