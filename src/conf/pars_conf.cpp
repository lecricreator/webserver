#include "webserv.hpp"

size_t  put_index_after_space(std::string line, size_t index) {
    for (size_t i = index; i < line.length(); i++) {
        if (line[i] != ' '){
            return (i);
        } else {
            continue ;
        }
    }
    return (0);
}

void    parse_conf(char *argv, t_data_web *data_web) {
    std::ifstream   fd_file;
    std::string     line;
    std::string     str_listen = "listen ";
    std::string     content_parse;
    size_t          posi_index;

    fd_file.open(argv, std::ifstream::in);
    while (std::getline(fd_file, line)) {
        
        for (size_t i = put_index_after_space(line, 0); i < line.length(); i++) {
            (void)data_web;
            if (line[i] == '#')
                break ;
            posi_index = line.find(str_listen, i);
            //std::cout << "posi find " << line.find(str_listen, i) << std::endl << "sposi find " << 2147483647 << std::endl;
            if (posi_index != std::string::npos) {
                print("FIND LISTEN");
                i = put_index_after_space(line, posi_index + str_listen.length());
                /*for (i < line.length(); i++;){
                    if (line[i] >= '0' && line[i] <= '9') {
                        content_parse += line[i]; 
                    } else if (line[i] == ';') {
                        std::cout << "content parse is " << content_parse << std::endl;
                        data_web->server.port = to_int(content_parse);
                        break ;
                    } else {
                        break ;
                    }
                }*/
                break ;
            }
        }
    }
}