#include "webserv.hpp"
#include "conf/conf.hpp"

void    add_in_var(std::string line, size_t posi, std::string *add_inside) {
    posi = put_index_after_space(line, posi);
    for (; posi < line.length(); posi++) {
        if (line[posi] == ';') {
            break ;
        } else if (line[posi] >= 'A' && line[posi] <= 'z') {
            *add_inside += line[posi];
        }
    }
}

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
    this->_events = NULL;
    this->_server = NULL;
}

void    Conf::parse(char *path_file) {
    std::ifstream   fd_file;
    std::string     line;
    size_t          posi;

    fd_file.open(path_file, std::ifstream::in);
    while (std::getline(fd_file, line)) {
        if ((posi = line.find("server ")) != std::string::npos) {
            print(line);
        } else if ((posi = line.find("events ")) != std::string::npos) {
            print(line);
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
            //add_in_var(line, posi + 17, &this->_keepalive_timeout);
        } else if ((posi = line.find("gzip ")) != std::string::npos) {
            if ((posi = line.find("true")) != std::string::npos) {
                this->_gzip = true;
            }
        }
    }
    fd_file.close();
}
