#include "log.h"
#include "mongoose.h"

enum {
    HTTP_SERVER_STATUS_SETUP   = 0,
    HTTP_SERVER_STATUS_RUNNING = 1,
    HTTP_SERVER_STATUS_EXIT    = 3,
    HTTP_SERVER_STATUS_MAX,
};

typedef struct {
    char url[64];
    int status;
    pthread_t thread;
    struct mg_mgr mgr;
    struct mg_connection *nc;
} http_server_priv_t;

static void ev_handler(struct mg_connection *nc, int ev, void *p)
{
    if (ev == MG_EV_HTTP_REQUEST) {
        struct mg_serve_http_opts opts = {};
        opts.document_root             = "."; // Serve current directory
        opts.enable_directory_listing  = "yes";
        mg_serve_http(nc, (struct http_message *)p, opts);
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