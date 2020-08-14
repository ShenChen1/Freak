#ifndef __CFG_JSONB_C_API_H__
#define __CFG_JSONB_C_API_H__
#include "jsonb_c_type.h"
extern void jsonb_opt_gsf_board_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    char model[32];
    char sn[32];
    char type[32];
    int snscnt;
    char sensor[8][32];
    uint64_t caps;
} gsf_board_t;
#endif /* __CFG_JSONB_C_API_H__ */
