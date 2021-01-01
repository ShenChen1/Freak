#ifndef __VSF_VPSS_H__
#define __VSF_VPSS_H__

typedef int (*vsf_frame_proc_t)(void *frame, void *args);

typedef struct {
    void *args;
    vsf_frame_proc_t func;
} vsf_frame_cb_t;

typedef enum {
    VSF_FRAME_CB_GET,
    VSF_FRAME_CB_FREE,
    VSF_FRAME_CB_MAX,
} vsf_frame_cb_e;

typedef struct {
    int format;
    int width;
    int height;
    int fps;
} vsf_frame_cfg_t;
typedef struct vpss {
    void *priv;
    int (*init)(struct vpss *self);
    int (*destroy)(struct vpss *self);

    int (*ctrl)(struct vpss *self, int id, vsf_frame_cfg_t *cfg);
    int (*regcallback)(struct vpss *self, int id, vsf_frame_cb_t cb[VSF_FRAME_CB_MAX]);
} vsf_vpss_t;

vsf_vpss_t *VSF_createVpss(int id);
int VSF_getVpssNum(void);

#endif //__VSF_VPSS_H__