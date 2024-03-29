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

typedef struct {
    struct mg_mgr *mgr;
    struct mbuf buf;
    web_media_t *media;
} web_media_session_t;

static void on_media_read(struct mg_connection *nc, int ev, void *p)
{
    struct mg_connection *c = nc;
    web_media_session_t *session = *(void **)p;
    struct mbuf *buf = &session->buf;

    if (session != c->user_data) {
        return;
    }

    if (buf->len <= 0 || c->send_mbuf.len >= 1024 * 1024) {
        // EOF is received. Schedule the connection to close
        c->flags |= MG_F_SEND_AND_CLOSE;
        if (c->send_mbuf.len <= 0) {
            c->flags |= MG_F_CLOSE_IMMEDIATELY;
        }
    } else {
        tracef("mg_send_websocket_frame:%zu", buf->len);
        mg_send_websocket_frame(c, WEBSOCKET_OP_BINARY, buf->buf, buf->len);
        mbuf_clear(buf);
    }
}

static int media_send_pre(void *args)
{
    web_media_session_t *session = args;

    mbuf_init(&session->buf, 1024 * 64);
    return 0;
}

static int media_send_proc(void *data, int len, void *args)
{
    web_media_session_t *session = args;

    tracef("media_send_proc:%d", len);
    mbuf_append(&session->buf, data, len);
    mg_broadcast(session->mgr, on_media_read, &session, sizeof(void *));
    return len;
}

static int media_send_post(void *args)
{
    web_media_session_t *session = args;

    mbuf_free(&session->buf);
    free(session);
    return 0;
}

static web_media_session_t *media_session_new(char *path, void *mgr)
{
    web_media_session_t *session = NULL;

    session = malloc(sizeof(web_media_session_t));
    if (session == NULL) {
        return NULL;
    }

    session->mgr = mgr;
    session->media = web_createMedia(path);
    if (session->media == NULL) {
        free(session);
        return NULL;
    }

    web_media_cb_t cb = {
        .args = session,
        .pre  = media_send_pre,
        .proc = media_send_proc,
        .post = media_send_post,
    };
    session->media->regcallback(session->media, &cb);

    return session;
}

static void media_session_del(web_media_session_t *session)
{
    web_media_t *media = session->media;
    media->destroy(media);
}

static void ev_handler(struct mg_connection *nc, int ev, void *p)
{
    switch (ev) {
        case MG_EV_HTTP_REQUEST: {
            struct http_message *hm = p;
            infof("MG_EV_HTTP_REQUEST [%zu], nc:%p, user_data:%p\n",
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
            infof("MG_EV_WEBSOCKET_HANDSHAKE_REQUEST [%zu], nc:%p, user_data:%p\n",
                hm->message.len,
                nc,
                nc->user_data);
            break;
        }

        case MG_EV_WEBSOCKET_FRAME: {
            struct websocket_message *wm = p;
            infof("MG_EV_WEBSOCKET_FRAME [%zu], nc:%p, user_data:%p\n",
                wm->size,
                nc,
                nc->user_data);
            break;
        }

        case MG_EV_WEBSOCKET_HANDSHAKE_DONE: {
            struct http_message *hm = p;
            infof("MG_EV_WEBSOCKET_HANDSHAKE_DONE [%zu], nc:%p, user_data:%p\n",
                hm->message.len,
                nc,
                nc->user_data);
            if (nc->user_data == NULL) {
                char path[64] = {};
                memcpy(path, hm->uri.p, hm->uri.len);
                nc->user_data = media_session_new(path, nc->mgr);
                infof("media_session_new:%p", nc->user_data);
            }
            break;
        }

        case MG_EV_CLOSE: {
            infof("MG_EV_CLOSE [%p], nc:%p, user_data:%p\n", p, nc, nc->user_data);
            if (nc->user_data) {
                infof("media_session_del:%p", nc->user_data);
                media_session_del(nc->user_data);
                nc->user_data = NULL;
            }
            break;
        }
    }
}

void *http_server_thread(void *args)
{
    http_server_priv_t *priv = args;
    const char *err = NULL;

    mg_mgr_init(&priv->mgr, priv);
    infof("Starting web server on %s\n", priv->url);

    struct mg_bind_opts opts;
    memset(&opts, 0, sizeof(opts));
    opts.error_string = &err;
    priv->nc = mg_bind_opt(&priv->mgr, priv->url, ev_handler, opts);
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