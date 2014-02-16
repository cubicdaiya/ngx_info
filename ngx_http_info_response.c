
#include "ngx_http_info_common.h"

extern ngx_str_t ngx_http_info_keys[NGX_HTTP_INFO_KEY_MAX];

static u_char *ngx_http_info_build_response_item(u_char *rbuf, const ngx_str_t *key, ngx_str_t *val);

static u_char *ngx_http_info_build_response_item(u_char *rbuf, const ngx_str_t *key, ngx_str_t *val)
{
    rbuf = ngx_cpystrn(rbuf, key->data, key->len + 1);
    rbuf = ngx_cpystrn(rbuf, (u_char *)":", (sizeof(":") - 1) + 1);
    rbuf = ngx_cpystrn(rbuf, val->data, val->len + 1);
    rbuf = ngx_cpystrn(rbuf, (u_char *)"\n", (sizeof("\n") - 1) + 1);
    return rbuf;
}

void ngx_http_info_build_response(ngx_str_t *response)
{
    ngx_int_t  i, c;
    size_t     rlen;
    u_char    *rbuf;
    ngx_str_t *key, val;
    
    rbuf = response->data;
    rlen = 0;
    c = sizeof(ngx_http_info_keys) / sizeof(ngx_str_t);
    for (i=0;i<c;i++) {
        key = &ngx_http_info_keys[i];
        switch (i) {
        case NGX_HTTP_INFO_KEY_VERSION:
            ngx_str_set(&val, NGINX_VERSION);
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
            ngx_str_set(&val, NGX_SBIN_PATH);
            rbuf = ngx_http_info_build_response_item(rbuf, key, &val);
            break;
#endif /* NGX_SBIN_PATH */
#ifdef NGX_CONF_PATH
        case NGX_HTTP_INFO_KEY_CONF_PATH:
            ngx_str_set(&val, NGX_CONF_PATH);
            rbuf = ngx_http_info_build_response_item(rbuf, key, &val);
            break;
#endif /* NGX_CONF_PATH */
#ifdef NGX_ERROR_LOG_PATH
        case NGX_HTTP_INFO_KEY_ERROR_LOG_PATH:
            ngx_str_set(&val, NGX_ERROR_LOG_PATH);
            rbuf = ngx_http_info_build_response_item(rbuf, key, &val);
            break;
#endif /* NGX_ERROR_LOG_PATH */
#ifdef NGX_PID_PATH
        case NGX_HTTP_INFO_KEY_PID_PATH:
            ngx_str_set(&val, NGX_PID_PATH);
            rbuf = ngx_http_info_build_response_item(rbuf, key, &val);
            break;
#endif /* NGX_ */
#ifdef NGX_LOCK_PATH
        case NGX_HTTP_INFO_KEY_LOCK_PATH:
            ngx_str_set(&val, NGX_LOCK_PATH);
            rbuf = ngx_http_info_build_response_item(rbuf, key, &val);
            break;
#endif /* NGX_LOCK_PATH */
#ifdef NGX_HTTP_LOG_PATH
        case NGX_HTTP_INFO_KEY_HTTP_LOG_PATH:
            ngx_str_set(&val, NGX_HTTP_LOG_PATH);
            rbuf = ngx_http_info_build_response_item(rbuf, key, &val);
            break;
#endif /* NGX_HTTP_LOG_PATH */
#ifdef NGX_HTTP_CLIENT_TEMP_PATH
        case NGX_HTTP_INFO_KEY_HTTP_CLIENT_TEMP_PATH:
            ngx_str_set(&val, NGX_HTTP_CLIENT_TEMP_PATH);
            rbuf = ngx_http_info_build_response_item(rbuf, key, &val);
            break;
#endif /* NGX_HTTP_CLIENT_TEMP_PATH */
#ifdef NGX_HTTP_PROXY_TEMP_PATH
        case NGX_HTTP_INFO_KEY_HTTP_PROXY_TEMP_PATH:
            ngx_str_set(&val, NGX_HTTP_PROXY_TEMP_PATH);
            rbuf = ngx_http_info_build_response_item(rbuf, key, &val);
            break;
#endif /* NGX_HTTP_PROXY_TEMP_PATH */
#ifdef NGX_HTTP_FASTCGI_TEMP_PATH
        case NGX_HTTP_INFO_KEY_HTTP_FASTCGI_TEMP_PATH:
            ngx_str_set(&val, NGX_HTTP_FASTCGI_TEMP_PATH);
            rbuf = ngx_http_info_build_response_item(rbuf, key, &val);
            break;
#endif /* NGX_HTTP_FASTCGI_TEMP_PATH */
#ifdef NGX_HTTP_UWSGI_TEMP_PATH
        case NGX_HTTP_INFO_KEY_HTTP_UWSGI_TEMP_PATH:
            ngx_str_set(&val, NGX_HTTP_UWSGI_TEMP_PATH);
            rbuf = ngx_http_info_build_response_item(rbuf, key, &val);
            break;
#endif /* NGX_HTTP_UWSGI_TEMP_PATH */
#ifdef NGX_HTTP_SCGI_TEMP_PATH
        case NGX_HTTP_INFO_KEY_HTTP_SCGI_TEMP_PATH:
            ngx_str_set(&val, NGX_HTTP_SCGI_TEMP_PATH);
            rbuf = ngx_http_info_build_response_item(rbuf, key, &val);
            break;
#endif /* NGX_HTTP_SCGI_TEMP_PATH */
        case NGX_HTTP_INFO_KEY_PCRE_ENABLED:
#if NGX_PCRE
            ngx_str_set(&val, "yes");
#else
            ngx_str_set(&val, "no");
#endif /* NGX_PCRE */
            rbuf = ngx_http_info_build_response_item(rbuf, key, &val);
            break;
        case NGX_HTTP_INFO_KEY_PCRE_JIT_ENABLED:
#if NGX_HAVE_PCRE_JIT
            ngx_str_set(&val, "yes");
#else
            ngx_str_set(&val, "no");
#endif /* NGX_HAVE_PCRE_JIT */
            rbuf = ngx_http_info_build_response_item(rbuf, key, &val);
            break;
#ifdef NGX_COMPILER
        case NGX_HTTP_INFO_KEY_BUILT_COMPILER:
            ngx_str_set(&val, NGX_COMPILER);
            rbuf = ngx_http_info_build_response_item(rbuf, key, &val);
            break;
#endif /* NGX_COMPILER */
        case NGX_HTTP_INFO_KEY_CONFIGURE_ARGUMENTS:
            ngx_str_set(&val, NGX_CONFIGURE);
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

    response->len = rlen;
}
