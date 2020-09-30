#ifndef __RTP_MEDIA_H__
#define __RTP_MEDIA_H__

#include <stdint.h>
#include <stddef.h>

struct rtp_media_t {
    int (*get_sdp)(struct rtp_media_t* m, char* sdp);
    int (*get_duration)(struct rtp_media_t* m, int64_t* duration);
    int (*play)(struct rtp_media_t* m);
    int (*pause)(struct rtp_media_t* m);
    int (*set_speed)(struct rtp_media_t* m, double speed);
    int (*seek)(struct rtp_media_t* m, int64_t pos);
    int (*get_rtpinfo)(struct rtp_media_t* m, const char* uri, char* rtpinfo, size_t bytes);
    int (*add_transport)(struct rtp_media_t* m, const char* track, void* transport);
};

struct rtp_media_t* rtp_media_live_new(char *path);
int rtp_media_live_free(struct rtp_media_t* m);

#endif /* __RTP_MEDIA_H__ */