# ifndef WEBSERV
# define WEBSERV

# include "support.hpp"
# include "conf/Conf.hpp"

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
# include <algorithm>
# include <vector>
# include <map>

# define ERROR -1
# define SUCCESS 0
# define FAILURE 1
# define DONE 0
# define UNFINISHED 1

//socket
int   create_listening_socket(int port);
int   accept_client(int server_fd);
int   get_request(int client_fd, std::string &response, Conf conf_c);
int   send_response(int client_fd, std::string &response);

//socket support
void  print_function_error(std::string function_name);
void  print_success(std::string function_name, std::string output_name, int output);
void  set_nonblocking(int fd);

//epoll
int   manage_events(std::vector<int> server_fds, Conf conf_c);

//conf
void  parse_conf(char *argv);

#endif
