#ifndef __RTSP_H__
#define __RTSP_H__

void* rtsp_server_init(const char* ip, int port);
int rtsp_server_uninit(void* rtsp);

void* rtsp_client_init(const char* url);
int rtsp_client_uninit(void* rtsp);

#endif /* __RTSP_H__ */