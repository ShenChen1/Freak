#ifndef __RTSP_SVR_MGR_H__
#define __RTSP_SVR_MGR_H__

#include "proto_rtsp.h"

typedef struct svr_mgr {
    void *priv;
    int (*destroy)(struct svr_mgr *self);

    int (*num)(struct svr_mgr *self);
    int (*cap)(struct svr_mgr *self, proto_rtsp_svr_cap_t *cap);
    int (*get)(struct svr_mgr *self, proto_rtsp_svr_cfg_t *cfg);
    int (*set)(struct svr_mgr *self, proto_rtsp_svr_cfg_t *cfg);

} rtsp_svr_mgr_t;

rtsp_svr_mgr_t *rtsp_createSvrMgr(void);

#endif /* __RTSP_SVR_MGR_H__ */