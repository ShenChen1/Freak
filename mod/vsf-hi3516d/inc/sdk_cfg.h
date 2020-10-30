#ifndef __SDK_CFG_H__
#define __SDK_CFG_H__

#include "cfg/sdk_cfg.h"

#define sdk_cfg_get_member(member) \
({ \
    sdk_cfg_t *cfg = sdk_cfg_get(); \
    &cfg->member; \
})

int sdk_cfg_load(const char *path);
int sdk_cfg_save(const char *path);
void *sdk_cfg_get();

#endif /* __SDK_CFG_H__ */