
#include <nginx.h>
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

typedef struct ngx_http_info_main_conf_t {
    ngx_str_t response;
    size_t response_buffer_size;
} ngx_http_info_main_conf_t;

typedef enum ngx_http_info_key {
    NGX_HTTP_INFO_KEY_VERSION = 0,
    NGX_HTTP_INFO_KEY_PREFIX,
    NGX_HTTP_INFO_KEY_CONF_PREFIX,
#ifdef NGX_SBIN_PATH
    NGX_HTTP_INFO_KEY_SBIN_PATH,
#endif /* NGX_SBIN_PATH */
#ifdef NGX_CONF_PATH
    NGX_HTTP_INFO_KEY_CONF_PATH,
#endif /* NGX_CONF_PATH */
#ifdef NGX_ERROR_LOG_PATH
    NGX_HTTP_INFO_KEY_ERROR_LOG_PATH,
#endif /* NGX_ERROR_LOG_PATH */
#ifdef NGX_PID_PATH
    NGX_HTTP_INFO_KEY_PID_PATH,
#endif /* NGX_PID_PATH */
#ifdef NGX_LOCK_PATH
    NGX_HTTP_INFO_KEY_LOCK_PATH,
#endif /* NGX_LOCK_PATH */
#ifdef NGX_HTTP_LOG_PATH
    NGX_HTTP_INFO_KEY_HTTP_LOG_PATH,
#endif /* NGX_HTTP_LOG_PATH */
#ifdef NGX_HTTP_CLIENT_TEMP_PATH
    NGX_HTTP_INFO_KEY_HTTP_CLIENT_TEMP_PATH,
#endif /* NGX_HTTP_CLIENT_TEMP_PATH */
#ifdef NGX_HTTP_PROXY_TEMP_PATH
    NGX_HTTP_INFO_KEY_HTTP_PROXY_TEMP_PATH,
#endif /* NGX_HTTP_PROXY_TEMP_PATH */
#ifdef NGX_HTTP_FASTCGI_TEMP_PATH
    NGX_HTTP_INFO_KEY_HTTP_FASTCGI_TEMP_PATH,
#endif /* NGX_HTTP_FASTCGI_TEMP_PATH */
#ifdef NGX_HTTP_UWSGI_TEMP_PATH
    NGX_HTTP_INFO_KEY_HTTP_UWSGI_TEMP_PATH,
#endif /* NGX_HTTP_UWSGI_TEMP_PATH */
#ifdef NGX_HTTP_SCGI_TEMP_PATH
    NGX_HTTP_INFO_KEY_HTTP_SCGI_TEMP_PATH,
#endif /* NGX_HTTP_SCGI_TEMP_PATH */
#ifdef NGX_COMPILER
    NGX_HTTP_INFO_KEY_BUILT_COMPILER,
#endif /* NGX_COMPILER */
    NGX_HTTP_INFO_KEY_CONFIGURE_ARGUMENTS,
    NGX_HTTP_INFO_KEY_MAX
} ngx_http_info_key;

ngx_str_t ngx_http_info_keys[NGX_HTTP_INFO_KEY_MAX] = {
    ngx_string("nginx-version"),
    ngx_string("prefix-path"),
    ngx_string("conf-prefix-path"),
#ifdef NGX_SBIN_PATH
    ngx_string("sbin-path"),
#endif /* NGX_SBIN_PATH */
#ifdef NGX_CONF_PATH
    ngx_string("conf-path"),
#endif /* NGX_CONF_PATH */
#ifdef NGX_ERROR_LOG_PATH
    ngx_string("error-log-path"),
#endif /* NGX_ERROR_LOG_PATH */
#ifdef NGX_PID_PATH
    ngx_string("pid-path"),
#endif /* NGX_PID_PATH */
#ifdef NGX_LOCK_PATH
    ngx_string("lock-path"),
#endif /* NGX_LOCK_PATH */
#ifdef NGX_HTTP_LOG_PATH
    ngx_string("http-log-path"),
#endif /* NGX_HTTP_LOG_PATH */
#ifdef NGX_HTTP_CLIENT_TEMP_PATH
    ngx_string("http-client-body-temp-path"),
#endif /* NGX_HTTP_CLIENT_TEMP_PATH */
#ifdef NGX_HTTP_PROXY_TEMP_PATH
    ngx_string("http-proxy-temp-path"),
#endif /* NGX_HTTP_PROXY_TEMP_PATH */
#ifdef NGX_HTTP_FASTCGI_TEMP_PATH
    ngx_string("http-fastcgi-temp-path"),
#endif /* NGX_HTTP_FASTCGI_TEMP_PATH */
#ifdef NGX_HTTP_UWSGI_TEMP_PATH
    ngx_string("http-uwsgi-temp-path"),
#endif /* NGX_HTTP_UWSGI_TEMP_PATH */
#ifdef NGX_HTTP_SCGI_TEMP_PATH
    ngx_string("http-scgi-temp-path"),
#endif /* NGX_HTTP_SCGI_TEMP_PATH */
#ifdef NGX_COMPILER
    ngx_string("built-compiler"),
#endif
    ngx_string("configure-arguments"),
};

static void *ngx_http_info_create_main_conf(ngx_conf_t *cf);
static char *ngx_http_info_init_main_conf(ngx_conf_t *cf, void *conf);
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

    ngx_null_command
};

static ngx_http_module_t ngx_http_info_module_ctx = {
    NULL,                              /* preconfiguration */
    NULL,                              /* postconfiguration */

    ngx_http_info_create_main_conf,    /* create main configuration */
    ngx_http_info_init_main_conf,      /* init main configuration */

    NULL,                              /* create server configuration */
    NULL,                              /* merge server configuration */

    NULL,                              /* create location configuration */
    NULL                               /* merge location configuration */
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

static u_char *ngx_http_info_build_response_item(u_char *rbuf, const ngx_str_t *key, ngx_str_t *val)
{
    rbuf = ngx_cpystrn(rbuf, key->data, key->len + 1);
    rbuf = ngx_cpystrn(rbuf, (u_char *)":", (sizeof(":") - 1) + 1);
    rbuf = ngx_cpystrn(rbuf, val->data, val->len + 1);
    rbuf = ngx_cpystrn(rbuf, (u_char *)"\n", (sizeof("\n") - 1) + 1);
    return rbuf;
}

static void ngx_http_info_build_response(ngx_http_info_main_conf_t *imcf)
{
    ngx_int_t i, c;
    size_t rlen;
    u_char *rbuf;
    ngx_str_t *key, val;
    
    rbuf = imcf->response.data;
    rlen = 0;
    c = sizeof(ngx_http_info_keys) / sizeof(ngx_str_t);
    for (i=0;i<c;i++) {
        key = &ngx_http_info_keys[i];
        switch (i) {
        case NGX_HTTP_INFO_KEY_VERSION:
            val.data = (u_char *)NGINX_VERSION;
            val.len  = sizeof(NGINX_VERSION) - 1;
            rbuf = ngx_http_info_build_response_item(rbuf, key, &val);
            break;
        case NGX_HTTP_INFO_KEY_PREFIX:
            val.data = ngx_cycle->prefix.data;
            val.len  = ngx_cycle->prefix.len;
            rbuf = ngx_http_info_build_response_item(rbuf, key, &val);
            break;
        case NGX_HTTP_INFO_KEY_CONF_PREFIX:
            val.data = ngx_cycle->conf_prefix.data;
            val.len  = ngx_cycle->conf_prefix.len;
            rbuf = ngx_http_info_build_response_item(rbuf, key, &val);
            break;
#ifdef NGX_SBIN_PATH
        case NGX_HTTP_INFO_KEY_SBIN_PATH:
            val.data = (u_char *)NGX_SBIN_PATH;
            val.len  = sizeof(NGX_SBIN_PATH) - 1;
            rbuf = ngx_http_info_build_response_item(rbuf, key, &val);
            break;
#endif /* NGX_SBIN_PATH */
#ifdef NGX_CONF_PATH
        case NGX_HTTP_INFO_KEY_CONF_PATH:
            val.data = (u_char *)NGX_CONF_PATH;
            val.len  = sizeof(NGX_CONF_PATH) - 1;
            rbuf = ngx_http_info_build_response_item(rbuf, key, &val);
            break;
#endif /* NGX_CONF_PATH */
#ifdef NGX_ERROR_LOG_PATH
        case NGX_HTTP_INFO_KEY_ERROR_LOG_PATH:
            val.data = (u_char *)NGX_ERROR_LOG_PATH;
            val.len  = sizeof(NGX_ERROR_LOG_PATH) - 1;
            rbuf = ngx_http_info_build_response_item(rbuf, key, &val);
            break;
#endif /* NGX_ERROR_LOG_PATH */
#ifdef NGX_PID_PATH
        case NGX_HTTP_INFO_KEY_PID_PATH:
            val.data = (u_char *)NGX_PID_PATH;
            val.len  = sizeof(NGX_PID_PATH) - 1;
            rbuf = ngx_http_info_build_response_item(rbuf, key, &val);
            break;
#endif /* NGX_ */
#ifdef NGX_LOCK_PATH
        case NGX_HTTP_INFO_KEY_LOCK_PATH:
            val.data = (u_char *)NGX_LOCK_PATH;
            val.len  = sizeof(NGX_LOCK_PATH) - 1;
            rbuf = ngx_http_info_build_response_item(rbuf, key, &val);
            break;
#endif /* NGX_LOCK_PATH */
#ifdef NGX_HTTP_LOG_PATH
        case NGX_HTTP_INFO_KEY_HTTP_LOG_PATH:
            val.data = (u_char *)NGX_HTTP_LOG_PATH;
            val.len  = sizeof(NGX_HTTP_LOG_PATH) - 1;
            rbuf = ngx_http_info_build_response_item(rbuf, key, &val);
            break;
#endif /* NGX_HTTP_LOG_PATH */
#ifdef NGX_HTTP_CLIENT_TEMP_PATH
        case NGX_HTTP_INFO_KEY_HTTP_CLIENT_TEMP_PATH:
            val.data = (u_char *)NGX_HTTP_CLIENT_TEMP_PATH;
            val.len  = sizeof(NGX_HTTP_CLIENT_TEMP_PATH) - 1;
            rbuf = ngx_http_info_build_response_item(rbuf, key, &val);
            break;
#endif /* NGX_HTTP_CLIENT_TEMP_PATH */
#ifdef NGX_HTTP_PROXY_TEMP_PATH
        case NGX_HTTP_INFO_KEY_HTTP_PROXY_TEMP_PATH:
            val.data = (u_char *)NGX_HTTP_PROXY_TEMP_PATH;
            val.len  = sizeof(NGX_HTTP_PROXY_TEMP_PATH) - 1;
            rbuf = ngx_http_info_build_response_item(rbuf, key, &val);
            break;
#endif /* NGX_HTTP_PROXY_TEMP_PATH */
#ifdef NGX_HTTP_FASTCGI_TEMP_PATH
        case NGX_HTTP_INFO_KEY_HTTP_FASTCGI_TEMP_PATH:
            val.data = (u_char *)NGX_HTTP_FASTCGI_TEMP_PATH;
            val.len  = sizeof(NGX_HTTP_FASTCGI_TEMP_PATH) - 1;
            rbuf = ngx_http_info_build_response_item(rbuf, key, &val);
            break;
#endif /* NGX_HTTP_FASTCGI_TEMP_PATH */
#ifdef NGX_HTTP_UWSGI_TEMP_PATH
        case NGX_HTTP_INFO_KEY_HTTP_UWSGI_TEMP_PATH:
            val.data = (u_char *)NGX_HTTP_UWSGI_TEMP_PATH;
            val.len  = sizeof(NGX_HTTP_UWSGI_TEMP_PATH) - 1;
            rbuf = ngx_http_info_build_response_item(rbuf, key, &val);
            break;
#endif /* NGX_HTTP_UWSGI_TEMP_PATH */
#ifdef NGX_HTTP_SCGI_TEMP_PATH
        case NGX_HTTP_INFO_KEY_HTTP_SCGI_TEMP_PATH:
            val.data = (u_char *)NGX_HTTP_SCGI_TEMP_PATH;
            val.len  = sizeof(NGX_HTTP_SCGI_TEMP_PATH) - 1;
            rbuf = ngx_http_info_build_response_item(rbuf, key, &val);
            break;
#endif /* NGX_HTTP_SCGI_TEMP_PATH */
#ifdef NGX_COMPILER
        case NGX_HTTP_INFO_KEY_BUILT_COMPILER:
            val.data = (u_char *)NGX_COMPILER;
            val.len  = sizeof(NGX_COMPILER) - 1;
            rbuf = ngx_http_info_build_response_item(rbuf, key, &val);
            break;
#endif /* NGX_COMPILER */
        case NGX_HTTP_INFO_KEY_CONFIGURE_ARGUMENTS:
            val.data = (u_char *)NGX_CONFIGURE;
            val.len  = sizeof(NGX_CONFIGURE) - 1;
            rbuf = ngx_http_info_build_response_item(rbuf, key, &val);
            break;
        default:
            /* not through */
            break;
        }

        rlen += key->len
            + 1 /* ":" */
            + val.len
            + 1 /* LF  */
            ;
    }

    imcf->response.len = rlen;
}

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

    imcf = ngx_http_conf_get_module_main_conf(cf, ngx_http_info_module);

    if (imcf->response_buffer_size == NGX_CONF_UNSET_SIZE) {
        imcf->response_buffer_size = 1 * 1024 * 1024;
    }

    imcf->response.data = ngx_palloc(cf->pool, imcf->response_buffer_size);
    if (imcf->response.data == NULL) {
        return NGX_CONF_ERROR;
    }

    ngx_http_info_build_response(imcf);

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
    ngx_int_t    rc;
    ngx_chain_t  out;
    ngx_buf_t   *b;
    ngx_http_info_main_conf_t *imcf;

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
