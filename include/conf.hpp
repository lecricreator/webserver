
struct server {
    int         port;
    std::string host;
    std::string index;
};

struct location {
    std::string root;
};

void    parse_conf(char *argv);