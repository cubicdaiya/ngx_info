#ifndef NGX_HTTP_COMMON_MODULE_H
#define NGX_HTTP_COMMON_MODULE_H

#include <nginx.h>
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

typedef enum ngx_http_info_response_format_t {
    NGX_HTTP_INFO_RESPONSE_FORMAT_TEXT = 0,
    NGX_HTTP_INFO_RESPONSE_FORMAT_JSON,
    NGX_HTTP_INFO_RESPONSE_FORMAT_MAX
} ngx_http_info_response_format_t;


typedef enum ngx_http_info_key_t {
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
    NGX_HTTP_INFO_KEY_PCRE_ENABLED,
    NGX_HTTP_INFO_KEY_PCRE_JIT_ENABLED,
    NGX_HTTP_INFO_KEY_SSL_ENABLED,
    NGX_HTTP_INFO_KEY_ZLIB_ENABLED,
    NGX_HTTP_INFO_KEY_HTTP_CACHE_ENABLED,
    NGX_HTTP_INFO_KEY_HTTP_STAT_STUB_ENABLED,
    NGX_HTTP_INFO_KEY_HTTP_DAV_ENABLED,
    NGX_HTTP_INFO_KEY_DEBUG_LOGGING_ENABLED,
    NGX_HTTP_INFO_KEY_IPV6_ENABLED,
    NGX_HTTP_INFO_KEY_SPDY_ENABLED,
#if NGX_HTTP_SPDY
    NGX_HTTP_INFO_KEY_SPDY_VERSION,
#endif /* NGX_HTTP_SPDY */
    NGX_HTTP_INFO_KEY_NGX_USER,
    NGX_HTTP_INFO_KEY_NGX_GROUP,
#ifdef NGX_COMPILER
    NGX_HTTP_INFO_KEY_BUILT_COMPILER,
#endif /* NGX_COMPILER */
    NGX_HTTP_INFO_KEY_CONFIGURE_ARGUMENTS,
    NGX_HTTP_INFO_KEY_MAX
} ngx_http_info_key_t;


typedef struct ngx_http_info_main_conf_t {
    ngx_str_t response[NGX_HTTP_INFO_RESPONSE_FORMAT_MAX];
    size_t response_buffer_size;
} ngx_http_info_main_conf_t;


typedef struct ngx_http_info_loc_conf_t {
    ngx_uint_t response_format;
} ngx_http_info_loc_conf_t;


#endif /* NGX_HTTP_INFO_COMMON_H */
