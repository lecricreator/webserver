#ifndef SERVER
# define SERVER
# include <vector>
# include "conf/Location.hpp"

class Server {
    private:
        int                         _listening_port;
        std::vector<std::string>    _server_name;
        std::string                 _include;
        std::string                 _default_type;
        std::string                 _log_format;
        std::string                 _sendfile;
        std::string                 _keepalive_timeout;
        std::string                 _gzip;
        std::vector<std::string>    _access_log;
        std::vector<std::string>    _error_page;
        std::vector<Location>       _location;
    public:
        Server();
        bool                        parse_server(std::ifstream *fd_file);
        Set_variable                set;

        //GET
        int                         get_port_listen()   {return (this->_listening_port);};
        std::vector<std::string>    get_server_name()   {return (this->_server_name);};
        std::vector<Location>       get_location()      {return (this->_location);};
};

#endif
