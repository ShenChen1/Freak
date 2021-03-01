#ifndef __MEDIA_H__
#define __MEDIA_H__

#include "common.h"
#include "video_frame.h"
#include "video_stream.h"

typedef struct {
    uint32_t size;
    uint32_t tag;
    uint64_t ts;
    uint8_t buf[0];
} media_record_t;

typedef enum {
    MEDIA_VIDEO_STREAM,
    MEDIA_VIDEO_FRAME,
} media_type_t;

typedef struct {
    int type;
    int chn;
    int subchn;
} mfifo_init_t;

typedef struct mfifo {
    void *priv;
    int (*destroy)(struct mfifo *self);

    uint (*size)(struct mfifo *self);
    uint (*len)(struct mfifo *self);

    void (*oldest)(struct mfifo *self);
    void (*newest)(struct mfifo *self);
    void (*skip)(struct mfifo *self);

    uint (*put)(struct mfifo *self, void *buf, uint size, long millisec);
    uint (*get)(struct mfifo *self, void *buf, uint size, long millisec);
} mfifo_t;

mfifo_t *mfifo_create(mfifo_init_t *init, size_t size);
mfifo_t *mfifo_attach(mfifo_init_t *init, int shared);

#endif /* __MEDIA_H__ */