# All NGINX VARIABLE

## events
```
Syntax:	events { ... }
Default:	—
Context:	main
```
Provides the configuration file context in which the directives that affect connection processing are specified.

## http
```
Syntax:	http { ... }
Default:	—
Context:	main
```
Provides the configuration file context in which the HTTP server directives are specified.

## server
```
Syntax:	server { ... }
Default:	—
Context:	http
```
Sets configuration for a virtual server. There is no clear separation between IP-based (based on the IP address) and name-based (based on the “Host” request header field) virtual servers. Instead, the listen directives describe all addresses and ports that should accept connections for the server, and the server_name directive lists all server names. Example configurations are provided in the “How nginx processes a request” document.

## location
```
Syntax:	location [ = | ~ | ~* | ^~ ] uri { ... }
location @name { ... }
Default:	—
Context:	server, location
Sets configuration depending on a request URI.
```
The matching is performed against a normalized URI, after decoding the text encoded in the “%XX” form, resolving references to relative path components “.” and “..”, and possible compression of two or more adjacent slashes into a single slash.

```
location = / {
    [ configuration A ]
}

location / {
    [ configuration B ]
}

location /documents/ {
    [ configuration C ]
}

location ^~ /images/ {
    [ configuration D ]
}

location ~* \.(gif|jpg|jpeg)$ {
    [ configuration E ]
}
```

For webser, i only use:
```BASH
location / {
    [ configuration B ]
}

location /documents/ {
    [ configuration C ]
}

#maybe if obligatory
location ~* \.(gif|jpg|jpeg)$ {
    [ configuration E ]
}
```


## listen
```SH
Syntax:	
listen address[:port] [default_server] [ssl] [http2 | quic] [proxy_protocol] [setfib=number]
    [fastopen=number] [backlog=number] [rcvbuf=size] [sndbuf=size] [accept_filter=filter] [deferred] 
    [bind] [ipv6only=on|off] [reuseport] [multipath] [so_keepalive=on|off|[keepidle]:[keepintvl]:[keepcnt]];
listen port [default_server] [ssl] [http2 | quic] [proxy_protocol] [setfib=number] [fastopen=number] 
    [backlog=number] [rcvbuf=size] [sndbuf=size] [accept_filter=filter] [deferred] [bind] 
    [ipv6only=on|off] [reuseport] [multipath] [so_keepalive=on|off|[keepidle]:[keepintvl]:[keepcnt]];
listen unix:path [default_server] [ssl] [http2 | quic] [proxy_protocol] 
    [backlog=number] [rcvbuf=size] [sndbuf=size] [accept_filter=filter] [deferred] 
    [bind] [so_keepalive=on|off|[keepidle]:[keepintvl]:[keepcnt]];
Default:	
listen *:80 | *:8000;
Context:	server
```
For webser i'll use only this example:
```SH
listen 127.0.0.1:8000;
listen 127.0.0.1;
listen 8000;
listen localhost:8000;
``` 


## server_name
```
Syntax:	server_name name ...;
Default:	
server_name "";
Context:	server
```
Sets names of a virtual server, for example:
```
server {
    server_name example.com www.example.com;
}
```
The first name becomes the primary server name.
Can be more complex, but it's sufisaly for webserver.

## root
```
Syntax:	root path;
Default:	
root html;
Context:	http, server, location, if in location
```
Sets the root directory for requests. For example, with the following configuration
```
location /i/ {
    root /data/w3;
}
```
The /data/w3/i/top.gif file will be sent in response to the “/i/top.gif” request.

The path value can contain variables, except $document_root and $realpath_root.

A path to the file is constructed by merely adding a URI to the value of the root directive. If a URI has to be modified, the alias directive should be used.

## index
```
Syntax:	index file ...;
Default:	
index index.html;
Context:	http, server, location
```
Defines files that will be used as an index. The file name can contain variables. Files are checked in the specified order. The last element of the list can be a file with an absolute path. Example:

```index index.$geo.html index.0.html /index.html;```

It should be noted that using an index file causes an internal redirect, and the request can be processed in a different location. For example, with the following configuration:
```
location = / {
    index index.html;
}

location / {
    ...
}
```
a “/” request will actually be processed in the second location as “/index.html”.


## keepalive_timeout
```
Syntax:	keepalive_timeout timeout [header_timeout];
Default:	
keepalive_timeout 75s;
Context:	http, server, location
```
The first parameter sets a timeout during which a keep-alive client connection will stay open on the server side. The zero value disables keep-alive client connections. The optional second parameter sets a value in the “Keep-Alive: timeout=time” response header field. Two parameters may differ.

The “Keep-Alive: timeout=time” header field is recognized by Mozilla and Konqueror. MSIE closes keep-alive connections by itself in about 60 seconds.

## worker_connections
```
Syntax:	    worker_connections number;
Default:	worker_connections 512;
Context:	events
Sets the maximum number of simultaneous connections that can be opened by a worker process.
```
It should be kept in mind that this number includes all connections (e.g. connections with proxied servers, among others), not only connections with clients. Another consideration is that the actual number of simultaneous connections cannot exceed the current limit on the maximum number of open files, which can be changed by worker_rlimit_nofile.


## include
```
Syntax:	include file | mask;
Default:	—
Context:	any
```
Includes another file, or files matching the specified mask, into configuration. Included files should consist of syntactically correct directives and blocks.

Usage example:
```
include mime.types;
include vhosts/*.conf;
```

## access_log
```
Syntax:	access_log path [format [buffer=size] [gzip[=level]] [flush=time] [if=condition]];
access_log off;
Default:	
access_log logs/access.log combined;
Context:	http, server, location, if in location, limit_except
```
Sets the path, format, and configuration for a buffered log write. Several logs can be specified on the same configuration level. Logging to syslog can be configured by specifying the “syslog:” prefix in the first parameter. The special value off cancels all access_log directives on the current level. If the format is not specified then the predefined “combined” format is used.

## use
```
Syntax:	use method;
Default:	—
Context:	events
```
Specifies the connection processing method to use. There is normally no need to specify it explicitly, because nginx will by default use the most efficient method. (poll, epoll, kqueue & select)

Bonus: can stop the program if use poll and kqueue is use.

## user
```
Syntax:	user user [group];
Default:	
user nobody nobody;
Context:	main
```
Defines user and group credentials used by worker processes. If group is omitted, a group whose name equals that of user is used.

## default_type
```
Syntax:	default_type mime-type;
Default:	
default_type text/plain;
Context:	http, server, location
```
Defines the default MIME type of a response. Mapping of file name extensions to MIME types can be set with the types directive.

The var exist but it's too complicate to use it. Just init the value and make nothing with it.
