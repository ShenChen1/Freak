#include "media.h"
#include "ufifo.h"

extern unsigned int video_frame_recsize(unsigned char *p1, unsigned int n1, unsigned char *p2);
extern unsigned int video_frame_rectag(unsigned char *p1, unsigned int n1, unsigned char *p2);
extern unsigned int video_frame_recput(unsigned char *p1, unsigned int n1, unsigned char *p2, void *arg);
extern unsigned int video_frame_recget(unsigned char *p1, unsigned int n1, unsigned char *p2, void *arg);

extern unsigned int video_stream_recsize(unsigned char *p1, unsigned int n1, unsigned char *p2);
extern unsigned int video_stream_rectag(unsigned char *p1, unsigned int n1, unsigned char *p2);
extern unsigned int video_stream_recput(unsigned char *p1, unsigned int n1, unsigned char *p2, void *arg);
extern unsigned int video_stream_recget(unsigned char *p1, unsigned int n1, unsigned char *p2, void *arg);

static int mfifo_destroy(mfifo_t *self)
{
    ufifo_close(self->priv);
    free(self);
    return 0;
}

static uint mfifo_size(mfifo_t *self)
{
    return ufifo_size(self->priv);
}

static uint mfifo_len(mfifo_t *self)
{
    return ufifo_len(self->priv);
}

static void mfifo_oldest(mfifo_t *self)
{
    ufifo_oldest(self->priv, (0xdeadbeef << 8) | 1);
}

static void mfifo_newest(mfifo_t *self)
{
    ufifo_newest(self->priv, (0xdeadbeef << 8) | 1);
}

static void mfifo_skip(mfifo_t *self)
{
    ufifo_skip(self->priv);
}

static uint mfifo_put(mfifo_t *self, void *buf, uint size, long millisec)
{
    return ufifo_put_timeout(self->priv, buf, size, millisec);
}

static uint mfifo_get(mfifo_t *self, void *buf, uint size, long millisec)
{
    return ufifo_get_timeout(self->priv, buf, size, millisec);
}

static mfifo_t *mfifo_init(mfifo_init_t *init, ufifo_init_t *cfg)
{
    int ret;
    char name[64];
    char *prefix = NULL;
    mfifo_t *obj = NULL;

    if (init->type == MEDIA_VIDEO_STREAM) {
        prefix            = "media-video-stream";
        cfg->hook.recsize = video_stream_recsize;
        cfg->hook.rectag  = video_stream_rectag;
        cfg->hook.recput  = video_stream_recput;
        cfg->hook.recget  = video_stream_recget;
    } else if (init->type == MEDIA_VIDEO_FRAME) {
        prefix            = "media-video-frame";
        cfg->hook.recsize = video_frame_recsize;
        cfg->hook.rectag  = video_frame_rectag;
        cfg->hook.recput  = video_frame_recput;
        cfg->hook.recget  = video_frame_recget;
    } else {
        return NULL;
    }

    obj = malloc(sizeof(mfifo_t));
    if (obj == NULL) {
        return NULL;
    }

    memset(obj, 0, sizeof(mfifo_t));
    obj->destroy = mfifo_destroy;
    obj->size    = mfifo_size;
    obj->len     = mfifo_len;
    obj->oldest  = mfifo_oldest;
    obj->newest  = mfifo_newest;
    obj->skip    = mfifo_skip;
    obj->put     = mfifo_put;
    obj->get     = mfifo_get;

    snprintf(name, sizeof(name), "%s-%d-%d", prefix, init->chn, init->subchn);
    ret = ufifo_open(name, cfg, (ufifo_t **)&obj->priv);
    if (ret) {
        goto err;
    }

    return obj;

err:
    free(obj);
    return NULL;
}

mfifo_t *mfifo_create(mfifo_init_t *init, size_t size)
{
    if (init == NULL || size == 0) {
        return NULL;
    }

    ufifo_init_t cfg = {
        .lock  = UFIFO_LOCK_NONE,
        .opt   = UFIFO_OPT_ALLOC,
        .alloc = { .size = size },
    };

    return mfifo_init(init, &cfg);
}

mfifo_t *mfifo_attach(mfifo_init_t *init, int shared)
{
    if (init == NULL) {
        return NULL;
    }

    ufifo_init_t cfg = {
        .lock  = UFIFO_LOCK_NONE,
        .opt   = UFIFO_OPT_ATTACH,
        .attach = { .shared = shared },
    };

    return mfifo_init(init, &cfg);
}
