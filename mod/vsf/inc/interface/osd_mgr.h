#ifndef __OSD_MGR_H__
#define __OSD_MGR_H__

#include "proto.h"

typedef struct osd_mgr {
    void *priv;
    int (*init)(struct osd_mgr *self);
    int (*destroy)(struct osd_mgr *self);

    int (*ctrl)(struct osd_mgr *self, proto_vsf_osd_t *info);

} vsf_osd_mgr_t;

vsf_osd_mgr_t *VSF_createOsdMgr(void);

#endif //__OSD_MGR_H__