#ifndef __CFG_H__
#define __CFG_H__

#include "cfg/cfg.h"

#define cfg_get_member(member)      \
    ({                              \
        vsf_cfg_t *cfg = cfg_get(); \
        &cfg->member;               \
    })

int cfg_load(const char *path);
int cfg_save(const char *path);
void *cfg_get();

#endif /* __CFG_H__ */
