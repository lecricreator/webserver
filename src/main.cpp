#include "webserv.hpp"

//handle_client receives and sends a limited nbr of bytes
int main(int argc, char **argv) {
  if (argc != 2) {
    print("Need 2 arguments.");
    return FAILURE;
  }
  t_data_web  data_web;
  parse_conf(argv[1], &data_web);
  int port = 8080;
  int server_fd = create_listening_socket(port);
  if (server_fd == ERROR)
    return FAILURE;
  while (true) {
    int client_fd = accept_client(server_fd);
    if (client_fd == ERROR)
      return FAILURE;
    handle_client(client_fd);
  }
  return SUCCESS;
}
