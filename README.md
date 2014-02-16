# ngx_info

`ngx_info` provides HTTP interface for checking nginx's information.

## Quick Start

```nginx
location /info {
    nginx_info;
}

location /info_json {
    nginx_info;
    nginx_info_format json;
}
```
