#ifndef __NNM_H__
#define __NNM_H__

typedef void *nnm_t;
typedef int (*nnm_recv_cb_t)(void *buf, size_t len);
typedef int (*nnm_transfer_cb_t)(void *in, size_t isize, void **out, size_t *osize);

void *nnm_allocmsg(size_t size);
void *nnm_reallocmsg(void *msg, size_t size);
int nnm_free(void *msg);

int nnm_pull_create(const char* url, nnm_recv_cb_t callback, nnm_t *handle);
int nnm_pull_destory(nnm_t handle);

int nnm_push_create(const char* url, nnm_t *handle);
int nnm_push_destory(nnm_t handle);
int nnm_push_send(nnm_t handle, const void *buf, size_t len);

int nnm_rep_create(const char* url, nnm_transfer_cb_t callback, nnm_t *handle);
int nnm_rep_destory(nnm_t handle);

int nnm_req_create(const char* url, nnm_t *handle);
int nnm_req_destory(nnm_t handle);
int nnm_req_exchange(nnm_t handle, void *in, size_t isize, void **out, size_t *osize);

int nnm_pub_create(const char* url, nnm_t *handle);
int nnm_pub_destory(nnm_t handle);
int nnm_pub_send(nnm_t handle, void *buf, size_t len);

int nnm_sub_create(const char* url, nnm_recv_cb_t callback, nnm_t *handle);
int nnm_sub_close(nnm_t handle);

#endif /* __NNM_H__ */