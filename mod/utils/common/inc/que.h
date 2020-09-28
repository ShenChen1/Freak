#ifndef __QUE_H__
#define __QUE_H__

typedef void que_t;
typedef void *ptr_t;

int que_create(int depth, que_t **que);
int que_delete(que_t *que);
int que_put(que_t *que, ptr_t val, int block);
int que_get(que_t *que, ptr_t *val, int block);
int que_peek(que_t *que, ptr_t *val);
int que_len(que_t *que);
int que_isempty(que_t *que);
int que_isfull(que_t *que);

#endif /* __QUE_H__ */