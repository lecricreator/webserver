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

bool    find_part(std::string at_find, std::string line, size_t *i) {
    size_t  tmp_i = *i;
    tmp_i = line.find(at_find, tmp_i);
    if (tmp_i != std::string::npos) {
        *i = tmp_i;
        return true ;
    }
    return false ;
}

void    search_listen(t_data_web *data_web, std::string line, size_t i) {
    size_t          posi_index;
    std::string     str_listen = "listen ";
    std::string     content_parse;
    (void) data_web;
    posi_index = line.find(str_listen, i);
    if (posi_index != std::string::npos) {
        i = put_index_after_space(line, posi_index + str_listen.length());
        for (;i < line.length(); i++){
            if (line[i] >= '0' && line[i] <= '9') {
                content_parse += line[i];
            } else if (line[i] == ';') {
                std::cout << "content parse is " << content_parse << std::endl;
                //data_web->conf.server[0].port = to_int(content_parse);
                break ;
            } else {
                break ;
            }
        }
    }
}

void    create_blocks(t_data_conf *conf, char *argv) {
    std::ifstream   fd_file;
    std::string     line;
    size_t          posi_index;
    (void)conf;

    fd_file.open(argv, std::ifstream::in);
    while (std::getline(fd_file, line)) {
        for (size_t i = put_index_after_space(line, 0); i < line.length(); i++) {
            if (line[i] == '#')
                break ;
            posi_index = i;
            if (find_part("events ", line, &posi_index)){
                if (find_part(" {", line, &posi_index)) {
                    print(line);
                }
            }
            if (find_part("http ", line, &posi_index)){
                if (find_part(" {", line, &posi_index)) {
                    print(line);
                }
            }
            if (find_part("server ", line, &posi_index)){
                if (find_part(" {", line, &posi_index)) {
                    print(line);
                }
            }
            break ;
        }
    }
}

void    parse_conf(char *argv, t_data_web *data_web) {
    std::ifstream   fd_file;
    std::string     line;
    t_parse_conf    parse_conf;

    init_conf(&parse_conf);
    create_blocks(&data_web->conf, argv);
    fd_file.open(argv, std::ifstream::in);
    while (std::getline(fd_file, line)) {
        for (size_t i = put_index_after_space(line, 0); i < line.length(); i++) {
            if (line[i] == '#')
                break ;
            search_listen(data_web, line, i);
            break ;
        }
    }
}
