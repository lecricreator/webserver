#include "webserv.hpp"
#include "conf/Server.hpp"

Server::Server() {
}


void    Server::parse_server(std::ifstream *fd_file) {
    std::string line;
    size_t      posi;

    while (std::getline(*fd_file, line)) {
        std::cout << "Server : " << line << std::endl;
        if ((posi = line.find("}")) != std::string::npos) {
            return ;
        } else if ((posi = line.find("port ")) != std::string::npos) {
            //print(line);
        } else if ((posi = line.find("location ")) != std::string::npos) {
            if ((posi = line.find("{", posi + 9)) != std::string::npos) {
                Location location = Location();
                location.parse_location(fd_file);
                this->_location.push_back(location);
            }
        } else if ((posi = line.find("server_name ")) != std::string::npos) {
            //print(line);
        } else if ((posi = line.find("include ")) != std::string::npos) {
            //print(line);
        } else if ((posi = line.find("default_type ")) != std::string::npos) {
            //print(line);
        } else if ((posi = line.find("log_format ")) != std::string::npos) {
            //print(line);
        } else if ((posi = line.find("sendfile ")) != std::string::npos) {
            //print(line);
        } else if ((posi = line.find("keepalive_timeout ")) != std::string::npos) {
            //print(line);
        } else if ((posi = line.find("gzip ")) != std::string::npos) {
            //print(line);
        } else if ((posi = line.find("access_log ")) != std::string::npos) {
            //print(line);
        } else if ((posi = line.find("error_page ")) != std::string::npos) {
            //print(line);
        }
    }
}
