#include "webserv.hpp"

void    parse_conf(char *argv) {
    std::ifstream   fd_file;
    std::string     line;

    fd_file.open(argv, std::ifstream::in);
    while (std::getline(fd_file, line)) {
        print(line);
    }
    (void)argv;
}