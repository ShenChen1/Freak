#include "cstringext.h"
#include "rtp-transport.h"
#include "librtsp/rtsp-server.h"

#define container_of(ptr, type, member) \
    (type *)((char *)(ptr) - (char *) &((type *)0)->member)

typedef struct {
    struct rtp_transport_t base;
    void* rtsp;
    uint8_t rtp;
    uint8_t rtcp;
    uint8_t packet[4 + (1 << 16)];
} rtp_tcp_transport_t;


static int rtp_tcp_transport_recv(struct rtp_transport_t* t, int rtcp, void* data, size_t bytes)
{
    return 0;
}

static int rtp_tcp_transport_send(struct rtp_transport_t* t, int rtcp, void* data, size_t bytes)
{
    int ret;
    rtp_tcp_transport_t* transport = container_of(t, rtp_tcp_transport_t, base);

    transport->packet[0] = '$';
    transport->packet[1] = rtcp ? transport->rtcp : transport->rtp;
    transport->packet[2] = (bytes >> 8) & 0xFF;
    transport->packet[3] = bytes & 0xff;
    memcpy(transport->packet + 4, data, bytes);
    ret = rtsp_server_send_interleaved_data(transport->rtsp, transport->packet, bytes + 4);
    return 0 == ret ? bytes : ret;
}

static void rtp_tcp_transport_free(struct rtp_transport_t* t)
{
    rtp_tcp_transport_t* transport = container_of(t, rtp_tcp_transport_t, base);
    free(transport);
}

struct rtp_transport_t* rtp_tcp_transport_new(void* rtsp, uint8_t rtp, uint8_t rtcp)
{
    rtp_tcp_transport_t* transport = NULL;

    transport = malloc(sizeof(rtp_tcp_transport_t));
    if (transport == NULL) {
        return NULL;
    }

    memset(transport, 0, sizeof(rtp_tcp_transport_t));
    transport->base.send = rtp_tcp_transport_send;
    transport->base.recv = rtp_tcp_transport_recv;
    transport->base.free = rtp_tcp_transport_free;
    transport->rtsp = rtsp;
    transport->rtp = rtp;
    transport->rtcp = rtcp;

    return &transport->base;
}