#ifndef __VSF_VENC_H__
#define __VSF_VENC_H__

typedef int (*vsf_stream_proc_t)(void *stream, void *args);

typedef struct {
    void *args;
    vsf_stream_proc_t func;
} vsf_stream_cb_t;

typedef struct venc {
    void *priv;
    int (*init)(struct venc *self);
    int (*destroy)(struct venc *self);

    int (*regcallback)(struct venc *self, vsf_stream_cb_t *cb);
} vsf_venc_t;

vsf_venc_t *VSF_createVenc(int id);
int VSF_getVencNum(void);

#endif //__VSF_VENC_H__