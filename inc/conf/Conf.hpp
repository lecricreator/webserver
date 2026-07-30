#ifndef CONF
# define CONF

# define CONF_SAFE "conf/safe.conf"
# include <cstddef>
# include <string>
# include <vector>

enum ErrorConf {
    TOO_MUCH_BRACKET,
    NO_END_BRACKET_HTTP,
    NO_END_BRACKET_SERVER,
    NO_END_BRACKET_LOCATION,
    NO_END_BRACKET_EVENTS,
    EMPTY_OR_MISSING,
    FILE_NOT_EXIST,
    HTTP_BEFORE_SERVER,
    NO_SEMICOLON,
    VALUE_NOT_EMPTY,
    VALUE_IS_NOT_CORRECT,
    VALUE_IS_NOT_INT,
};

size_t  put_index_after_space(std::string line, size_t index);
void    print_error_conf(ErrorConf error_numb);

//# include "webserv.hpp"
# include "conf/Server.hpp"
# include "conf/Events.hpp"
# include "conf/Location.hpp"

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
        bool                        _end_http;
    public:
        Conf();
        ~Conf() {};
        Set_variable            set;
        bool                    parse(std::ifstream &fd_file);

        //GET
        const std::vector<Server>&     get_servers() const {return (this->_servers);};
        const std::vector<Events>&     get_events() const {return (this->_events);};
    };

#endif
