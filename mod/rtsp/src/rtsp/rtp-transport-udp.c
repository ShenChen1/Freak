#include "log.h"
#include "utils.h"
#include "cstringext.h"
#include "port/ip-route.h"
#include "rtp-transport.h"
#include "sockpair.h"
#include "sockutil.h"

typedef struct {
    struct rtp_transport_t base;
    socket_t socket[2];
    socklen_t addrlen[2];
    struct sockaddr_storage addr[2];
} rtp_udp_transport_t;

static int rtp_udp_transport_recv(struct rtp_transport_t* t, int rtcp, void* data, size_t bytes)
{
    rtp_udp_transport_t* transport = container_of(t, rtp_udp_transport_t, base);
    tracef("bytes:%zu", bytes);

    int i = rtcp ? 1 : 0;
    socklen_t fromlen;
    struct sockaddr from;
    return socket_recvfrom(transport->socket[i], data, bytes, 0, &from, &fromlen);
}

static int rtp_udp_transport_send(struct rtp_transport_t* t, int rtcp, void* data, size_t bytes)
{
    rtp_udp_transport_t* transport = container_of(t, rtp_udp_transport_t, base);
    tracef("bytes:%zu", bytes);

    int i = rtcp ? 1 : 0;
    return socket_sendto(transport->socket[i], data, bytes, 0, (void*)&transport->addr[i], transport->addrlen[i]);
}

static void rtp_udp_transport_free(struct rtp_transport_t* t)
{
    int i;
    rtp_udp_transport_t* transport = container_of(t, rtp_udp_transport_t, base);

    for (i = 0; i < 2; i++) {
        if (transport->socket[i])
            socket_close(transport->socket[i]);
        transport->socket[i] = socket_invalid;
    }
    free(transport);
}
struct rtp_transport_t* rtp_udp_transport_new(const char* ip, uint16_t port[2])
{
    int ret = 0;
    rtp_udp_transport_t* transport = NULL;

    transport = malloc(sizeof(rtp_udp_transport_t));
    if (transport == NULL) {
        return NULL;
    }

    memset(transport, 0, sizeof(rtp_udp_transport_t));
    transport->base.send = rtp_udp_transport_send;
    transport->base.recv = rtp_udp_transport_recv;
    transport->base.free = rtp_udp_transport_free;

    ret |= socket_addr_from(&transport->addr[0], &transport->addrlen[0], ip, port[0]);
    ret |= socket_addr_from(&transport->addr[1], &transport->addrlen[1], ip, port[1]);
    if (ret) {
        goto err;
    }

    char local[SOCKET_ADDRLEN];
    ret = ip_route_get(ip, local);
    ret = sockpair_create(ret ? NULL : local, transport->socket, port);
    if (ret) {
        goto err;
    }

    tracef("ip: %s", ip);
    tracef("port: %u %u", port[0], port[1]);
    socket_setnonblock(transport->socket[0], 1);
    socket_setnonblock(transport->socket[1], 1);
    return &transport->base;

err:
    free(transport);
    return NULL;
}
