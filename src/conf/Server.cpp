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
            put_index_after_space(line, posi + 5);
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
        std::cout << "Server : " << line << std::endl;
    }
}


void    Server::add_in_var(std::string line, size_t posi, std::string *at_replace) {
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
    print("Error need to add ';'. Next step need to replace by another file\nServer.cpp / add_in_var");
    *at_replace = "NULL";
    return ;
}

void    Server::add_in_var(std::string line, size_t posi, int *at_replace) {
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
