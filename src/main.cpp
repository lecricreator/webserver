#include "webserv.hpp"

//handle_client receives and sends a limited nbr of bytes
int main(int argc, char **argv) {
  if (argc != 2) {
    print("Need 2 arguments.");
    return FAILURE;
  }
  Conf  conf_c = Conf();
  conf_c.parse(argv[1]);
  int port = 8080;
  int server_fd = create_listening_socket(port);
  if (server_fd == ERROR)
    return FAILURE;
  set_nonblocking(server_fd);
  if (manage_events(server_fd) == ERROR)
    return FAILURE;
  return SUCCESS;
}
