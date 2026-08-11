#ifndef LOCATION
# define LOCATION
# include "Set_variable.hpp"
# include <vector>

class Location {
    private:
        //std::string                 _path_location;
        //std::string                 _root;
        //std::vector<std::string>    _index;
        std::string                 _fastcgi_pass;
        std::string                 _fastcgi_index;
        std::vector<std::string>    _fastcgi_param;
        std::string                 _include;
        std::string                 _client_max_body_size;
        std::string                 _client_body_buffer_size;
        /*std::string                 _ssl_certificate;
        std::string                 _ssl_certificate_key;
        std::string                 _ssl_session_cache;
        std::string                 _ssl_session_timeout;
        std::string                 _ssl_ciphers;
        std::string                 _ssl_prefer_server_ciphers;*/
        std::string                 _proxy_pass;
        std::string                 _proxy_redirect;
        std::string                 _proxy_set_header;
        std::string                 _proxy_connect_timeout;
        std::string                 _proxy_send_timeout;
        std::string                 _proxy_read_timeout;
        std::string                 _proxy_send_lowat;
        std::string                 _proxy_buffer_size;
        std::string                 _proxy_buffers;
        std::string                 _proxy_busy_buffers_size;
        std::string                 _proxy_temp_file_write_size;
        std::string                 _proxy_temp_path;
        std::string                 _charset;
        std::string                 _expires;
        bool                        _is_cgi_py;
        bool                        _autoindex;
    public:
        Location();
        bool                        parse_location(std::ifstream *fd_file, const std::string pline, size_t posi);
        Set_variable                set;
        std::string                 _path_location;
        std::string                 _root;
        std::vector<std::string>    _index;


        //GET
        const std::string&              get_root() const {return (_root);};
        const std::vector<std::string>& get_index() const {return (_index);};
        const std::string&              get_path_location() const {return (_path_location);};

};

#endif
