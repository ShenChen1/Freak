#ifndef __VSF_VENC_H__
#define __VSF_VENC_H__

typedef int (*VSF_getVenStreamProc)(int id, int type, void *stream, void *args);
typedef struct {
    void *args;
    VSF_getVenStreamProc func;
} VSF_getVenStreamCb;

typedef struct venc {
    void *priv;
    int (*init)(struct venc *self);
    int (*destroy)(struct venc *self);

    int (*regCallback)(struct venc *self, VSF_getVenStreamCb *cb);
} vsf_venc_t;

vsf_venc_t *VSF_createVenc(int id);
int VSF_getVencNum(void);

#endif //__VSF_VENC_H__