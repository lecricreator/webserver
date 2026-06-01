#include "support.hpp"

#include <cerrno>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define ERROR -1
#define SUCCESS 0
#define FAILURE 1

//socket
int create_listening_socket(int port);
int accept_client(int server_fd);
void handle_client(int client_fd);

//socket support
void  print_function_error(std::string function_name);
void  print_success(std::string function_name, std::string output_name, int output);
