#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <nanomsg/nn.h>
#include <nanomsg/pubsub.h>
#include "nnm.h"

#define NNM_MAGIC   "NNM_PUBSUB"

typedef struct __nnm_pubsub {
    int             socket;
    nnm_recv_cb_t   callback;
    pthread_t       thread;
} nnm_pubsub_t;

static void fatal(const char *func)
{
    fprintf(stderr, "%s: %s\n", func, nn_strerror(nn_errno()));
    exit(1);
}

int nnm_pub_create(const char* url, nnm_t *handle)
{
    int sock;
    int ret;

    sock = nn_socket(AF_SP, NN_PUB);
    if (sock < 0) {
        fatal("nn_socket");
    }

    ret = nn_bind(sock, url);
    if (ret < 0) {
        fatal("nn_bind");
    }

    nnm_pubsub_t *self = malloc(sizeof(nnm_pubsub_t));
    if (self == NULL) {
        fatal("malloc");
    }

    memset(self, 0, sizeof(nnm_pubsub_t));
    self->socket = sock;

    *handle = self;
    return 0;
}

int nnm_pub_destory(nnm_t handle)
{
    nnm_pubsub_t *self = (nnm_pubsub_t *)handle;

    if (self == NULL) {
        return -EINVAL;
    }

    nn_shutdown(self->socket, 0);
    free(self);
    return 0;
}

int nnm_pub_send(nnm_t handle, void *buf, size_t len)
{
    int ret;
    nnm_pubsub_t *self = (nnm_pubsub_t *)handle;

    if (self == NULL || buf == NULL || len == 0) {
        return -EINVAL;
    }

    return nn_send(self->socket, buf, len, 0);
}

static void *nnm_sub_routine(void *arg)
{
    int bytes;
    char msg[] = NNM_MAGIC;
    nnm_pubsub_t *self = (nnm_pubsub_t *)arg;

    while(1) {
        char *buf = NULL;
        bytes = nn_recv(self->socket, &buf, NN_MSG, 0);
        if (bytes < 0) {
            fatal("nn_recv");
        }

        if (bytes == sizeof(msg) &&
            !memcmp(msg, buf, sizeof(msg))) {
            nn_freemsg(buf);
            break;
        }

        self->callback(buf, bytes);
        nn_freemsg(buf);
    }

    return NULL;
}

int nnm_sub_create(const char* url, nnm_recv_cb_t callback, nnm_t *handle)
{
    int sock;
    int ret;

    if (url == NULL || callback == NULL || handle == NULL) {
        return -EINVAL;
    }

    sock = nn_socket(AF_SP, NN_SUB);
    if (sock < 0) {
        fatal("nn_socket");
    }

    // subscribe to everything ("" means all topics)
    ret = nn_setsockopt(sock, NN_SUB, NN_SUB_SUBSCRIBE, "", 0);
    if (ret < 0) {
        fatal("nn_setsockopt");
    }

    ret = nn_connect(sock, url);
    if (ret < 0) {
        fatal("nn_connet");
    }

    nnm_pubsub_t *self = malloc(sizeof(nnm_pubsub_t));
    if (self == NULL) {
        fatal("malloc");
    }

    memset(self, 0, sizeof(nnm_pubsub_t));
    self->socket = sock;
    self->callback = callback;
    ret = pthread_create(&self->thread, NULL, nnm_sub_routine, (void *)self);
    if (ret < 0) {
        fatal("pthread_create");
    }

    *handle = self;
    return 0;
}

int nnm_sub_destory(nnm_t handle)
{
    char msg[] = NNM_MAGIC;
    nnm_pubsub_t *self = (nnm_pubsub_t *)handle;

    if (self == NULL) {
        return -EINVAL;
    }

    nn_send(self->socket, msg, sizeof(msg), 0);
    pthread_join(self->thread, NULL);
    nn_close(self->socket);
    free(self);
    return 0;
}
