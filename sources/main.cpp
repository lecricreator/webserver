#include "webserv.hpp"

#include <cerrno>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

void  print_error(std::string function_name)
{
  std::cerr << function_name + "() failed: " << strerror(errno) << std::endl;
}

void  print_success(std::string function_name, int output)
{
  std::cerr << function_name + "() ok - output = " << output << std::endl;
}

int create_server_socket()
{
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (server_fd == ERROR)
  {
    print_error("socket");
    return ERROR;
  }
  int opt = 1;
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  
  print_success("socket", server_fd);
  return server_fd;
}

int main()
{
  print("Hello World!");
  int server_fd = create_server_socket();
  return SUCCESS;
}
