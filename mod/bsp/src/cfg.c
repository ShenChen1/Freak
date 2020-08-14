#include <stdio.h>
#include "cJSON.h"
#include "cfg/cfg.h"

int json_load(char* filename, gsf_bsp_t* cfg)
{
    if (access(filename, 0))
        return -1;


}

int json_save(char *filename, gsf_bsp_t *cfg)
{

}