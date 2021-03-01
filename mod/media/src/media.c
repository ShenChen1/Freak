#include "media.h"
#include "ufifo.h"

extern unsigned int video_frame_recsize(unsigned char *p1, unsigned int n1, unsigned char *p2);
extern unsigned int video_frame_rectag(unsigned char *p1, unsigned int n1, unsigned char *p2);
extern unsigned int video_frame_recput(unsigned char *p1, unsigned int n1, unsigned char *p2, void *arg);
extern unsigned int video_frame_recget(unsigned char *p1, unsigned int n1, unsigned char *p2, void *arg);

extern unsigned int video_stream_recsize(unsigned char *p1, unsigned int n1, unsigned char *p2);
extern unsigned int video_stream_rectag(unsigned char *p1, unsigned int n1, unsigned char *p2);
extern unsigned int video_stream_recput(unsigned char *p1, unsigned int n1, unsigned char *p2, void *arg);
extern unsigned int video_stream_recget(unsigned char *p1, unsigned int n1, unsigned char *p2, void *arg);

mfifo_t *mfifo_create(mfifo_init_t *init, size_t size)
{
    return NULL;
}

mfifo_t *mfifo_attach(mfifo_init_t *init, int shared)
{
    return NULL;
}

