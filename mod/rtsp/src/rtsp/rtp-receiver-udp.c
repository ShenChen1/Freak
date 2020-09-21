#include "log.h"
#include "common.h"
#include "rtp-receiver.h"
#include "librtp/rtcp-header.h"
#include "librtp/rtp-demuxer.h"
#include "librtp/rtp-payload.h"
#include "librtp/rtp-profile.h"
#include "librtp/rtp.h"
#include "sockutil.h"
#include "sys/pollfd.h"
#include "sys/thread.h"

typedef struct {
    struct rtp_receiver_t base;
    int loop;
    pthread_t thread;

    char encoding[64];
    struct rtp_profile_t* profile;
    struct rtp_demuxer_t* demuxer;

    socket_t socket[2];
    struct sockaddr_storage ss[2];
    char rtp_buffer[32 * 1024];
    char rtcp_buffer[32 * 1024];

} rtp_receiver_priv_t;

static int rtp_read(rtp_receiver_priv_t* priv, socket_t s)
{
    int r;
    struct sockaddr_storage ss;
    socklen_t len = sizeof(ss);

    r = socket_recvfrom(s, priv->rtp_buffer, sizeof(priv->rtp_buffer), 0,
                        (struct sockaddr*)&ss, &len);
    if (r < 12)
        return -1;
    assert(0 == socket_addr_compare((const struct sockaddr*)&ss,
                                    (const struct sockaddr*)&priv->ss[0]));

    tracef("rtp_demuxer_input:%zu", len);
    return rtp_demuxer_input(priv->demuxer, priv->rtp_buffer, r);
}

static int rtcp_read(rtp_receiver_priv_t* priv, socket_t s)
{
    int r;
    socklen_t len;
    struct sockaddr_storage ss;
    len = sizeof(ss);
    r = socket_recvfrom(s, priv->rtcp_buffer, sizeof(priv->rtcp_buffer), 0,
                        (struct sockaddr*)&ss, &len);
    if (r < 12)
        return -1;
    assert(0 == socket_addr_compare((const struct sockaddr*)&ss,
                                    (const struct sockaddr*)&priv->ss[1]));

    tracef("rtp_demuxer_input:%zu", len);
    return rtp_demuxer_input(priv->demuxer, priv->rtcp_buffer, r);
}

static int rtp_receiver(rtp_receiver_priv_t* priv, socket_t rtp[2], int timeout)
{
    int i, r = 0;
    struct pollfd fds[2];

    for (i = 0; i < 2; i++) {
        fds[i].fd = rtp[i];
        fds[i].events = POLLIN;
        fds[i].revents = 0;
    }

    priv->loop = 1;
    while (priv->loop) {
        // RTCP report
        r = rtp_demuxer_rtcp(priv->demuxer, priv->rtcp_buffer,
                             sizeof(priv->rtcp_buffer));
        if (r > 0)
            r = socket_sendto(
                rtp[1], priv->rtcp_buffer, r, 0,
                (const struct sockaddr*)&priv->ss[1],
                socket_addr_len((const struct sockaddr*)&priv->ss[1]));

        r = poll(fds, 2, timeout);
        while (-1 == r && EINTR == errno)
            r = poll(fds, 2, timeout);

        if (0 == r) {
            continue;  // timeout
        } else if (r < 0) {
            return r;  // error
        } else {
            if (0 != fds[0].revents) {
                rtp_read(priv, rtp[0]);
                fds[0].revents = 0;
            }

            if (0 != fds[1].revents) {
                rtcp_read(priv, rtp[1]);
                fds[1].revents = 0;
            }
        }
    }
    return r;
}

static void rtp_onpacket(void* param,
                         const void* packet,
                         int bytes,
                         uint32_t timestamp,
                         int flags)
{
    rtp_receiver_priv_t* priv = param;

    if (priv->base.frame) {
        priv->base.frame(priv->base.param, priv->encoding, packet, bytes, timestamp, flags);
    }
}

static int rtp_worker(void* param)
{
    rtp_receiver_priv_t* priv = param;
    return rtp_receiver(priv, priv->socket, 2000);
}

static void rtp_udp_receiver_free(struct rtp_receiver_t* r)
{
    rtp_receiver_priv_t* priv = container_of(r, rtp_receiver_priv_t, base);

    priv->loop = 0;
    thread_destroy(priv->thread);
    rtp_demuxer_destroy(&priv->demuxer);
    free(priv);
}

struct rtp_receiver_t* rtp_udp_receiver_create(int rtp[2],
                                               const char* peer,
                                               int peerport[2],
                                               int payload,
                                               const char* encoding)
{
    rtp_receiver_priv_t* priv = NULL;

    priv = malloc(sizeof(rtp_receiver_priv_t));
    if (priv == NULL) {
        return NULL;
    }

    memset(priv, 0, sizeof(rtp_receiver_priv_t));
    priv->base.free = rtp_udp_receiver_free;
    snprintf(priv->encoding, sizeof(priv->encoding), "%s", encoding);
    priv->profile = (void*)rtp_profile_find(payload);
    priv->demuxer = rtp_demuxer_create(priv->profile ?
                    priv->profile->frequency : 90000,
                    payload, encoding, rtp_onpacket, priv);

    assert(0 == socket_addr_from(&priv->ss[0],
           NULL, peer, (uint16_t)peerport[0]));
    assert(0 == socket_addr_from(&priv->ss[1],
           NULL, peer, (uint16_t)peerport[1]));
    priv->socket[0] = rtp[0];
    priv->socket[1] = rtp[1];

    tracef("ip: %s", peer);
    tracef("port: %u %u", peerport[0], peerport[1]);
    thread_create(&priv->thread, rtp_worker, priv);
    return &priv->base;
}