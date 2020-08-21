#ifndef __JSON_CFG_H__
#define __JSON_CFG_H__

#include "jsonb_c_type.h"

int json_cfg_load(const char *filename, void *cfg, size_t size, jsonb_opt_func_t callback);
int json_cfg_save(const char *filename, void *cfg, size_t size, jsonb_opt_func_t callback);

#endif /* __JSON_CFG_H__ */