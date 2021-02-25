#include "common.h"
#include "librtp/rtcp-header.h"
#include "librtp/rtp-demuxer.h"
#include "librtp/rtp-payload.h"
#include "librtp/rtp-profile.h"
#include "librtp/rtp.h"
#include "log.h"
#include "rtp-receiver.h"
#include "sys/pollfd.h"
#include "sys/thread.h"

typedef struct {
    struct rtp_receiver_t base;

    char encoding[64];
    struct rtp_profile_t *profile;
    struct rtp_demuxer_t *demuxer;

} rtp_receiver_priv_t;

static int rtp_onpacket(void *param, const void *packet, int bytes, uint32_t timestamp, int flags)
{
    rtp_receiver_priv_t *priv = param;

    if (priv->base.frame) {
        priv->base.frame(priv->base.param, priv->encoding, packet, bytes, timestamp, flags);
    }

    return 0;
}

static void rtp_tcp_receiver_free(struct rtp_receiver_t *r)
{
    rtp_receiver_priv_t *priv = container_of(r, rtp_receiver_priv_t, base);
    rtp_demuxer_destroy(&priv->demuxer);
    free(priv);
}

static void rtp_tcp_receiver_input(struct rtp_receiver_t *r, uint8_t channel, const void *data, uint16_t bytes)
{
    rtp_receiver_priv_t *priv = container_of(r, rtp_receiver_priv_t, base);
    rtp_demuxer_input(priv->demuxer, data, bytes);
}

struct rtp_receiver_t *
rtp_tcp_receiver_create(uint8_t interleave1, uint8_t interleave2, int payload, const char *encoding)
{
    rtp_receiver_priv_t *priv = NULL;

    priv = malloc(sizeof(rtp_receiver_priv_t));
    if (priv == NULL) {
        return NULL;
    }

    memset(priv, 0, sizeof(rtp_receiver_priv_t));
    priv->base.free  = rtp_tcp_receiver_free;
    priv->base.input = rtp_tcp_receiver_input;
    snprintf(priv->encoding, sizeof(priv->encoding), "%s", encoding);
    priv->profile = (void *)rtp_profile_find(payload);
    priv->demuxer = rtp_demuxer_create(100,
                priv->profile ? priv->profile->frequency : 90000,
                payload, encoding, rtp_onpacket, priv);

    return &priv->base;
}