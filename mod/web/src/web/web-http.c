#include "log.h"
#include "mongoose.h"
#include "web-media.h"

enum {
    HTTP_SERVER_STATUS_SETUP   = 0,
    HTTP_SERVER_STATUS_RUNNING = 1,
    HTTP_SERVER_STATUS_EXIT    = 2,
    HTTP_SERVER_STATUS_MAX,
};

typedef struct {
    char url[64];
    int status;
    pthread_t thread;
    struct mg_mgr mgr;
    struct mg_connection *nc;
} http_server_priv_t;

static int media_send_proc(void *data, int len, void *args)
{
    struct mg_mgr *mgr      = args;
    struct mg_connection *c = NULL;

    for (c = mg_next(mgr, NULL); c != NULL; c = mg_next(mgr, c)) {

        if (!(c->flags & MG_F_IS_WEBSOCKET)) {
            continue;
        }

        if (len <= 0) {
            // EOF is received. Schedule the connection to close
            c->flags |= MG_F_SEND_AND_CLOSE;
            if (c->send_mbuf.len <= 0 || c->send_mbuf.len >= 1024 * 1024) {
                c->flags |= MG_F_CLOSE_IMMEDIATELY;
            }
        } else {
            mg_send_websocket_frame(c, WEBSOCKET_OP_BINARY, data, len);
            infof("mg_send_websocket_frame:%d", len);
        }
    }

    return len;
}

static void ev_handler(struct mg_connection *nc, int ev, void *p)
{
    switch (ev) {
        case MG_EV_HTTP_REQUEST: {
            struct http_message *hm = p;
            infof("MG_EV_HTTP_REQUEST [%s:%d], nc:%p, user_data:%p\n",
                hm->message.p,
                hm->message.len,
                nc,
                nc->user_data);

            struct mg_serve_http_opts opts = {};
            opts.document_root             = "/var/www";
            mg_serve_http(nc, hm, opts);
            break;
        }

        case MG_EV_WEBSOCKET_HANDSHAKE_REQUEST: {
            struct http_message *hm = p;
            infof("MG_EV_WEBSOCKET_HANDSHAKE_REQUEST [%s:%d], nc:%p, user_data:%p\n",
                hm->message.p,
                hm->message.len,
                nc,
                nc->user_data);

            if (nc->user_data == NULL) {
                web_media_t *media = web_createMedia(hm->uri.p, hm->uri.len);
                web_media_cb_t cb  = {
                    .args = nc->mgr,
                    .func = media_send_proc,
                };
                media->regcallback(media, &cb);
                media->init(media);
                nc->user_data = media;
            }
            break;
        }

        case MG_EV_WEBSOCKET_FRAME: {
            struct websocket_message *wm = p;
            infof("MG_EV_WEBSOCKET_FRAME [%s:%d], nc:%p, user_data:%p\n",
                wm->data,
                wm->size,
                nc,
                nc->user_data);
            break;
        }

        case MG_EV_WEBSOCKET_HANDSHAKE_DONE: {
            struct http_message *hm = p;
            infof("MG_EV_WEBSOCKET_HANDSHAKE_DONE [%s:%d], nc:%p, user_data:%p\n",
                hm->message.p,
                hm->message.len,
                nc,
                nc->user_data);
            break;
        }

        case MG_EV_CLOSE: {
            infof("MG_EV_CLOSE [%p], nc:%p, user_data:%p\n", p, nc, nc->user_data);
            if (nc->user_data) {
                web_media_t *media = nc->user_data;
                media->destroy(media);
                nc->user_data = NULL;
            }
            break;
        }
    }
}

void *http_server_thread(void *args)
{
    http_server_priv_t *priv = args;

    mg_mgr_init(&priv->mgr, NULL);
    infof("Starting web server on %s\n", priv->url);

    priv->nc = mg_bind(&priv->mgr, priv->url, ev_handler);
    if (priv->nc == NULL) {
        errorf("Failed to create listener\n");
        return NULL;
    }

    // Set up HTTP server parameters
    mg_set_protocol_http_websocket(priv->nc);

    priv->status = HTTP_SERVER_STATUS_RUNNING;
    while (priv->status == HTTP_SERVER_STATUS_RUNNING) {
        mg_mgr_poll(&priv->mgr, 1000);
    }

    mg_mgr_free(&priv->mgr);
    return NULL;
}

void *http_server_init(const char *ip, int port)
{
    http_server_priv_t *priv = malloc(sizeof(http_server_priv_t));
    if (priv == NULL) {
        return NULL;
    }

    memset(priv, 0, sizeof(http_server_priv_t));
    priv->status = HTTP_SERVER_STATUS_SETUP;
    snprintf(priv->url, sizeof(priv->url), "%s:%d", ip, port);
    pthread_create(&priv->thread, NULL, http_server_thread, priv);

    return priv;
}

int http_server_uninit(void *http)
{
    http_server_priv_t *priv = http;

    priv->status = HTTP_SERVER_STATUS_EXIT;
    pthread_join(priv->thread, NULL);
    free(priv);
    return 0;
}