#include "webserv.hpp"

//handle_client receives and sends a limited nbr of bytes
int main(int argc, char **argv) {
  if (argc != 2) {
    print("Need 2 arguments.");
    return FAILURE;
  }
  Conf  conf_c = Conf();
  conf_c.parse(argv[1]);
  std::vector<Server> servers = conf_c.get_servers();
  size_t nbr_of_servers = (size_t)servers.size();
  std::vector<int> server_fds;
  for (size_t server_index = 0; server_index < nbr_of_servers; server_index++)
  {
    int port = servers[server_index].get_port_listen();
    int server_fd = create_listening_socket(port);
    if (server_fd == ERROR)
      return FAILURE;
    set_nonblocking(server_fd);
    server_fds.push_back(server_fd);
  }
  if (manage_events(server_fds) == ERROR)
    return FAILURE;
  return SUCCESS;
}
