#ifndef WEBSERV
# define WEBSERV

# include "support.hpp"
# include "conf/Conf.hpp"

# include <cerrno>
# include <fcntl.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <iostream>
# include <fstream>
# include <list>

#define ERROR -1
#define SUCCESS 0
#define FAILURE 1

typedef struct  s_data_web {
    t_data_conf     conf;
}t_data_web;

//socket
int create_listening_socket(int port);
int accept_client(int server_fd);
void handle_client(int client_fd);

//socket support
void  print_function_error(std::string function_name);
void  print_success(std::string function_name, std::string output_name, int output);

// init
void    init_conf(t_parse_conf *parse_conf);

//conf
void    parse_conf(char *argv);
size_t  put_index_after_space(std::string line, size_t index);

#endif
