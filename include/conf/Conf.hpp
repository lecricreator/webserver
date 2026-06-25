#ifndef CONF
# define CONF

//# include "webserv.hpp"
# include <string>
# include "conf/Server.hpp"
# include "conf/Events.hpp"
# include "conf/Location.hpp"
# include <vector>

size_t  put_index_after_space(std::string line, size_t index);

class Conf {
    private:
        std::string                 _user;
        int                         _worker_process;
        std::vector<std::string>    _error_log;
        std::vector<std::string>    _error_page;
        std::vector<std::string>    _access_log;
        std::string                 _pid;
        std::string                 _include;
        std::string                 _default_type;
        std::string                 _log_format;
        std::string                 _sendfile;
        std::vector<int>            _keepalive_timeout;
        bool                        _gzip;
        bool                        _http;
        std::vector<Events>         _events;
        std::vector<Server>         _servers;
    public:
        Conf();
        Set_variable            set;
        void                    parse(char *argv);

        //GET
        std::vector<Server>     get_servers()    {return (this->_servers);};
        std::vector<Events>     get_events()    {return (this->_events);};
    };

#endif
