#ifndef __RTSP_CTRL_H__
#define __RTSP_CTRL_H__

int rtsp_ctrl_open(char *url);
int rtsp_ctrl_close(char *url);

void *rtsp_ctrl_thread(void *arg);
int rtsp_ctrl_init();
int rtsp_ctrl_uninit();

#endif /* __RTSP_CTRL_H__ */