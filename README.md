*his project has been created as part of the 42 curriculum by makurek, samokhta, lomorale.*

# Webserv

## Description

Webserv is an HTTP/1.1 web server written in C++98.  
It is designed to handle multiple clients concurrently using non-blocking I/O and an event loop based on `poll()` and/or `kqueue()`.

The server supports static file serving, directory listing, CGI execution, virtual servers, custom error pages, and a configuration file inspired by nginx.  
The goal of the project is to understand how a real web server works at system level: socket creation, HTTP parsing, request routing, response generation, and process management for CGI.

## Features

- HTTP/1.1 request parsing and response generation
- Non-blocking sockets and event-driven architecture
- Multiple virtual servers (`server` blocks)
- Location-based routing with allowed methods (`GET`, `POST`, `DELETE`)
- Static file serving and directory autoindex
- Custom error pages
- CGI execution using `fork`/`execve` and pipes
- Configuration file parser with syntax validation
- Client body size limits

## Instructions

### Prerequisites

- Unix-like operating system: Linux or macOS
- A C++ compiler (`g++` or `clang++`)
- `make`

### Compilation

```sh
make
```

This produces the `webserv` executable.

### Execution

```sh
./webserv [path/to/config.conf]
```

If no configuration file is provided, the server uses a default configuration file, e.g.:

```sh
./webserv config/default.conf
```

Then open a browser and go to:

```text
http://localhost:8080
```

### Cleaning

```sh
make clean
make fclean
make re
```

### Usage examples

```sh
curl -v http://localhost:8080/
curl -v http://localhost:8080/nonexistent
curl -X POST -d "name=42" http://localhost:8080/cgi-bin/echo.py
```

CGI scripts must be executable and located in a directory configured for CGI.

## Configuration

Example configuration file:

```nginx
server {
    listen 8080;
    server_name localhost;
    root ./www;
    index index.html;
    error_page 404 ./errors/404.html;
    client_max_body_size 1M;

    location /cgi-bin/ {
        cgi_pass .py;
    }

    location /uploads/ {
        upload_dir ./www/uploads;
        allowed_except POST;
    }
}
```

## Project structure

```text
.
├── Makefile
├── inc/
│   └── webserv headers
├── src/
│   └── webserv sources
├── config/
│   └── default.conf
├── www/
│   ├── index.html
│   └── ...
└── README.md
```

## Technical choices

- Single-threaded event loop with non-blocking sockets
- `epoll()` on Linux
- One process for the server, CGI executed in separate child processes
- Pipes used for CGI input/output
- C++98 compliant code, no external libraries

## Resources

- https://github.com/Kaydooo/Webserv_42#server-core
- https://github.com/cclaude42/webserv
- https://www.alimnaqvi.com/blog/webserv#http-messages
- https://www.rfc-editor.org/info/rfc3875/

### AI usage

AI tools were used during development for the following tasks:

- Explaining information concerning the required concepts
- Helping to find bugs (all fixes were made manually)
- Making this readme

All AI-generated code and explanations were reviewed, adapted to the project constraints, and tested manually.  
No part of the final codebase was committed without understanding its behavior.