#ifndef CONF
# define CONF

# include "webserv.hpp"

typedef struct s_server {
    int         port;
    std::string host;
    std::string index;
}t_server;

typedef struct s_location {
    std::string root;
}t_location;

void    parse_conf(char *argv, t_data_web *data_web);

#endif