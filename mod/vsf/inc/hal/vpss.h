#ifndef __VSF_VPSS_H__
#define __VSF_VPSS_H__

typedef int (*vsf_frame_proc_t)(void *frame, void *args);

typedef struct {
    void *args;
    vsf_frame_proc_t func;
} vsf_frame_cb_t;

typedef struct vpss {
    void *priv;
    int (*init)(struct vpss *self);
    int (*destroy)(struct vpss *self);

    int (*regcallback)(struct vpss *self, int id, vsf_frame_cb_t *cb);
} vsf_vpss_t;

vsf_vpss_t *VSF_createVpss(int id);
int VSF_getVpssNum(void);

#endif //__VSF_VPSS_H__