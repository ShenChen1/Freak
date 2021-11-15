#include "log.h"
#include "mm.h"
#include "ufifo.h"
#include "media.h"

unsigned int video_frame_recsize(unsigned char *p1, unsigned int n1, unsigned char *p2)
{
    unsigned int size = sizeof(media_record_t);

    if (n1 >= size) {
        media_record_t *rec = (media_record_t *)p1;
        size                = rec->size;
    } else {
        media_record_t rec;
        char *p = (char *)(&rec);
        memcpy(p, p1, n1);
        memcpy(p + n1, p2, size - n1);
        size = rec.size;
    }

    tracef("size:%u", size);
    return size;
}

unsigned int video_frame_rectag(unsigned char *p1, unsigned int n1, unsigned char *p2)
{
    unsigned int tag;
    unsigned int size = sizeof(media_record_t);

    if (n1 >= size) {
        media_record_t *rec = (media_record_t *)p1;
        tag                 = rec->tag;
    } else {
        media_record_t rec;
        char *p = (char *)(&rec);
        memcpy(p, p1, n1);
        memcpy(p + n1, p2, size - n1);
        tag = rec.tag;
    }

    tracef("tag:%x", tag);
    return tag;
}

unsigned int video_frame_recput(unsigned char *p1, unsigned int n1, unsigned char *p2, void *arg)
{
    video_frame_t *frame = arg;
    size_t totalsize     = sizeof(media_record_t) + sizeof(video_frame_t) + (frame->u64ExtVirAddr[0] - frame->u64HeaderVirAddr[0]);
    media_record_t rec   = {};
    unsigned int a = 0, l = 0, _n1 = n1;
    unsigned char *p = NULL, *_p1 = p1, *_p2 = p2;

    rec.size = totalsize;
    rec.tag  = (0xdeadbeef << 8 | 1);
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    rec.ts = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;

    // copy header
    p = (unsigned char *)(&rec);
    a = sizeof(media_record_t);
    l = min(a, _n1);
    memcpy(_p1, p, l);
    memcpy(_p2, p + l, a - l);
    _n1 -= l;
    _p1 += l;
    _p2 += a - l;

    // copy info
    p = (unsigned char *)(frame);
    a = sizeof(video_frame_t);
    l = min(a, _n1);
    memcpy(_p1, p, l);
    memcpy(_p2, p + l, a - l);
    _n1 -= l;
    _p1 += l;
    _p2 += a - l;

    // copy data
    if(frame->size == 0)
    {
        uint32_t uSize = frame->u64ExtVirAddr[0] - frame->u64HeaderVirAddr[0];
        uint8_t *pMap = physmap(frame->u64PhyAddr[0], uSize);
        p = pMap;
        a = uSize;
        l = min(a, _n1);
        memcpy(_p1, p, l);
        memcpy(_p2, p + l, a - l);
        _n1 -= l;
        _p1 += l;
        _p2 += a - l;
        physunmap(pMap, uSize);
    }
    else
    {
        p = frame->ptr;
        a = frame->size;
        l = min(a, _n1);
        memcpy(_p1, p, l);
        memcpy(_p2, p + l, a - l);
        _n1 -= l;
        _p1 += l;
        _p2 += a - l;
    }


    tracef("frame:%u", frame->u32TimeRef);
    return totalsize;
}

unsigned int video_frame_recget(unsigned char *p1, unsigned int n1, unsigned char *p2, void *arg)
{
    media_record_t *rec = arg;
    unsigned int a = 0, l = 0, _n1 = n1;
    unsigned char *p = NULL, *_p1 = p1, *_p2 = p2;

    // copy header
    p = (unsigned char *)(rec);
    a = sizeof(media_record_t);
    l = min(a, _n1);
    memcpy(p, _p1, l);
    memcpy(p + _n1, _p2, a - l);
    _n1 -= l;
    _p1 += l;
    _p2 += a - l;

    // check header
    assert(rec->tag >= (0xdeadbeef << 8));

    // copy data
    p = (unsigned char *)(rec->buf);
    a = rec->size - sizeof(media_record_t);
    l = min(a, _n1);
    memcpy(p, _p1, l);
    memcpy(p + l, _p2, a - l);
    _n1 -= l;
    _p1 += l;
    _p2 += a - l;

    return rec->size;
}
