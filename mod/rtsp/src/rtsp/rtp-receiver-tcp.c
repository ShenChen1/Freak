#include "rtp-receiver.h"
#include "librtp/rtcp-header.h"
#include "librtp/rtp-demuxer.h"
#include "librtp/rtp-payload.h"
#include "librtp/rtp-profile.h"
#include "librtp/rtp.h"
#include "sockutil.h"
#include "sys/pollfd.h"
#include "sys/thread.h"

#define container_of(ptr, type, member) \
    (type *)((char *)(ptr) - (char *) &((type *)0)->member)

typedef struct {
    struct rtp_receiver_t* receiver;

    char encoding[64];
    struct rtp_profile_t* profile;
    struct rtp_demuxer_t* demuxer;

} rtp_receiver_priv_t;

static void rtp_tcp_receiver_free(struct rtp_receiver_t *r)
{
    rtp_receiver_priv_t* priv = container_of(r, rtp_receiver_priv_t, receiver);

    rtp_demuxer_destroy(&priv->demuxer);
}

struct rtp_receiver_t* rtp_tcp_receiver_create(uint8_t interleave1,
                                               uint8_t interleave2,
                                               int payload,
                                               const char* encoding)
{
    rtp_receiver_priv_t* priv = NULL;

    priv = malloc(sizeof(rtp_receiver_priv_t));
    if (priv == NULL) {
        return NULL;
    }

    memset(priv, 0, sizeof(rtp_receiver_priv_t));
    priv->receiver->free = rtp_tcp_receiver_free;
    snprintf(priv->encoding, sizeof(priv->encoding), "%s", encoding);
    priv->profile = (void *)rtp_profile_find(payload);
    priv->demuxer = rtp_demuxer_create(priv->profile ? priv->profile->frequency : 90000, payload, encoding, NULL, NULL);

    return priv->receiver;
}