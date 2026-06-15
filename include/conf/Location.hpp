#ifndef LOCATION
# define LOCATION
# include <vector>

class Location {
    private:
        std::string _root;
        std::string _index;
        std::string _deny;
        std::string _fastcgi_pass;
        std::string _fastcgi_index;
        std::string _fastcgi_param;
        std::string _include;
        std::string _client_max_body_size;
        std::string _client_body_buffer_size;
        std::string _client_body_temp_path;
        std::string _ssl_certificate;
        std::string _ssl_certificate_key;
        std::string _ssl_session_cache;
        std::string _ssl_session_timeout;
        std::string _ssl_ciphers;
        std::string _ssl_prefer_server_ciphers;
        std::string _proxy_pass;
        std::string _proxy_redirect;
        std::string _proxy_set_header;
        std::string _proxy_connect_timeout;
        std::string _proxy_send_timeout;
        std::string _proxy_read_timeout;
        std::string _proxy_send_lowat;
        std::string _proxy_buffer_size;
        std::string _proxy_buffers;
        std::string _proxy_busy_buffers_size;
        std::string _proxy_temp_file_write_size;
        std::string _proxy_temp_path;
        std::string _charset;
        std::string _expires;
    public:
        Location();
        void    parse_location(std::ifstream *fd_file);
};

#endif
