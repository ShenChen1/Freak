#ifndef __MEDIA_H__
#define __MEDIA_H__

#include "video_frame.h"
#include "video_stream.h"

typedef enum {
    MEDIA_VIDEO_MAIN_STREAM,
    MEDIA_VIDEO_SECONDARY_STREAM,
    MEDIA_VIDEO_SNAP_STREAM,
    MEDIA_VIDEO_MAX_STREAM,
} media_video_subchn_type_e;

typedef struct {
    uint32_t size;
    uint32_t tag;
    size_t ts;
    uint8_t buf[0];
} media_record_t;

#endif /* __MEDIA_H__ */