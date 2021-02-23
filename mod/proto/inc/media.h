#ifndef __MEDIA_H__
#define __MEDIA_H__

#include "video_frame.h"
#include "video_stream.h"

typedef struct {
    uint32_t size;
    uint32_t tag;
    uint64_t ts;
    uint8_t buf[0];
} media_record_t;

static inline unsigned short argb8888_1555(unsigned int color)
{
    unsigned char a = (color >> 24) & 0xff;
    unsigned char r = (color >> 16) & 0xff;
    unsigned char g = (color >> 8) & 0xff;
    unsigned char b = color & 0xff;

    a = a ? 1 : 0;
    r >>= 3;
    g >>= 3;
    b >>= 3;

    return (unsigned short)(a << 15 | r << 10 | g << 5 | b);
}

static inline unsigned int argb8888_888(unsigned int color)
{
    return color & 0xffffff;
}

#endif /* __MEDIA_H__ */