#ifndef CONF
# define CONF

# include "webserv.hpp"

typedef struct s_data_conf {
    std::string user;
    int         worker_process;
    std::string error_log;
    std::string error_page;
    std::string access_log;
    std::string pid;
    std::string include;
    std::string default_type;
    std::string log_format;
    std::string sendfile;
    int         keepalive_timeout;
    bool        gzip;
    t_events    *events;
    t_server    *server;
    t_location  *location;
}t_data_conf;

typedef struct s_events {
    std::string worker_connections;
    std::string use;
}t_events;

typedef struct s_server {
    int         port;
    std::string server_name;
    std::string include;
    std::string default_type;
    std::string log_format;
    std::string sendfile;
    std::string keepalive_timeout;
    std::string gzip;
    std::string access_log;
    std::string error_page;

}t_server;

typedef struct s_location {
    std::string root;
    std::string index;
    std::string deny;
    std::string fastcgi_pass;
    std::string fastcgi_index;
    std::string fastcgi_param;
    std::string include;
    std::string client_max_body_size;
    std::string client_body_buffer_size;
    std::string client_body_temp_path;
    std::string ssl_certificate;
    std::string ssl_certificate_key;
    std::string ssl_session_cache;
    std::string ssl_session_timeout;
    std::string ssl_ciphers;
    std::string ssl_prefer_server_ciphers;
    std::string proxy_pass;
    std::string proxy_redirect;
    std::string proxy_set_header;
    std::string proxy_connect_timeout;
    std::string proxy_send_timeout;
    std::string proxy_read_timeout;
    std::string proxy_send_lowat;
    std::string proxy_buffer_size;
    std::string proxy_buffers;
    std::string proxy_busy_buffers_size;
    std::string proxy_temp_file_write_size;
    std::string proxy_temp_path;
    std::string charset;
    std::string expires;
}t_location;

#endif
