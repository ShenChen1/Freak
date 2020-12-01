#ifndef __HTTP_SERVER_H__
#define __HTTP_SERVER_H__

void *http_server_init(const char *ip, int port);
int http_server_uninit(void *http);

#endif //__HTTP_SERVER_H__