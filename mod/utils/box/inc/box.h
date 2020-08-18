#ifndef __BOX_H__
#define __BOX_H__

#include <stdint.h>

typedef void * box_t;

box_t box_new(size_t num);
void box_free(box_t box);
int box_add(box_t box, uint32_t key, void *val);
int box_del(box_t box, uint32_t key);
void *box_get(box_t box, uint32_t key, void *def);

#endif /* __BOX_H__ */