#include "webserv.hpp"
#include "conf/Server.hpp"

Server::Server() {

}


void    Server::parse_server(std::ifstream *fd_file) {
    std::string line;
    size_t      posi;

    while (std::getline(*fd_file, line)) {
        if ((posi = line.find("#")) != std::string::npos) {
            continue ;
        } else if ((posi = line.find("}")) != std::string::npos) {
            return ;
        } else if ((posi = line.find("port ")) != std::string::npos) {
            this->set.add_in_var(line, posi + 5, &this->_port_listen);
        } else if ((posi = line.find("location ")) != std::string::npos) {
            size_t endposi;
            if ((endposi = line.find("{", posi + 9)) != std::string::npos) {
                Location location = Location();
                location.parse_location(fd_file, line, posi + 9);
                this->_location.push_back(location);
            }
        } else if ((posi = line.find("server_name ")) != std::string::npos) {
            this->set.add_in_var(line, posi + 12, &this->_server_name);
        } else if ((posi = line.find("include ")) != std::string::npos) {
            this->set.add_in_var(line, posi + 8, &this->_include);
        } else if ((posi = line.find("default_type ")) != std::string::npos) {
            this->set.add_in_var(line, posi + 12, &this->_default_type);
        } else if ((posi = line.find("log_format ")) != std::string::npos) {
            this->set.add_in_var(line, posi + 11, &this->_log_format);
        } else if ((posi = line.find("sendfile ")) != std::string::npos) {
            this->set.add_in_var(line, posi + 9, &this->_sendfile);
        } else if ((posi = line.find("keepalive_timeout ")) != std::string::npos) {
            this->set.add_in_var(line, posi + 18, &this->_keepalive_timeout);
        } else if ((posi = line.find("gzip ")) != std::string::npos) {
            this->set.add_in_var(line, posi + 5, &this->_gzip);
        } else if ((posi = line.find("access_log ")) != std::string::npos) {
            this->set.add_in_var(line, posi + 11, &this->_access_log);
        } else if ((posi = line.find("error_page ")) != std::string::npos) {
            this->set.add_in_var(line, posi + 11, &this->_error_page);
        }
    }
}
