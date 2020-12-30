#ifndef __VSF_FRAME_MGR_H__
#define __VSF_FRAME_MGR_H__

#include "proto_vsf.h"
#include "video_frame.h"

typedef struct frame_mgr {
    void *priv;
    int (*destroy)(struct frame_mgr *self);

    int (*num)(struct frame_mgr *self);
    int (*cap)(struct frame_mgr *self, proto_vsf_frame_cap_t *cap);
    int (*get)(struct frame_mgr *self, proto_vsf_frame_cfg_t *cfg);
    int (*set)(struct frame_mgr *self, proto_vsf_frame_cfg_t *cfg);

} vsf_frame_mgr_t;

vsf_frame_mgr_t *vsf_createFrameMgr(void);
vsf_frame_mgr_t *vsf_createFrameMgr_r(void);

#endif //__VSF_FRAME_MGR_H__