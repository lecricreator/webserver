#ifndef CONF
# define CONF

# include "webserv.hpp"
# include "conf/Server.hpp"
# include "conf/Events.hpp"
# include <vector>

class Conf {
    private:
        std::string             _user;
        int                     _worker_process;
        std::string             _error_log;
        std::string             _error_page;
        std::string             _access_log;
        std::string             _pid;
        std::string             _include;
        std::string             _default_type;
        std::string             _log_format;
        std::string             _sendfile;
        int                     _keepalive_timeout;
        bool                    _gzip;
        bool                    _http;
        std::vector<Events>     _events;
        std::vector<Server>     _server;
        int                     _nb_server;
    public:
        Conf();
        void                    parse(char *path_file);
};

#endif
