#ifndef __FRAME_MGR_H__
#define __FRAME_MGR_H__

#include "proto.h"

typedef struct frame_mgr {
    void *priv;
    int (*init)(struct frame_mgr *self);
    int (*destroy)(struct frame_mgr *self);

    int (*ctrl)(struct frame_mgr *self, proto_vsf_frame_t *info);

    int (*getFrame)(struct frame_mgr *self, int id, void *frame, int timeout);
    int (*freeFrame)(struct frame_mgr *self, int id, void *frame);

} vsf_frame_mgr_t;

vsf_frame_mgr_t *VSF_createFrameMgr(void);

#endif //__FRAME_MGR_H__