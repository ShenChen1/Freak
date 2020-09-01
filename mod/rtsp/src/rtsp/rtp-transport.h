#ifndef __RTP_TRANSPORT_H__
#define __RTP_TRANSPORT_H__

#include <stdlib.h>
#include <stdint.h>

struct rtp_transport_t {
    int (*send)(struct rtp_transport_t* t, int rtcp, const void* data, size_t bytes);
    int (*recv)(struct rtp_transport_t* t, int rtcp, const void* data, size_t bytes);
    void (*free)(struct rtp_transport_t* t);
};

struct rtp_transport_t* rtp_udp_transport_new(const char* ip, uint16_t port[2]);
struct rtp_transport_t* rtp_tcp_transport_new(void* rtsp, uint8_t rtp, uint8_t rtcp);

#endif /* __RTP_TRANSPORT_H__ */