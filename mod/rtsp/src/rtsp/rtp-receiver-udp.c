#include "rtp-receiver.h"
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