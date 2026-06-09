#include "webserv.hpp"

const char* response =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Content-Length: 21\r\n"
    "Connection: close\r\n"
    "\r\n"
    "<h1>Hello World!</h1>";

//handle_client receives and sends a limited nbr of bytes
int main()
{
  int port = 8080;
  int server_fd = create_listening_socket(port);
  if (server_fd == ERROR)
    return FAILURE;
  set_nonblocking(server_fd);
  if (manage_events(response, server_fd) == ERROR)
    return FAILURE;
  return SUCCESS;
}
