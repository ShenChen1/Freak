#ifndef __WEB_SVR_MGR_H__
#define __WEB_SVR_MGR_H__

#include "proto_web.h"

typedef struct svr_mgr {
    void *priv;
    int (*destroy)(struct svr_mgr *self);

    int (*num)(struct svr_mgr *self);
    int (*cap)(struct svr_mgr *self, proto_web_svr_cap_t *cap);
    int (*get)(struct svr_mgr *self, proto_web_svr_cfg_t *cfg);
    int (*set)(struct svr_mgr *self, proto_web_svr_cfg_t *cfg);

} web_svr_mgr_t;

web_svr_mgr_t *web_createSvrMgr(void);
web_svr_mgr_t *web_createSvrMgr_r(void);

#endif /* __WEB_SVR_MGR_H__ */