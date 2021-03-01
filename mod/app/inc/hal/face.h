#ifndef __HSFACE_H
#define __HSFACE_H

typedef int (*app_alg_proc_t)(void *frame, void *args);
typedef struct {
    void *args;
    app_alg_proc_t func;
} app_alg_cb_t;

typedef enum {
    APP_ALG_CB_FRAME_GET,
    APP_ALG_CB_FRAME_FREE,
    APP_ALG_CB_RESULT_OUT,
    APP_ALG_CB_MAX,
} app_alg_cb_e;

typedef struct face {
    void *priv;
    int (*init)(struct face *self, char *path);
    int (*destroy)(struct face *self);

    int (*regcallback)(struct face *self, int type, app_alg_cb_t cb[APP_ALG_CB_MAX]);
} app_face_t;

app_face_t *APP_createFaceAlg(void);

#endif
