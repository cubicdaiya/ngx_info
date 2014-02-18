# ngx_info

`ngx_info` provides HTTP interface for checking nginx's informations.

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

## Directives

### nginx_info

|Syntax |nginx_info|
|-------|--------|
|Default|-|
|Context|location|

Turns on module processing in a surrounding location.

### nginx_info_format

|Syntax |nginx_info_format <*text or json*>|
|-------|--------|
|Default|text|
|Context|location|

Sets a format of nginx_info's response.

### nginx_info_buffer

|Syntax |nginx_info_buffer <*size*>|
|-------|--------|
|Default|1m|
|Context|main|

Sets the maximum size of nginx_info's response. 
If response-size goes over its size, nginx will exit on start-up.

## TODO

 * detecting embedded modules
