#include "webserv.hpp"
#include "conf/Conf.hpp"

Conf::Conf() {
    this->_user = "";
    this->_worker_process = -1;
    this->_error_log = "";
    this->_error_page = "";
    this->_access_log = "";
    this->_pid = "";
    this->_include = "";
    this->_default_type = "";
    this->_log_format = "";
    this->_sendfile = "";
    this->_keepalive_timeout = -1;
    this->_gzip = false;
    this->_http = false;
}

void    Conf::parse(char *path_file) {
    std::ifstream   fd_file;
    std::string     line;
    size_t          posi;

    fd_file.open(path_file, std::ifstream::in);
    while (std::getline(fd_file, line)) {
        if ((posi = line.find("#")) != std::string::npos) {
            continue ;
        } else if ((posi = line.find("server ")) != std::string::npos) {
            if ((posi = line.find("{", posi + 7)) != std::string::npos) {
                Server  server = Server();
                server.parse_server(&fd_file);
                this->_server.push_back(server);
            }
        } else if ((posi = line.find("events ")) != std::string::npos) {
            Events  events = Events();
            events.parse_events(&fd_file);
            this->_events.push_back(events);
        } else if ((posi = line.find("http ")) != std::string::npos) {
            this->_http = true;
        } else if ((posi = line.find("user ")) != std::string::npos) {
            add_in_var(line, posi + 4, &this->_user);
        } else if ((posi = line.find("worker_process ")) != std::string::npos) {
            //add_in_var(line, posi + 14, &this->_worker_process);
        } else if ((posi = line.find("error_log ")) != std::string::npos) {
            add_in_var(line, posi + 9, &this->_error_log);
        } else if ((posi = line.find("error_page ")) != std::string::npos) {
            add_in_var(line, posi + 10, &this->_error_page);
        } else if ((posi = line.find("access_log ")) != std::string::npos) {
            add_in_var(line, posi + 10, &this->_access_log);
        } else if ((posi = line.find("pid")) != std::string::npos) {
            add_in_var(line, posi + 3, &this->_pid);
        } else if ((posi = line.find("include ")) != std::string::npos) {
            add_in_var(line, posi + 7, &this->_include);
        } else if ((posi = line.find("default_type ")) != std::string::npos) {
            add_in_var(line, posi + 12, &this->_default_type);
        } else if ((posi = line.find("log_format ")) != std::string::npos) {
            add_in_var(line, posi + 10, &this->_log_format);
        } else if ((posi = line.find("sendfile ")) != std::string::npos) {
            add_in_var(line, posi + 8, &this->_sendfile);
        } else if ((posi = line.find("keepalive_timeout ")) != std::string::npos) {
            add_in_var(line, posi + 17, &this->_keepalive_timeout);
            std::cout << "keeeeeeeeeeppp is :" << this->_keepalive_timeout;
        } else if ((posi = line.find("gzip ")) != std::string::npos) {
            if ((posi = line.find("true")) != std::string::npos) {
                this->_gzip = true;
            }
        }
        std::cout << "Conf : " << line << std::endl;
    }
    fd_file.close();
}

void    Conf::add_in_var(std::string line, size_t posi, std::string *at_replace) {
    posi = put_index_after_space(line, posi);
    for (; posi < line.length(); posi++) {
        if (line[posi] == ';') {
            return ;
        } else if (line[posi] >= 33 && line[posi] <= 125) {
            *at_replace += line[posi];
        } else {
            *at_replace = "NULL";
            return ;
        }
    }
    print("Error need to add ';'. Next step need to replace by another file\nconf.cpp:13 / add_in_var");
    *at_replace = "NULL";
    return ;
}

void    Conf::add_in_var(std::string line, size_t posi, int *at_replace) {
    std::string tmp_val = "";
    posi = put_index_after_space(line, posi);
    for (; posi < line.length(); posi++) {
        if (line[posi] == ';') {
            *at_replace = to_int(tmp_val);
            return ;
        } else if (line[posi] >= '0' && line[posi] <= '9') {
            tmp_val += line[posi];
        } else {
            *at_replace = -1;
            return ;
        }
    }
    print("Error need to add ';'. Next step need to replace by another file\nconf.cpp:13 / add_in_var");
    *at_replace = -1;
    return ;
}
