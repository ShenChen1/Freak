#include "que.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <pthread.h>

typedef struct {
    unsigned int depth;
    unsigned int len;

    unsigned int curRd;
    unsigned int curWr;

    pthread_mutex_t lock;
    pthread_cond_t condRd;
    pthread_cond_t condWr;

    ptr_t *elements;
} que_priv_t;

int que_create(int depth, que_t **que)
{
    int ret = 0;
    que_priv_t *priv = NULL;

    priv = malloc(sizeof(que_priv_t));
    if (priv == NULL) {
        return -ENOMEM;
    }

    memset(priv, 0, sizeof(que_priv_t));
    priv->depth = depth;
    priv->len = 0;
    priv->curRd = 0;
    priv->curWr = 0;
    priv->elements = malloc(sizeof(ptr_t) * depth);
    if (priv->elements == NULL) {
        ret = -ENOMEM;
        goto err;
    }

    ret |= pthread_mutex_init(&priv->lock, NULL);
    ret |= pthread_cond_init(&priv->condRd, NULL);
    ret |= pthread_cond_init(&priv->condWr, NULL);
    assert(!ret);

    *que = priv;
    return ret;

err:
    free(priv);
    return ret;
}

int que_delete(que_t *que)
{
    que_priv_t *priv = que;

    pthread_cond_destroy(&priv->condRd);
    pthread_cond_destroy(&priv->condWr);
    pthread_mutex_destroy(&priv->lock);
    free(priv);

    return 0;
}

int que_put(que_t *que, ptr_t val, int block)
{
    int ret = 0;
    que_priv_t *priv = que;

    pthread_mutex_lock(&priv->lock);
    while (1) {
        if (priv->len < priv->depth) {
            priv->elements[priv->curWr] = val;
            priv->curWr = (priv->curWr + 1) % priv->depth;
            priv->len++;

            ret = 0;
            pthread_cond_signal(&priv->condRd);
            break;
        } else {
            if (!block) {
                ret = -EAGAIN;
                break;
            }

            ret = pthread_cond_wait(&priv->condWr, &priv->lock);
            if (ret) {
                ret = -errno;
                break;
            }
        }
    }
    pthread_mutex_unlock(&priv->lock);

    return ret;
}

int que_get(que_t *que, ptr_t *val, int block)
{
    int ret = 0;
    que_priv_t *priv = que;

    pthread_mutex_lock(&priv->lock);
    while (1) {
        if (priv->len > 0) {
            *val = priv->elements[priv->curRd];
            priv->curRd = (priv->curRd + 1) % priv->depth;
            priv->len--;

            ret = 0;
            pthread_cond_signal(&priv->condWr);
            break;
        } else {
            if (!block) {
                ret = -EAGAIN;
                break;
            }

            ret = pthread_cond_wait(&priv->condRd, &priv->lock);
            if (ret) {
                ret = -errno;
                break;
            }
        }
    }
    pthread_mutex_unlock(&priv->lock);

    return ret;
}

int que_peek(que_t *que, ptr_t *val)
{
    int ret = 0;
    que_priv_t *priv = que;

    pthread_mutex_lock(&priv->lock);
    if (priv->len > 0) {
        *val = priv->elements[priv->curRd];
    }
    pthread_mutex_unlock(&priv->lock);

    return ret;
}

int que_len(que_t *que)
{
    int ret = 0;
    que_priv_t *priv = que;

    pthread_mutex_lock(&priv->lock);
    ret = priv->len;
    pthread_mutex_unlock(&priv->lock);

    return ret;
}

int que_isempty(que_t *que)
{
    int ret = 0;
    que_priv_t * priv = que;

    pthread_mutex_lock(&priv->lock);
    ret = priv->len == 0;
    pthread_mutex_unlock(&priv->lock);

    return ret;
}

int que_isfull(que_t *que)
{
    int ret = 0;
    que_priv_t *priv = que;

    pthread_mutex_lock(&priv->lock);
    ret = priv->len == priv->depth;
    pthread_mutex_unlock(&priv->lock);

    return ret;
}
