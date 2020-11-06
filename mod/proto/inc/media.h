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

#endif /* __MEDIA_H__ */