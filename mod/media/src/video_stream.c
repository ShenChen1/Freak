#include "log.h"
#include "common.h"
#include "mm.h"
#include "ufifo.h"
#include "media.h"

unsigned int video_stream_recsize(unsigned char *p1, unsigned int n1, unsigned char *p2)
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

unsigned int video_stream_rectag(unsigned char *p1, unsigned int n1, unsigned char *p2)
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

unsigned int video_stream_recput(unsigned char *p1, unsigned int n1, unsigned char *p2, void *arg)
{
    int i;
    size_t totalsize       = sizeof(media_record_t) + sizeof(video_stream_t);
    media_record_t rec     = {};
    video_stream_t *stream = arg;
    unsigned int a = 0, l = 0, _n1 = n1;
    unsigned char *p = NULL, *_p1 = p1, *_p2 = p2;

    for (i = 0; i < stream->u32PackCount; i++) {
        totalsize += sizeof(video_stream_pack_t);
        totalsize += stream->pstPack[i].u32Len - stream->pstPack[i].u32Offset;
    }
    rec.size = totalsize;
    rec.tag  = (0xdeadbeef << 8 | (stream->u32PackCount > 1));
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

    // copy data
    p = (unsigned char *)(stream);
    a = sizeof(video_stream_t);
    l = min(a, _n1);
    memcpy(_p1, p, l);
    memcpy(_p2, p + l, a - l);
    _n1 -= l;
    _p1 += l;
    _p2 += a - l;

    for (i = 0; i < stream->u32PackCount; i++) {
        // copy data
        p = (unsigned char *)(&stream->pstPack[i]);
        a = sizeof(video_stream_pack_t);
        l = min(a, _n1);
        memcpy(_p1, p, l);
        memcpy(_p2, p + l, a - l);
        _n1 -= l;
        _p1 += l;
        _p2 += a - l;
    }

    for (i = 0; i < stream->u32PackCount; i++) {
        // copy data
        p = (unsigned char *)(stream->pstPack[i].pu8Addr  + stream->pstPack[i].u32Offset);
        a = stream->pstPack[i].u32Len - stream->pstPack[i].u32Offset;
        l = min(a, _n1);
        memcpy(_p1, p, l);
        memcpy(_p2, p + l, a - l);
        _n1 -= l;
        _p1 += l;
        _p2 += a - l;
    }

    return totalsize;
}

unsigned int video_stream_recget(unsigned char *p1, unsigned int n1, unsigned char *p2, void *arg)
{
    media_record_t *rec  = arg;
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


