
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
        case NGX_HTTP_INFO_KEY_PCRE_ENABLED:
#ifdef NGX_PCRE
            val.data = (u_char *)"yes";
            val.len  = sizeof("yes") - 1;
#else
            val.data = (u_char *)"no";
            val.len  = (u_char *)"no";
#endif /* NGX_PCRE */
            rbuf = ngx_http_info_build_response_item(rbuf, key, &val);
            break;
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

    response->len = rlen;
}
