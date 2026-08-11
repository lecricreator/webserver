#ifndef SERVER
# define SERVER
# include <vector>
# include "conf/Location.hpp"

class Server {
    private:
        int                                 _port_listen;
        std::vector<std::string>            _server_name;
        std::string                         _include;
        std::string                         _default_type;
        std::string                         _log_format;
        std::string                         _sendfile;
        std::string                         _keepalive_timeout;
        std::string                         _gzip;
        std::vector<std::string>            _access_log;
        std::vector<Location>               _locations;
        std::map<int, std::string>          _error_page;
    public:
        Server();
        bool                                parse_server(std::ifstream *fd_file);
        Set_variable                        set;

        //GET
        int                                 &get_port_listen()   {return (this->_port_listen);};
        const std::vector<std::string>      &get_server_name() const {return (this->_server_name);};
        const std::vector<Location>         &get_location() const {return (this->_locations);};
        const std::map<int, std::string>    &get_error_page() const {return (this->_error_page);};
};

#endif
