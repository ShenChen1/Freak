#include "rtp-media.h"
#include "cstringext.h"
#include "librtp/rtp-internal.h"
#include "librtp/rtp-payload.h"
#include "librtp/rtp-profile.h"
#include "librtp/rtp.h"
#include "log.h"
#include "proto.h"
#include "media.h"
#include "rtp-transport.h"
#include "sys/locker.h"
#include "sys/thread.h"

enum {
    MEDIA_TRACK_VIDEO = 0, // a=control:video
    MEDIA_TRACK_AUDIO = 1, // a=control:audio
    MEDIA_TRACK_META  = 2, // a=control:meta
    MEDIA_TRACK_MAX,
};

enum {
    MEDIA_STATUS_SETUP = 0,
    MEDIA_STATUS_PLAY  = 1,
    MEDIA_STATUS_PAUSE = 2,
    MEDIA_STATUS_EXIT  = 3,
    MEDIA_STATUS_MAX,
};

typedef struct {
    void *rtp;
    void *packer;
    mfifo_t *fifo;
    uint8_t packet[64 * 1024];
    struct rtp_transport_t *transport;
} rtp_media_track_t;

typedef struct {
    struct rtp_media_t base;
    char path[64];
    uint8_t data[512 * 1024];
    pthread_t thread;
    int status;
    rtp_media_track_t track[MEDIA_TRACK_MAX];
} rtp_media_priv_t;

static void *__alloc(void *param, int bytes)
{
    rtp_media_track_t *track = param;
    assert(bytes < sizeof(track->packet));
    return track->packet;
}

static void __free(void *param, void *packet)
{
    rtp_media_track_t *track = param;
    assert(track->packet == packet);
    track = track;
}

static int __packet(void *param, const void *packet, int bytes, uint32_t timestamp, int flags)
{
    int ret;
    rtp_media_track_t *track = param;
    assert(track->packet == packet);

    ret = track->transport->send(track->transport, 0, (void *)packet, bytes);
    if (ret < 0) {
        return ret;
    }

    return rtp_onsend(track->rtp, packet, bytes);
}

static int rtp_send_data(void *arg)
{
    rtp_media_priv_t *priv = arg;

    while (1) {
        if (priv->status == MEDIA_STATUS_EXIT) {
            break;
        }
        if (priv->status != MEDIA_STATUS_PLAY) {
            usleep(40 * 1000);
            continue;
        }

        media_record_t *rec = (media_record_t *)priv->data;
        memset(rec, 0, sizeof(media_record_t));

        if (priv->track[MEDIA_TRACK_VIDEO].fifo) {
            priv->track[MEDIA_TRACK_VIDEO].fifo->get(priv->track[MEDIA_TRACK_VIDEO].fifo, priv->data, sizeof(priv->data), 1000);
            tracef("bytes:%u tag:0x%x ts:%llu", rec->size, rec->tag, rec->ts);
        }

        if (rec->size && priv->track[MEDIA_TRACK_VIDEO].packer) {
            int i;
            video_stream_t *stream = (void *)rec->buf;
            stream->pstPack = (void *)&rec->buf[sizeof(video_stream_t)];
            for (i = 0; i < stream->u32PackCount; i++) {
                if (i == 0) {
                    stream->pstPack[i].pu8Addr = (void *)&stream->pstPack[stream->u32PackCount];
                } else {
                    stream->pstPack[i].pu8Addr = stream->pstPack[i - 1].pu8Addr +
                                                 stream->pstPack[i - 1].u32Len -
                                                 stream->pstPack[i - 1].u32Offset;
                }
                rtp_payload_encode_input(
                    priv->track[MEDIA_TRACK_VIDEO].packer,
                    stream->pstPack[i].pu8Addr,
                    stream->pstPack[i].u32Len - stream->pstPack[i].u32Offset,
                    stream->pstPack[i].u64PTS * 90 / 1000 /*kHz*/);
            }
        }
    }

    return 0;
}

static int rtp_get_sdp(struct rtp_media_t *m, char *sdp)
{
    rtp_media_priv_t *priv = container_of(m, rtp_media_priv_t, base);
    tracef("m:%p sdp:%s", m, sdp);

    static const char *pattern_video =
        "m=video 0 RTP/AVP %d\n"
        "a=rtpmap:%d H264/90000\n"
        "a=range:npt=now-\n"
        "a=recvonly\n"
        "a=control:video\n"
        "a=fmtp:%d profile-level-id=%02X%02X%02X;packetization-mode=1;sprop-parameter-sets=";
    uint32_t ssrc = rtp_ssrc();

    if (!priv->track[MEDIA_TRACK_VIDEO].packer) {
        struct rtp_payload_t func = { __alloc, __free, __packet };
        priv->track[MEDIA_TRACK_VIDEO].packer = rtp_payload_encode_create(
            RTP_PAYLOAD_H264, "H264", (uint16_t)ssrc, ssrc, &func, &priv->track[MEDIA_TRACK_VIDEO]);
    }

    if (!priv->track[MEDIA_TRACK_VIDEO].fifo) {
        mfifo_init_t init = { .type = MEDIA_VIDEO_STREAM };
        int ret = sscanf(priv->path, "/%d-%d", &init.chn, &init.subchn);
        if (ret != 2) {
            return -1;
        }
        priv->track[MEDIA_TRACK_VIDEO].fifo = mfifo_attach(&init, 1);
        if (priv->track[MEDIA_TRACK_VIDEO].fifo == NULL) {
            return -1;
        }
    }

    if (!priv->track[MEDIA_TRACK_VIDEO].rtp) {
        struct rtp_event_t event           = { NULL };
        priv->track[MEDIA_TRACK_VIDEO].rtp = rtp_create(&event, NULL, ssrc, ssrc, 90000, 4 * 1024, 1);
        rtp_set_info(priv->track[MEDIA_TRACK_VIDEO].rtp, "RTSPServer", "live.h264");
    }

    sprintf(&sdp[strlen(sdp)], pattern_video, RTP_PAYLOAD_H264, RTP_PAYLOAD_H264, RTP_PAYLOAD_H264, 0, 0, 0);
    return 0;
}

static int rtp_get_duration(struct rtp_media_t *m, int64_t *duration)
{
    // rtp_media_priv_t* priv = container_of(m, rtp_media_priv_t, base);
    tracef("m:%p duration:%p", m, duration);
    return 0;
}

static int rtp_play(struct rtp_media_t *m)
{
    rtp_media_priv_t *priv = container_of(m, rtp_media_priv_t, base);
    tracef("m:%p", m);
    if (priv->track[MEDIA_TRACK_VIDEO].fifo) {
        priv->track[MEDIA_TRACK_VIDEO].fifo->newest(priv->track[MEDIA_TRACK_VIDEO].fifo);
    }
    priv->status = MEDIA_STATUS_PLAY;
    return 0;
}

static int rtp_pause(struct rtp_media_t *m)
{
    rtp_media_priv_t *priv = container_of(m, rtp_media_priv_t, base);
    tracef("m:%p", m);
    priv->status = MEDIA_STATUS_PAUSE;
    return 0;
}

static int rtp_set_speed(struct rtp_media_t *m, double speed)
{
    // rtp_media_priv_t* priv = container_of(m, rtp_media_priv_t, base);
    tracef("m:%p speed:%g", m, speed);
    return 0;
}

static int rtp_seek(struct rtp_media_t *m, int64_t pos)
{
    // rtp_media_priv_t* priv = container_of(m, rtp_media_priv_t, base);
    tracef("m:%p pos:%ld", m, pos);
    return 0;
}

static int rtp_get_rtpinfo(struct rtp_media_t *m, const char *uri, char *rtpinfo, size_t bytes)
{
    uint16_t seq = 0;
    uint32_t timestamp = 0;
    rtp_media_priv_t *priv = container_of(m, rtp_media_priv_t, base);
    tracef("m:%p uri:%p rtpinfo:%s bytes:%zu", m, uri, rtpinfo, bytes);

    if (priv->track[MEDIA_TRACK_VIDEO].packer) {
        rtp_payload_encode_getinfo(priv->track[MEDIA_TRACK_VIDEO].packer, &seq, &timestamp);
    }
    // url=rtsp://video.example.com/twister/video;seq=12312232;rtptime=78712811
    snprintf(rtpinfo, bytes, "url=%s;seq=%hu;rtptime=%u", uri, seq, timestamp);
    return 0;
}

static int rtp_add_transport(struct rtp_media_t *m, const char *track, void *t)
{
    int i;
    const char *key[]      = { "video", "audio", "meta" };
    const int value[]      = { MEDIA_TRACK_VIDEO, MEDIA_TRACK_AUDIO, MEDIA_TRACK_META };
    rtp_media_priv_t *priv = container_of(m, rtp_media_priv_t, base);
    tracef("m:%p track:%s t:%p", m, track, t);

    for (i = 0; i < MEDIA_TRACK_MAX; i++) {
        if (strstr(track, key[i])) {
            struct rtp_transport_t *tt      = priv->track[value[i]].transport;
            priv->track[value[i]].transport = t;
            if (tt) {
                tt->free(tt);
            }
            break;
        }
    }

    return i == MEDIA_TRACK_MAX;
}

struct rtp_media_t *rtp_media_live_new(char *path)
{
    rtp_media_priv_t *priv = malloc(sizeof(rtp_media_priv_t));
    if (priv == NULL) {
        return NULL;
    }

    memset(priv, 0, sizeof(rtp_media_priv_t));
    priv->base.get_sdp       = rtp_get_sdp;
    priv->base.get_duration  = rtp_get_duration;
    priv->base.play          = rtp_play;
    priv->base.pause         = rtp_pause;
    priv->base.set_speed     = rtp_set_speed;
    priv->base.seek          = rtp_seek;
    priv->base.get_rtpinfo   = rtp_get_rtpinfo;
    priv->base.add_transport = rtp_add_transport;
    priv->status             = MEDIA_STATUS_SETUP;
    strncpy(priv->path, path, sizeof(priv->path) - 1);
    thread_create(&priv->thread, rtp_send_data, priv);
    tracef("&priv->base:%p", &priv->base);
    return &priv->base;
}

int rtp_media_live_free(struct rtp_media_t *m)
{
    int i;
    rtp_media_priv_t *priv = container_of(m, rtp_media_priv_t, base);
    tracef("m:%p", m);

    priv->status = MEDIA_STATUS_EXIT;
    thread_destroy(priv->thread);

    for (i = 0; i < MEDIA_TRACK_MAX; i++) {
        if (priv->track[i].transport) {
            tracef("transport free");
            priv->track[i].transport->free(priv->track[i].transport);
        }
        if (priv->track[i].packer) {
            tracef("rtp_payload_encode_destroy");
            rtp_payload_encode_destroy(priv->track[i].packer);
        }
        if (priv->track[i].rtp) {
            tracef("rtp_destroy");
            rtp_destroy(priv->track[i].rtp);
        }
        if (priv->track[i].fifo) {
            tracef("ufifo_close");
            priv->track[i].fifo->destroy(priv->track[i].fifo);
        }
    }

    free(priv);
    tracef("m:%p done", m);
    return 0;
}