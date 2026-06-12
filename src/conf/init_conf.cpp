#include "webserv.hpp"

void    init_value_parse_conf(t_parse_conf *parse_conf) {
    parse_conf->start_bracket = 0;
    parse_conf->end_bracket = 0;
    parse_conf->nbr_events = 0;
    parse_conf->nbr_http = 0;
    parse_conf->server = NULL;
}

void    init_value_data_conf(t_data_web *data_web) {
    data_web->conf.user = "a";
    data_web->conf.worker_process = -1;
    data_web->conf.error_log = "";
    data_web->conf.error_page = "";
    data_web->conf.access_log = "";
    data_web->conf.pid = "";
    data_web->conf.include = "";
    data_web->conf.default_type = "";
    data_web->conf.log_format = "";
    data_web->conf.sendfile = "";
    data_web->conf.keepalive_timeout = -1;
    data_web->conf.gzip = false;
    data_web->conf.events = NULL;
    data_web->conf.server = NULL;
}

void    init_conf(t_parse_conf *parse_conf) {
    init_value_parse_conf(parse_conf);
    //init_value_data_conf(data_web);
}
