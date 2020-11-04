#ifndef __RTSP_H__
#define __RTSP_H__

typedef enum {
    RTSP_PROTOCOL_TCP,
    RTSP_PROTOCOL_UDP,
} rtsp_client_protocol_t;

typedef struct {
    void *param;
    int (*onframe)(void *param, const char *encoding, const void *packet, int bytes, unsigned int time, int flags);
} rtsp_client_callback_t;

void *rtsp_server_init(const char *ip, int port);
int rtsp_server_uninit(void *rtsp);

void *rtsp_client_init(const char *url, rtsp_client_protocol_t protocol, rtsp_client_callback_t *cb);
int rtsp_client_uninit(void *rtsp);

#endif /* __RTSP_H__ */