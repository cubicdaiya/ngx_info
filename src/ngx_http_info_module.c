
#include "ngx_http_info_common.h"
#include "ngx_http_info_response.h"

static void *ngx_http_info_create_main_conf(ngx_conf_t *cf);
static char *ngx_http_info_init_main_conf(ngx_conf_t *cf, void *conf);
static void *ngx_http_info_create_loc_conf(ngx_conf_t *cf);
static char *ngx_http_info_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child);
static char *ngx_http_info(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static ngx_int_t ngx_http_info_handler(ngx_http_request_t *r);

static ngx_command_t ngx_http_info_commands[] = {

    {
        ngx_string("nginx_info"),
        NGX_HTTP_LOC_CONF|NGX_CONF_NOARGS,
        ngx_http_info,
        0,
        0,
        NULL
    },

    { ngx_string("nginx_info_buffer"),
      NGX_HTTP_MAIN_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_size_slot,
      NGX_HTTP_MAIN_CONF_OFFSET,
      offsetof(ngx_http_info_main_conf_t, response_buffer_size),
      NULL
    },

    { ngx_string("nginx_info_format"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_info_loc_conf_t, response_format),
      NULL
    },

    ngx_null_command
};

static ngx_http_module_t ngx_http_info_module_ctx = {
    NULL,                              /* preconfiguration */
    NULL,                              /* postconfiguration */

    ngx_http_info_create_main_conf,    /* create main configuration */
    ngx_http_info_init_main_conf,      /* init main configuration */

    NULL,                              /* create server configuration */
    NULL,                              /* merge server configuration */

    ngx_http_info_create_loc_conf,     /* create location configuration */
    ngx_http_info_merge_loc_conf       /* merge location configuration */
};

ngx_module_t ngx_http_info_module = {
    NGX_MODULE_V1,
    &ngx_http_info_module_ctx, /* module context */
    ngx_http_info_commands,    /* module directives */
    NGX_HTTP_MODULE,           /* module type */
    NULL,                      /* init master */
    NULL,                      /* init module */
    NULL,                      /* init process */
    NULL,                      /* init thread */
    NULL,                      /* exit thread */
    NULL,                      /* exit process */
    NULL,                      /* exit master */
    NGX_MODULE_V1_PADDING
};

static void *ngx_http_info_create_main_conf(ngx_conf_t *cf)
{
    ngx_http_info_main_conf_t *imcf;

    imcf = ngx_palloc(cf->pool, sizeof(ngx_http_info_main_conf_t));
    if (imcf == NULL) {
        return NULL;
    }

    imcf->response_buffer_size = NGX_CONF_UNSET_SIZE;
    
    return imcf;
}

static char *ngx_http_info_init_main_conf(ngx_conf_t *cf, void *conf)
{
    ngx_http_info_main_conf_t *imcf;
    ngx_int_t rc;

    imcf = ngx_http_conf_get_module_main_conf(cf, ngx_http_info_module);

    if (imcf->response_buffer_size == NGX_CONF_UNSET_SIZE) {
        imcf->response_buffer_size = 1 * 1024 * 1024;
    }

    imcf->response.data = ngx_palloc(cf->pool, imcf->response_buffer_size);
    if (imcf->response.data == NULL) {
        return NGX_CONF_ERROR;
    }

    rc = ngx_http_info_build_response(&imcf->response, 
                                      imcf->response_buffer_size);
    if (rc != NGX_OK) {
        return "nginx_info_buffer is not enough";
    }

    return NGX_CONF_OK;
}

static void *ngx_http_info_create_loc_conf(ngx_conf_t *cf)
{
    ngx_http_info_loc_conf_t *ilcf;

    ilcf = ngx_pcalloc(cf->pool, sizeof(ngx_http_info_loc_conf_t));
    if (ilcf == NULL) {
        return NULL;
    }

    ngx_str_null(&ilcf->response_format);

    return ilcf;
}

static char *ngx_http_info_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child)
{
    ngx_http_info_loc_conf_t *prev = parent;
    ngx_http_info_loc_conf_t *conf = child;

    ngx_conf_merge_str_value(conf->response_format, prev->response_format, "text");

    return NGX_CONF_OK;
}

static char *ngx_http_info(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_core_loc_conf_t  *clcf;

    clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    clcf->handler = ngx_http_info_handler;

    return NGX_CONF_OK;
}

static ngx_int_t ngx_http_info_handler(ngx_http_request_t *r)
{
    ngx_http_info_main_conf_t *imcf;
    ngx_int_t                  rc;
    ngx_chain_t                out;
    ngx_buf_t                 *b;

    if (r->method != NGX_HTTP_GET && r->method != NGX_HTTP_HEAD) {
        return NGX_HTTP_NOT_ALLOWED;
    }

    b = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));
    if (b == NULL) {
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

    imcf = ngx_http_get_module_main_conf(r, ngx_http_info_module);

    b->pos      = imcf->response.data;
    b->last     = imcf->response.data + imcf->response.len;
    b->memory   = 1;
    b->last_buf = 1;
    out.buf     = b;
    out.next    = NULL;

    r->headers_out.content_type.len  = sizeof("text/plain") - 1;
    r->headers_out.content_type.data = (u_char *)"text/plain";
    r->headers_out.status            = NGX_HTTP_OK;
    r->headers_out.content_length_n  = imcf->response.len;

    rc = ngx_http_send_header(r);
    if (rc == NGX_ERROR || rc > NGX_OK || r->header_only) {
        return rc;
    }

    return ngx_http_output_filter(r, &out);
}
