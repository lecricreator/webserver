#ifndef SERVER
# define SERVER
# include <vector>
# include "conf/Location.hpp"

class Server {
    private:
        int                                 _port_listen;
        int                                 _client_max_body_size;

        std::string                         _server_name;
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
		Server(const Server& copy);
		Server	&operator=(const Server& copy);
		~Server();
        bool                                parse_server(std::ifstream *fd_file);
        Set_variable                        set;

        //GET
        int                                 &get_port_listen()   {return (this->_port_listen);};
        const std::string                   &get_server_name() const {return (this->_server_name);};
        const std::vector<Location>         &get_location() const {return (this->_locations);};
        const std::map<int, std::string>    &get_error_page() const {return (this->_error_page);};
        const int                           &get_client_max_body_size() const {return (this->_client_max_body_size);};
    };

#endif
