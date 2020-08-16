#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <nanomsg/nn.h>
#include <nanomsg/pipeline.h>
#include "nnm.h"

#define NNM_MAGIC    "NNM_PIPELINE"

typedef struct __nnm_pipeline {
    int             socket;
    nnm_recv_cb_t   callback;
    pthread_t       thread;
} nnm_pipeline_t;

static void fatal(const char *func)
{
    fprintf(stderr, "%s: %s\n", func, nn_strerror(nn_errno()));
    exit(1);
}

static void *nnm_pull_routine(void *arg)
{
    int bytes;
    char msg[] = NNM_MAGIC;
    nnm_pipeline_t *self = (nnm_pipeline_t *)arg;

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

int nnm_pull_create(const char* url, nnm_recv_cb_t callback, nnm_t *handle)
{
    int sock;
    int ret;

    if (url == NULL || callback == NULL || handle == NULL) {
        return -EINVAL;
    }

    sock = nn_socket(AF_SP, NN_PULL);
    if (sock < 0) {
        fatal("nn_socket");
    }

    ret = nn_bind(sock, url);
    if (ret < 0) {
        fatal("nn_bind");
    }

    nnm_pipeline_t *self = malloc(sizeof(nnm_pipeline_t));
    if (self == NULL) {
        fatal("malloc");
    }

    memset(self, 0, sizeof(nnm_pipeline_t));
    self->socket = sock;
    self->callback = callback;
    ret = pthread_create(&self->thread, NULL, nnm_pull_routine, (void *)self);
    if (ret < 0) {
        fatal("pthread_create");
    }

    *handle = self;
    return 0;
}

int nnm_pull_destory(nnm_t handle)
{
    char msg[] = NNM_MAGIC;
    nnm_pipeline_t *self = (nnm_pipeline_t *)handle;

    if (self == NULL) {
        return -EINVAL;
    }

    nn_send(self->socket, msg, sizeof(msg), 0);
    pthread_join(self->thread, NULL);
    nn_close(self->socket);
    free(self);
    return 0;
}

int nnm_push_create(const char* url, nnm_t *handle)
{
    int sock;
    int ret;

    sock = nn_socket(AF_SP, NN_PUSH);
    if (sock < 0) {
        fatal("nn_socket");
    }

    ret = nn_connect(sock, url);
    if (ret < 0) {
        fatal("nn_connect");
    }

    nnm_pipeline_t *self = malloc(sizeof(nnm_pipeline_t));
    if (self == NULL) {
        fatal("malloc");
    }

    memset(self, 0, sizeof(nnm_pipeline_t));
    self->socket = sock;

    *handle = self;
    return 0;
}

int nnm_push_destory(nnm_t handle)
{
    nnm_pipeline_t *self = (nnm_pipeline_t *)handle;

    if (self == NULL) {
        return -EINVAL;
    }

    nn_shutdown(self->socket, 0);
    free(self);
    return 0;
}

int nnm_push_send(nnm_t handle, const void *buf, size_t len)
{
    nnm_pipeline_t *self = (nnm_pipeline_t *)handle;

    if (self == NULL || buf == NULL || len == 0) {
        return -EINVAL;
    }

    return nn_send(self->socket, buf, len, 0);
}
