
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include "box.h"

box_t box_new(size_t num);
void box_free(box_t box);
int box_add(box_t box, uint32_t key, void *val);
int box_del(box_t box, uint32_t key);
void *box_get(box_t box, uint32_t key, void *def);

typedef struct {
    size_t  num;
    void   *box[0];
} box_priv_t;

box_t box_new(size_t num)
{
    int size;
    box_priv_t *priv = NULL;

    if (num == 0) {
        return NULL;
    }

    size = sizeof(box_priv_t) + num * sizeof(void *);
    priv = malloc(size);
    if (priv == NULL) {
        return NULL;
    }

    memset(priv, 0, size);
    priv->num = num;

    return priv;
}

void box_free(box_t box)
{
    free(box);
}

int box_add(box_t box, uint32_t key, void *val)
{
    box_priv_t *priv = box;

    if (box == NULL || key >= priv->num || val == NULL) {
        return -EINVAL;
    }

    priv->box[key] = val;
    return 0;
}

int box_del(box_t box, uint32_t key)
{
    box_priv_t *priv = box;

    if (box == NULL || key >= priv->num) {
        return -EINVAL;
    }

    priv->box[key] = NULL;
    return 0;
}

void *box_get(box_t box, uint32_t key, void *def)
{
    box_priv_t *priv = box;

    if (box == NULL || key >= priv->num) {
        return def;
    }

    if (priv->box[key] == NULL) {
        return def;
    }

    return priv->box[key];
}


