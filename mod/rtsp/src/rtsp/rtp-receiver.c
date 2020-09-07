#define OS_LINUX
#include "librtp/rtcp-header.h"
#include "librtp/rtp-demuxer.h"
#include "librtp/rtp-payload.h"
#include "librtp/rtp-profile.h"
#include "librtp/rtp.h"
#include "sockutil.h"
#include "sys/pollfd.h"
#include "sys/thread.h"


struct rtp_receiver_t* rtp_udp_receiver_create(int rtp[2],
                                               const char* peer,
                                               int peerport[2],
                                               int payload,
                                               const char* encoding)
{
    return NULL;
}

struct rtp_receiver_t* rtp_tcp_receiver_create(uint8_t interleave1,
                                               uint8_t interleave2,
                                               int payload,
                                               const char* encoding)
{
    return NULL;
}