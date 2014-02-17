
#include "ngx_http_info_common.h"

extern ngx_str_t ngx_http_info_keys[NGX_HTTP_INFO_KEY_MAX];

static u_char *ngx_http_info_build_response_text_item(u_char *rbuf, const ngx_str_t *key, ngx_str_t *val);
static u_char *ngx_http_info_build_response_json_item(u_char *rbuf, const ngx_str_t *key, ngx_str_t *val);
static u_char *ngx_http_info_build_response_json_start(u_char *rbuf);
static u_char *ngx_http_info_build_response_json_end(u_char *rbuf);
static size_t ngx_http_info_text_item_length(ngx_str_t *key, ngx_str_t *val);
static size_t ngx_http_info_json_item_length(ngx_str_t *key, ngx_str_t *val);

static u_char *ngx_http_info_build_response_text_item(u_char *rbuf, const ngx_str_t *key, ngx_str_t *val)
{
    rbuf = ngx_cpystrn(rbuf, key->data, key->len + 1);
    rbuf = ngx_cpystrn(rbuf, (u_char *)":", (sizeof(":") - 1) + 1);
    rbuf = ngx_cpystrn(rbuf, val->data, val->len + 1);
    rbuf = ngx_cpystrn(rbuf, (u_char *)"\n", (sizeof("\n") - 1) + 1);
    return rbuf;
}

static u_char *ngx_http_info_build_response_json_item(u_char *rbuf, const ngx_str_t *key, ngx_str_t *val)
{
    rbuf = ngx_cpystrn(rbuf, (u_char *)"\"", (sizeof("\"") - 1) + 1);
    rbuf = ngx_cpystrn(rbuf, key->data, key->len + 1);
    rbuf = ngx_cpystrn(rbuf, (u_char *)"\"", (sizeof("\"") - 1) + 1);
    rbuf = ngx_cpystrn(rbuf, (u_char *)":",  (sizeof(":")  - 1) + 1);
    rbuf = ngx_cpystrn(rbuf, (u_char *)"\"", (sizeof("\"") - 1) + 1);
    rbuf = ngx_cpystrn(rbuf, val->data, val->len + 1);
    rbuf = ngx_cpystrn(rbuf, (u_char *)"\",\n", (sizeof("\",\n") - 1) + 1);
    return rbuf;
}

static u_char *ngx_http_info_build_response_json_start(u_char *rbuf)
{
    rbuf = ngx_cpystrn(rbuf, (u_char *)"{\n", (sizeof("{\n") - 1) + 1);
    return rbuf;
}

static u_char *ngx_http_info_build_response_json_end(u_char *rbuf)
{
    rbuf = ngx_cpystrn(rbuf, (u_char *)"}\n", (sizeof("}\n") - 1) + 1);
    return rbuf;
}

static size_t ngx_http_info_text_item_length(ngx_str_t *key, ngx_str_t *val)
{
    return 
          key->len
        + 1 /* ":" */
        + val->len
        + 1 /* LF  */
        ;
}

static size_t ngx_http_info_json_item_length(ngx_str_t *key, ngx_str_t *val)
{
    return 
          1 /* "\"" */
        + key->len
        + 1 /* "\"" */
        + 1 /* ":"  */
        + 1 /* "\"" */
        + val->len
        + 1 /* "\"" */
        + 1 /* ","  */
        + 1 /* LF   */
        ;
}

ngx_int_t ngx_http_info_build_response(ngx_str_t *response, ngx_http_info_response_format_t response_format, size_t response_buffer_size)
{
    ngx_int_t  i, c;
    size_t     rlen, ilen;
    u_char    *rbuf;
    ngx_str_t *key, val;
    u_char    *(*item_build_func)(u_char *rbuf, const ngx_str_t *key, ngx_str_t *val);
    size_t     (*item_length_func)(ngx_str_t *key, ngx_str_t *val);

    rbuf = response->data;
    rlen = 0;

    switch (response_format) {
    case NGX_HTTP_INFO_RESPONSE_FORMAT_JSON:
        item_build_func  = ngx_http_info_build_response_json_item;
        item_length_func = ngx_http_info_json_item_length;
        rbuf = ngx_http_info_build_response_json_start(rbuf);
        rlen += sizeof("{\n") - 1;
        break;
    case NGX_HTTP_INFO_RESPONSE_FORMAT_TEXT:
    default:
        item_build_func  = ngx_http_info_build_response_text_item;
        item_length_func = ngx_http_info_text_item_length;
        break;
    }

    c = sizeof(ngx_http_info_keys) / sizeof(ngx_str_t);
    for (i=0;i<c;i++) {
        key = &ngx_http_info_keys[i];
        switch (i) {
        case NGX_HTTP_INFO_KEY_VERSION:
            ngx_str_set(&val, NGINX_VERSION);
            break;
        case NGX_HTTP_INFO_KEY_PREFIX:
            val.data = ngx_cycle->prefix.data;
            val.len  = ngx_cycle->prefix.len;
            break;
        case NGX_HTTP_INFO_KEY_CONF_PREFIX:
            val.data = ngx_cycle->conf_prefix.data;
            val.len  = ngx_cycle->conf_prefix.len;
            break;
#ifdef NGX_SBIN_PATH
        case NGX_HTTP_INFO_KEY_SBIN_PATH:
            ngx_str_set(&val, NGX_SBIN_PATH);
            break;
#endif /* NGX_SBIN_PATH */
#ifdef NGX_CONF_PATH
        case NGX_HTTP_INFO_KEY_CONF_PATH:
            ngx_str_set(&val, NGX_CONF_PATH);
            break;
#endif /* NGX_CONF_PATH */
#ifdef NGX_ERROR_LOG_PATH
        case NGX_HTTP_INFO_KEY_ERROR_LOG_PATH:
            ngx_str_set(&val, NGX_ERROR_LOG_PATH);
            break;
#endif /* NGX_ERROR_LOG_PATH */
#ifdef NGX_PID_PATH
        case NGX_HTTP_INFO_KEY_PID_PATH:
            ngx_str_set(&val, NGX_PID_PATH);
            break;
#endif /* NGX_ */
#ifdef NGX_LOCK_PATH
        case NGX_HTTP_INFO_KEY_LOCK_PATH:
            ngx_str_set(&val, NGX_LOCK_PATH);
            break;
#endif /* NGX_LOCK_PATH */
#ifdef NGX_HTTP_LOG_PATH
        case NGX_HTTP_INFO_KEY_HTTP_LOG_PATH:
            ngx_str_set(&val, NGX_HTTP_LOG_PATH);
            break;
#endif /* NGX_HTTP_LOG_PATH */
#ifdef NGX_HTTP_CLIENT_TEMP_PATH
        case NGX_HTTP_INFO_KEY_HTTP_CLIENT_TEMP_PATH:
            ngx_str_set(&val, NGX_HTTP_CLIENT_TEMP_PATH);
            break;
#endif /* NGX_HTTP_CLIENT_TEMP_PATH */
#ifdef NGX_HTTP_PROXY_TEMP_PATH
        case NGX_HTTP_INFO_KEY_HTTP_PROXY_TEMP_PATH:
            ngx_str_set(&val, NGX_HTTP_PROXY_TEMP_PATH);
            break;
#endif /* NGX_HTTP_PROXY_TEMP_PATH */
#ifdef NGX_HTTP_FASTCGI_TEMP_PATH
        case NGX_HTTP_INFO_KEY_HTTP_FASTCGI_TEMP_PATH:
            ngx_str_set(&val, NGX_HTTP_FASTCGI_TEMP_PATH);
            break;
#endif /* NGX_HTTP_FASTCGI_TEMP_PATH */
#ifdef NGX_HTTP_UWSGI_TEMP_PATH
        case NGX_HTTP_INFO_KEY_HTTP_UWSGI_TEMP_PATH:
            ngx_str_set(&val, NGX_HTTP_UWSGI_TEMP_PATH);
            break;
#endif /* NGX_HTTP_UWSGI_TEMP_PATH */
#ifdef NGX_HTTP_SCGI_TEMP_PATH
        case NGX_HTTP_INFO_KEY_HTTP_SCGI_TEMP_PATH:
            ngx_str_set(&val, NGX_HTTP_SCGI_TEMP_PATH);
            break;
#endif /* NGX_HTTP_SCGI_TEMP_PATH */
        case NGX_HTTP_INFO_KEY_PCRE_ENABLED:
#if NGX_PCRE
            ngx_str_set(&val, "yes");
#else
            ngx_str_set(&val, "no");
#endif /* NGX_PCRE */
            break;
        case NGX_HTTP_INFO_KEY_PCRE_JIT_ENABLED:
#if NGX_HAVE_PCRE_JIT
            ngx_str_set(&val, "yes");
#else
            ngx_str_set(&val, "no");
#endif /* NGX_HAVE_PCRE_JIT */
            break;
        case NGX_HTTP_INFO_KEY_SSL_ENABLED:
#if NGX_SSL
            ngx_str_set(&val, "yes");
#else
            ngx_str_set(&val, "no");
#endif /* NGX_SSL */
            break;
        case NGX_HTTP_INFO_KEY_ZLIB_ENABLED:
#if NGX_ZLIB
            ngx_str_set(&val, "yes");
#else
            ngx_str_set(&val, "no");
#endif /* NGX_ZLIB */
            break;
        case NGX_HTTP_INFO_KEY_SPDY_ENABLED:
#if NGX_HTTP_SPDY
            ngx_str_set(&val, "yes");
#else
            ngx_str_set(&val, "no");
#endif /* NGX_HTTP_SPDY */
            break;
#if NGX_HTTP_SPDY
        case NGX_HTTP_INFO_KEY_SPDY_VERSION:
  #if NGX_SPDY_VERSION == 2
    #ifdef TLSEXT_TYPE_next_proto_neg
            ngx_str_set(&val, NGX_SPDY_NPN_NEGOTIATED);
    #else /* TLSEXT_TYPE_next_proto_neg */
            ngx_str_set(&val, "2");
    #endif /* TLSEXT_TYPE_next_proto_neg */
  #else /* NGX_SPDY_VERSION == 2 */
            ngx_str_set(&val, NGX_SPDY_NPN_NEGOTIATED);
  #endif /* NGX_SPDY_VERSION == 2 */
            break;
#endif /* NGX_HTTP_SPDY */
        case NGX_HTTP_INFO_KEY_HTTP_PROXY_ENABLED:
#if NGX_HTTP_PROXY
            ngx_str_set(&val, "yes");
#else
            ngx_str_set(&val, "no");
#endif /* NGX_HTTP_PROXY */
            break;
        case NGX_HTTP_INFO_KEY_HTTP_CACHE_ENABLED:
#if NGX_HTTP_CACHE
            ngx_str_set(&val, "yes");
#else
            ngx_str_set(&val, "no");
#endif /* NGX_HTTP_CACHE */
            break;
        case NGX_HTTP_INFO_KEY_DEBUG_LOGGING_ENABLED:
#if NGX_DEBUG
            ngx_str_set(&val, "yes");
#else
            ngx_str_set(&val, "no");
#endif /* NGX_HTTP_PROXY */
            break;
#ifdef NGX_USER
        case NGX_HTTP_INFO_KEY_NGX_USER:
            ngx_str_set(&val, NGX_USER);
            break;
#endif /* NGX_USER */
#ifdef NGX_GROUP
        case NGX_HTTP_INFO_KEY_NGX_GROUP:
            ngx_str_set(&val, NGX_GROUP);
            break;
#endif /* NGX_GROUP */
#ifdef NGX_COMPILER
        case NGX_HTTP_INFO_KEY_BUILT_COMPILER:
            ngx_str_set(&val, NGX_COMPILER);
            break;
#endif /* NGX_COMPILER */
        case NGX_HTTP_INFO_KEY_CONFIGURE_ARGUMENTS:
            ngx_str_set(&val, NGX_CONFIGURE);
            break;
        default:
            /* not through */
            break;
        }

        ilen = item_length_func(key, &val);

        if (rlen + ilen > response_buffer_size) {
            return NGX_ERROR;
        }

        rbuf = item_build_func(rbuf, key, &val);
        rlen += ilen;
    }

    if (response_format == NGX_HTTP_INFO_RESPONSE_FORMAT_JSON) {
        if (rlen + 1 > response_buffer_size) {
            return NGX_ERROR;
        }
        rbuf -= 2;
        *rbuf++ = '\n';
        rlen -= 1;
        rbuf = ngx_http_info_build_response_json_end(rbuf);
        rlen += sizeof("}\n") - 1;
    }

    response->len = rlen;

    return NGX_OK;
}
