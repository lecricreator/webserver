# ifndef WEBSERV
# define WEBSERV

# include "support.hpp"
# include "conf/Conf.hpp"
# include "httpRequest.hpp"

# include <cerrno>
# include <fcntl.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <sys/socket.h>
# include <sys/epoll.h>
# include <arpa/inet.h>
# include <iostream>
# include <fstream>
# include <list>

# define ERROR -1
# define SUCCESS 0
# define FAILURE 1

//socket
int     create_listening_socket(int port);
int     accept_client(int server_fd);
void    handle_client(int client_fd, const char *response);

//socket support
void    print_function_error(std::string function_name);
void    print_success(std::string function_name, std::string output_name, int output);
void    set_nonblocking(int fd);

//epoll
int     manage_events(const char *response, int client_fd);

//conf
void    parse_conf(char *argv);
Conf    *init_conf(char *path_conf);

#endif
