#include "webserv.hpp"

//handle_client receives and sends a limited nbr of bytes
int main()
{
  int port = 8080;
  int server_fd = create_listening_socket(port);
  if (server_fd == ERROR)
    return FAILURE;
  set_nonblocking(server_fd);
  if (manage_events(server_fd) == ERROR)
    return FAILURE;
  return SUCCESS;
}
