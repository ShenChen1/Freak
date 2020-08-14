#ifndef __CFG_JSONB_C_API_H__
#define __CFG_JSONB_C_API_H__
#include "jsonb_c_type.h"
extern void jsonb_opt_gsf_bsp_board_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    char model[32];
    char sn[32];
    char chip[32];
} gsf_bsp_board_t;
extern void jsonb_opt_gsf_bsp_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    gsf_bsp_board_t board;
} gsf_bsp_t;
#endif /* __CFG_JSONB_C_API_H__ */
