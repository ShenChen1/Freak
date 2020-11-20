#ifndef __MEDIA_H__
#define __MEDIA_H__

#include "video_frame.h"
#include "video_stream.h"

typedef struct {
    uint32_t size;
    uint32_t tag;
    size_t ts;
    uint8_t buf[0];
} media_record_t;

#endif /* __MEDIA_H__ */