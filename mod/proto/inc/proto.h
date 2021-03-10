#ifndef __PROTO_H__
#define __PROTO_H__

#include "common.h"
#include "node.h"
#include "path.h"
#include "proto_hdr.h"

#define proto_header_dump(_package)                               \
    (void)({                                                      \
        proto_header_t *p = (void *)(_package);                   \
        infof("----------------------");                          \
        infof("- version:   %u", p->version);                     \
        infof("- ts:        %llu", p->ts);                        \
        infof("- chn:       %u", p->chn);                         \
        infof("- key:       %u", p->key);                         \
        if (p->action == PROTO_ACTION_SET)                        \
            infof("- action:    %s", "SET");                      \
        if (p->action == PROTO_ACTION_GET)                        \
            infof("- action:    %s", "GET");                      \
        if (p->action == PROTO_ACTION_CAP)                        \
            infof("- action:    %s", "CAP");                      \
        if (p->action == PROTO_ACTION_NUM)                        \
            infof("- action:    %s", "NUM");                      \
        if (p->action == PROTO_ACTION_TGR)                        \
            infof("- action:    %s", "TGR");                      \
        infof("- errcode:   %u", p->errcode);                     \
        infof("- format:    %s", p->format ? "JSON" : "STRUCTE"); \
        infof("- size:      %u", p->size);                        \
        if (p->format == PROTO_FORMAT_JSON)                       \
            infof("%s", p->data);                                 \
        infof("----------------------");                          \
    })

#define proto_package_fill(_package, _chn, _key, _action, _format, _data, _size) \
    (void)({                                                                     \
        proto_header_t *p = (void *)(_package);                                  \
        struct timespec _ts;                                                     \
        clock_gettime(CLOCK_MONOTONIC, &_ts);                                    \
        p->version = 1;                                                          \
        p->format  = 1;                                                          \
        p->ts      = _ts.tv_sec * 1000 + _ts.tv_nsec / 1000000;                  \
        p->chn     = (_chn);                                                     \
        p->key     = (_key);                                                     \
        p->action  = (_action);                                                  \
        p->errcode = 0;                                                          \
        p->format  = (_format);                                                  \
        p->size    = (_size);                                                    \
        memcpy(p->data, (_data), (_size));                                       \
    })

#define proto_package_fill_header(_package, _chn, _key, _action, _format, _size) \
    (void)({                                                                     \
        proto_header_t *p = (void *)(_package);                                  \
        struct timespec _ts;                                                     \
        clock_gettime(CLOCK_MONOTONIC, &_ts);                                    \
        p->version = 1;                                                          \
        p->format  = 1;                                                          \
        p->ts      = _ts.tv_sec * 1000 + _ts.tv_nsec / 1000000;                  \
        p->chn     = (_chn);                                                     \
        p->key     = (_key);                                                     \
        p->action  = (_action);                                                  \
        p->errcode = 0;                                                          \
        p->format  = (_format);                                                  \
        p->size    = (_size);                                                    \
    })

#define proto_package_size(_package)            \
    ({                                          \
        proto_header_t *p = (void *)(_package); \
        p->size + sizeof(proto_header_t);       \
    })

#define proto_package_data(_package)            \
    (void *)({                                  \
        proto_header_t *p = (void *)(_package); \
        p->data;                                \
    })

#define proto_client_data_pre(proto, func, in, isize, out, osize)         \
    {                                                                     \
        if (proto == PROTO_FORMAT_JSON) {                                 \
            cJSON *json = cJSON_CreateObject();                           \
            func(JSONB_OPT_S2J, json, in, isize);                         \
            cJSON_PrintPreallocated(json, out, PROTO_PACKAGE_MAXSIZE, 0); \
            cJSON_Delete(json);                                           \
            *osize = strlen((char *)out) + 1;                             \
        } else {                                                          \
            memcpy(out, in, isize);                                       \
            *osize = isize;                                               \
        }                                                                 \
    }

#define proto_client_data_post(proto, func, in, isize, out, osize) \
    {                                                              \
        if (proto == PROTO_FORMAT_JSON) {                          \
            assert(isize == strlen((char *)in) + 1);               \
            cJSON *json = cJSON_Parse((const char *)in);           \
            func(JSONB_OPT_J2S, json, out, osize);                 \
            cJSON_Delete(json);                                    \
        } else {                                                   \
            assert(isize == osize);                                \
            memcpy(out, in, isize);                                \
        }                                                          \
    }

#define proto_server_data_pre(proto, func, in, isize, out, osize) \
    {                                                             \
        if (proto == PROTO_FORMAT_JSON) {                         \
            assert(isize == strlen((char *)in) + 1);              \
            cJSON *json = cJSON_Parse((const char *)in);          \
            func(JSONB_OPT_J2S, json, out, osize);                \
            cJSON_Delete(json);                                   \
        } else {                                                  \
            assert(isize == osize);                               \
            memcpy(out, in, isize);                               \
        }                                                         \
    }

#define proto_server_data_post(proto, func, in, isize, out, osize)        \
    {                                                                     \
        if (proto == PROTO_FORMAT_JSON) {                                 \
            cJSON *json = cJSON_CreateObject();                           \
            func(JSONB_OPT_S2J, json, in, isize);                         \
            cJSON_PrintPreallocated(json, out, PROTO_PACKAGE_MAXSIZE, 0); \
            cJSON_Delete(json);                                           \
            *osize = strlen((char *)out) + 1;                             \
        } else {                                                          \
            memcpy(out, in, isize);                                       \
            *osize = isize;                                               \
        }                                                                 \
    }

#endif /* __PROTO_H__ */