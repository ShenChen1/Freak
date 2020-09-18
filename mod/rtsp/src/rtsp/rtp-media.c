#include "log.h"
#include "rtp-media.h"
#include "rtp-transport.h"
#include "cstringext.h"
#include "sys/thread.h"
#include "librtp/rtp-payload.h"
#include "librtp/rtp-profile.h"
#include "librtp/rtp.h"
#include "librtp/rtp-internal.h"
#include <unistd.h>

#define container_of(ptr, type, member) \
    (type*)((char*)(ptr) - (char*)&((type*)0)->member)

enum {
    MEDIA_TRACK_VIDEO = 0,  // a=control:video
    MEDIA_TRACK_AUDIO = 1,  // a=control:audio
    MEDIA_TRACK_META = 2,   // a=control:meta
    MEDIA_TRACK_MAX,
};

enum {
    MEDIA_STATUS_SETUP = 0,
    MEDIA_STATUS_PLAY = 1,
    MEDIA_STATUS_PAUSE = 2,
    MEDIA_STATUS_EXIT = 3,
    MEDIA_STATUS_MAX,
};

typedef struct {
    struct rtp_transport_t* transport;
    void* rtp;
    void* packer;
    char packet[2 * 1024];
} rtp_media_track_t;

typedef struct {
    struct rtp_media_t base;
    char data[512 * 1024];
    pthread_t thread;
    volatile int status;
    rtp_media_track_t track[MEDIA_TRACK_MAX];
} rtp_media_priv_t;

static void* __alloc(void* param, int bytes)
{
    rtp_media_track_t* track = param;
    assert(bytes < sizeof(track->packet));
    return track->packet;
}

static void __free(void* param, void* packet)
{
    rtp_media_track_t* track = param;
    assert(track->packet == packet);
}

static void __packet(void* param,
                     const void* packet,
                     int bytes,
                     uint32_t timestamp,
                     int flags)
{
    int ret;
    rtp_media_track_t* track = param;
    assert(track->packet == packet);
    ret = track->transport->send(track->transport, 0, (void *)packet, bytes);
    assert(ret == (int)bytes);
    rtp_onsend(track->rtp, packet, bytes);
}

static int rtp_send_data(void *arg)
{
    size_t nread = 0;
    uint32_t timestamp = 0;
    rtp_media_priv_t* priv = arg;

    char *path = "/mnt/hgfs/WinShare/200frames_count.h264";
    FILE *fp = fopen(path, "r");

    while (1) {

        if (priv->status == MEDIA_STATUS_EXIT) {
            break;
        }

        if (priv->status != MEDIA_STATUS_PLAY) {
            continue;
        }

        nread = fread(priv->data, 1, sizeof(priv->data), fp);
        if (nread <= 0) {
            break;
        }

        rtp_payload_encode_input(priv->track[MEDIA_TRACK_VIDEO].packer, priv->data, 1024, timestamp * 90 /*kHz*/);
        timestamp += 40;
    }

    fclose(fp);
    return 0;
}

static int rtp_get_sdp(struct rtp_media_t* m, char* sdp)
{
    rtp_media_priv_t* priv = container_of(m, rtp_media_priv_t, base);
    tracef("m:%p sdp:%s", m, sdp);

    static const char* pattern_video =
        "m=video 0 RTP/AVP %d\n"
        "a=rtpmap:%d H264/90000\n"
        "a=range:npt=now-\n"
        "a=recvonly\n"
        "a=control:video\n"
        "a=fmtp:%d profile-level-id=%02X%02X%02X;packetization-mode=1;sprop-parameter-sets=";

    uint32_t ssrc = rtp_ssrc();
    struct rtp_payload_t func = {__alloc, __free, __packet};
    priv->track[MEDIA_TRACK_VIDEO].packer = rtp_payload_encode_create(
        RTP_PAYLOAD_H264, "H264", (uint16_t)ssrc, ssrc, &func, &priv->track[MEDIA_TRACK_VIDEO]);
    struct rtp_event_t event = {NULL};
    priv->track[MEDIA_TRACK_VIDEO].rtp = rtp_create(&event, NULL, ssrc, 0, 90000, 4*1024, 1);
    rtp_set_info(priv->track[MEDIA_TRACK_VIDEO].rtp, "RTSPServer", "live.h264");

    sprintf(&sdp[strlen(sdp)], pattern_video, RTP_PAYLOAD_H264, RTP_PAYLOAD_H264, RTP_PAYLOAD_H264, 0, 0, 0);
    return 0;
}

static int rtp_get_duration(struct rtp_media_t* m, int64_t* duration)
{
    //rtp_media_priv_t* priv = container_of(m, rtp_media_priv_t, base);
    tracef("m:%p duration:%p", m, duration);

    return 0;
}

static int rtp_play(struct rtp_media_t* m)
{
    rtp_media_priv_t* priv = container_of(m, rtp_media_priv_t, base);
    tracef("m:%p", m);
    priv->status = MEDIA_STATUS_PLAY;
    return 0;
}

static int rtp_pause(struct rtp_media_t* m)
{
    rtp_media_priv_t* priv = container_of(m, rtp_media_priv_t, base);
    tracef("m:%p", m);
    priv->status = MEDIA_STATUS_PAUSE;
    return 0;
}

static int rtp_set_speed(struct rtp_media_t* m, double speed)
{
    // rtp_media_priv_t* priv = container_of(m, rtp_media_priv_t, base);
    tracef("m:%p speed:%g", m, speed);
    return 0;
}

static int rtp_seek(struct rtp_media_t* m, int64_t pos)
{
    // rtp_media_priv_t* priv = container_of(m, rtp_media_priv_t, base);
    tracef("m:%p pos:%ld", m, pos);
    return 0;
}

static int rtp_get_rtpinfo(struct rtp_media_t* m,
                           const char* uri,
                           char* rtpinfo,
                           size_t bytes)
{
    uint16_t seq;
    uint32_t timestamp;
    rtp_media_priv_t* priv = container_of(m, rtp_media_priv_t, base);
    tracef("m:%p uri:%p rtpinfo:%s bytes:%d", m, uri, rtpinfo, bytes);

    rtp_payload_encode_getinfo(priv->track[MEDIA_TRACK_VIDEO].packer, &seq, &timestamp);
    // url=rtsp://video.example.com/twister/video;seq=12312232;rtptime=78712811
    snprintf(rtpinfo, bytes, "url=%s;seq=%hu;rtptime=%u", uri, seq, timestamp);
    return 0;
}

static int rtp_add_transport(struct rtp_media_t* m, const char* track, void* t)
{
    int i;
    const char *key[] = {"video", "audio", "meta"};
    const int value[] = {MEDIA_TRACK_VIDEO, MEDIA_TRACK_AUDIO, MEDIA_TRACK_META};
    rtp_media_priv_t* priv = container_of(m, rtp_media_priv_t, base);
    tracef("m:%p track:%s t:%p", m, track, t);

    for (i = 0; i < MEDIA_TRACK_MAX; i++) {
        if (strstr(track, key[i])) {
            priv->track[value[i]].transport = t;
            break;
        }
    }

    return i == MEDIA_TRACK_MAX;
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

    priv->status = MEDIA_STATUS_SETUP;
    thread_create(&priv->thread, rtp_send_data, priv);
    tracef("&priv->base:%p", &priv->base);
    return &priv->base;
}

int rtp_media_live_free(struct rtp_media_t* m)
{
    int i;
    rtp_media_priv_t* priv = container_of(m, rtp_media_priv_t, base);
    tracef("m:%p", m);

    priv->status = MEDIA_STATUS_EXIT;
    thread_destroy(priv->thread);

    for (i = 0; i < MEDIA_TRACK_MAX; i++) {
        if (priv->track[i].transport) {
            priv->track[i].transport->free(priv->track[i].transport);
        }
        if (priv->track[i].packer) {
            rtp_payload_encode_destroy(priv->track[i].packer);
        }
        if (priv->track[i].rtp) {
            rtp_destroy(priv->track[i].rtp);
        }
    }

    free(priv);
    return 0;
}