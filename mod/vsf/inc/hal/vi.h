#ifndef __VI_H__
#define __VI_H__

typedef struct vi {
    /* Private date */
    void *priv;

    /**
     * @brief  Destroy vi instance
     *
     * @param  self     vi instance
     * @return 0 on successs, <0 otherwise.
     */
    int (*destroy)(struct vi *self);

} vi_t;

/**
 * @brief  Create vi instance
 *
 * @param  id           vi instance id
 * @return vi           created instance on success,
 *                      NULL on error
 */
vi_t *createVi(int id);

#endif //__VI_H__