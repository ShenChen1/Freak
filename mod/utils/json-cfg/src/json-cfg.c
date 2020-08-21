#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include "json-cfg.h"

int json_cfg_load(const char *filename, void *cfg, size_t size, jsonb_opt_func_t callback)
{
    int ret = 0;
    char *data = NULL;
    cJSON *json = NULL;
    FILE *fp = NULL;
    size_t len = 0;

    fp = fopen(filename, "rb");
    if (fp == NULL) {
        return -ENOENT;
    }

    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    data = malloc(len + 1);
    if (data == NULL) {
        ret = -ENOMEM;
        goto end1;
    }
    data[size] = '\0';

    if (fread(data, 1, len, fp) < len) {
        ret = -EIO;
        goto end2;
    }

    json = cJSON_Parse(data);
    if (json == NULL) {
        ret = -ENOMEM;
        goto end2;
    }

    callback(JSONB_OPT_J2S, json, cfg, size);
    cJSON_Delete(json);

end2:
    free(data);
end1:
    fclose(fp);

    return ret;
}

int json_cfg_save(const char *filename, void *cfg, size_t size, jsonb_opt_func_t callback)
{
    int ret = 0;
    char *data = NULL;
    cJSON *json = NULL;
    FILE *fp = NULL;

    fp = fopen(filename, "wb");
    if (fp == NULL) {
        return -ENOENT;
    }

    json = cJSON_CreateObject();
    if (json == NULL) {
        ret = -ENOMEM;
        goto end1;
    }

    callback(JSONB_OPT_S2J, json, cfg, size);
    data = cJSON_Print(json);
    if (json == NULL) {
        ret = -ENOMEM;
        goto end2;
    }

    fprintf(fp, "%s", data);
    cJSON_Delete(json);

end2:
    free(data);
end1:
    fclose(fp);

    return ret;
}