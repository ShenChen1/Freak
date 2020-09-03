#include "cstringext.h"
#include "rtp-media.h"

#define container_of(ptr, type, member) \
    (type *)((char *)(ptr) - (char *) &((type *)0)->member)

enum {
    MEDIA_TRACK_VIDEO = 0, // a=control:video
    MEDIA_TRACK_AUDIO = 1, // a=control:audio
    MEDIA_TRACK_META  = 2, // a=control:meta
    MEDIA_TRACK_BUTT,
};

typedef struct {
    struct rtp_media_t base;
    struct rtp_transport_t* rtp_transport[MEDIA_TRACK_BUTT];
} rtp_media_priv_t;


static int rtp_get_sdp(struct rtp_media_t* m, char *sdp)
{
    //rtp_media_priv_t* priv = container_of(m, rtp_media_priv_t, base);
    return 0;
}

static int rtp_get_duration(struct rtp_media_t* m, int64_t* duration)
{
    //rtp_media_priv_t* priv = container_of(m, rtp_media_priv_t, base);
    return 0;
}

static int rtp_play(struct rtp_media_t* m)
{
    //rtp_media_priv_t* priv = container_of(m, rtp_media_priv_t, base);
    return 0;
}

static int rtp_pause(struct rtp_media_t* m)
{
    //rtp_media_priv_t* priv = container_of(m, rtp_media_priv_t, base);
    return 0;
}

static int rtp_set_speed(struct rtp_media_t* m, double speed)
{
    //rtp_media_priv_t* priv = container_of(m, rtp_media_priv_t, base);
    return 0;
}

static int rtp_seek(struct rtp_media_t* m, int64_t pos)
{
    //rtp_media_priv_t* priv = container_of(m, rtp_media_priv_t, base);
    return 0;
}

static int rtp_get_rtpinfo(struct rtp_media_t* m, const char* uri, char *rtpinfo, size_t bytes)
{
    //rtp_media_priv_t* priv = container_of(m, rtp_media_priv_t, base);
    return 0;
}

static int rtp_add_transport(struct rtp_media_t* m, const char* track, void* t)
{
    //rtp_media_priv_t* priv = container_of(m, rtp_media_priv_t, base);
    return 0;
}

struct rtp_media_t* rtp_media_live_new()
{
    rtp_media_priv_t* priv = malloc(sizeof(rtp_media_priv_t));
    if (priv == NULL) {
        return NULL;
    }

    memset(priv, 0, sizeof(rtp_media_priv_t));
    priv->base.get_sdp = rtp_get_sdp;
    priv->base.get_duration = rtp_get_duration;
    priv->base.play = rtp_play;
    priv->base.pause = rtp_pause;
    priv->base.set_speed = rtp_set_speed;
    priv->base.seek = rtp_seek;
    priv->base.get_rtpinfo = rtp_get_rtpinfo;
    priv->base.add_transport = rtp_add_transport;

    return &priv->base;
}

int rtp_media_live_free(struct rtp_media_t* m)
{
    rtp_media_priv_t* priv = container_of(m, rtp_media_priv_t, base);

    free(priv);
    return 0;
}