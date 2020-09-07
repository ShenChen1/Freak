#ifndef __RTP_RECEIVER_H__
#define __RTP_RECEIVER_H__

#include "ctypedef.h"

struct rtp_receiver_t {
    void (*free)(struct rtp_receiver_t *receiver);
};

struct rtp_receiver_t *rtp_udp_receiver_create(int rtp[2], const char* peer, int peerport[2], int payload, const char* encoding);
struct rtp_receiver_t *rtp_tcp_receiver_create(uint8_t interleave1, uint8_t interleave2, int payload, const char* encoding);

#endif /* __RTP_RECEIVER_H__ */