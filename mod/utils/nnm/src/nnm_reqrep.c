#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <nanomsg/nn.h>
#include <nanomsg/reqrep.h>
#include "nnm.h"

typedef struct __nnm_reqrep {
    const char         *url;
    int                 socket;
    nnm_trans_func_t    callback;
    void               *arg;
    pthread_t           thread;
} nnm_reqrep_t;

static void fatal(const char *func)
{
    fprintf(stderr, "%s: %s\n", func, nn_strerror(nn_errno()));
    exit(1);
}

static void *nnm_rep_routine(void *arg)
{
    int bytes;
    nnm_reqrep_t *self = (nnm_reqrep_t *)arg;

    while(1) {
        char *buf = NULL;
        bytes = nn_recv(self->socket, &buf, NN_MSG, 0);
        if (bytes < 0) {
            break;
        }

        void *outbuf = NULL;
        size_t outlen = 0;
        if (!self->callback(buf, bytes, &outbuf, &outlen, self->arg)) {
            bytes = nn_send(self->socket, outbuf, outlen, 0);
            if (bytes < 0) {
                fatal("nn_send");
            }
        }

        nn_freemsg(buf);
    }

    return NULL;
}

int nnm_rep_create(const char* url, nnm_rep_init_t *init, nnm_t *handle)
{
    int sock;
    int ret;

    if (url == NULL || init == NULL || handle == NULL) {
        return -EINVAL;
    }

    sock = nn_socket(AF_SP, NN_REP);
    if (sock < 0) {
        fatal("nn_socket");
    }

    ret = nn_bind(sock, url);
    if (ret < 0) {
        fatal("nn_bind");
    }

    nnm_reqrep_t *self = malloc(sizeof(nnm_reqrep_t));
    if (self == NULL) {
        fatal("malloc");
    }

    memset(self, 0, sizeof(nnm_reqrep_t));
    self->socket = sock;
    self->callback = init->func;
    self->arg = init->arg;
    ret = pthread_create(&self->thread, NULL, nnm_rep_routine, (void *)self);
    if (ret < 0) {
        fatal("pthread_create");
    }

    *handle = self;
    return 0;
}

int nnm_rep_destory(nnm_t handle)
{
    nnm_reqrep_t *self = (nnm_reqrep_t *)handle;

    if (self == NULL) {
        return -EINVAL;
    }

    nn_close(self->socket);
    pthread_join(self->thread, NULL);
    free(self);
    return 0;
}

int nnm_req_create(const char* url, nnm_t *handle)
{
    int sock;
    int ret;

    sock = nn_socket(AF_SP, NN_REQ);
    if (sock < 0) {
        fatal("nn_socket");
    }

    ret = nn_connect(sock, url);
    if (ret < 0) {
        fatal("nn_connect");
    }

    nnm_reqrep_t *self = malloc(sizeof(nnm_reqrep_t));
    if (self == NULL) {
        fatal("malloc");
    }

    memset(self, 0, sizeof(nnm_reqrep_t));
    self->socket = sock;

    *handle = self;
    return 0;
}

int nnm_req_destory(nnm_t handle)
{
    nnm_reqrep_t *self = (nnm_reqrep_t *)handle;

    if (self == NULL) {
        return -EINVAL;
    }

    nn_close(self->socket);
    free(self);
    return 0;
}

int nnm_req_exchange(nnm_t handle, void *in, size_t isize, void **out, size_t *osize)
{
    int ret;
    nnm_reqrep_t *self = (nnm_reqrep_t *)handle;

    if (self == NULL ||
        in == NULL || isize == 0 ||
        out == NULL || osize == 0) {
        return -EINVAL;
    }

    ret = nn_send(self->socket, in, isize, 0);
    if (ret < 0) {
        fatal("nn_send");
    }

    ret = nn_recv(self->socket, out, NN_MSG, 0);
    if (ret < 0) {
        fatal("nn_recv");
    }

    *osize = ret;
    return 0;
}
