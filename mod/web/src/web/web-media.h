#ifndef __WEB_MEDIA_H__
#define __WEB_MEDIA_H__

typedef int (*web_media_proc_t)(void *data, int len, void *args);

typedef struct {
    void *args;
    web_media_proc_t func;
} web_media_cb_t;

typedef struct media {
    void *priv;
    int (*destroy)(struct media *self);

    int (*regcallback)(struct media *self, web_media_cb_t *cb);
} web_media_t;

web_media_t *web_createMedia(const char *path);

#endif /* __WEB_MEDIA_H__ */