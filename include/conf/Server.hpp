#ifndef SERVER
# define SERVER
# include "conf/Location.hpp"
# include <vector>

class Server {
    private:
        int                     _port;
        std::string             _server_name;
        std::string             _include;
        std::string             _default_type;
        std::string             _log_format;
        std::string             _sendfile;
        std::string             _keepalive_timeout;
        std::string             _gzip;
        std::string             _access_log;
        std::string             _error_page;
        std::vector<Location>   _location;
        int                     _nb_location;
    public:
        Server();
        void                    parse_server(std::ifstream *fd_file);
        void                    add_in_var(std::string line, size_t posi, std::string *at_replace);
        void                    add_in_var(std::string line, size_t posi, int *at_replace);

};

#endif
