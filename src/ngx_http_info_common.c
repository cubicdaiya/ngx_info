
#include "ngx_http_info_common.h"

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
    ngx_string("pcre-enabled"),
    ngx_string("pcre-jit-enabled"),
    ngx_string("ssl-enabled"),
    ngx_string("zlib-enabled"),
    ngx_string("http-cache-enabled"),
    ngx_string("http-stub-status-enabled"),
    ngx_string("http-dav-enabled"),
    ngx_string("debug-logging-enabled"),
    ngx_string("ipv6-enabled"),
    ngx_string("spdy-enabled"),
#if NGX_HTTP_SPDY
    ngx_string("spdy-version"),
#endif /* NGX_HTTP_SPDY */
    ngx_string("nginx-user"),
    ngx_string("nginx-group"),
#ifdef NGX_COMPILER
    ngx_string("built-compiler"),
#endif /* NGX_COMPILER */
    ngx_string("configure-arguments")
};
