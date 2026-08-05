#include "webserv.hpp"

std::map<int, Server> create_server(Conf conf_c)
{
  std::map<int, Server> servers_fds;
  std::vector<Server> servers = conf_c.get_servers();
  size_t nbr_of_servers = (size_t)servers.size();
  for (size_t server_index = 0; server_index < nbr_of_servers; server_index++)
  {
    Server server = servers[server_index];
    int port = server.get_port_listen();
    int server_fd = create_listening_socket(port);
    if (server_fd == ERROR)
    {
      print_error("Server with port" + to_str(server_fd) + "couldn't start");
      continue;
    }
    set_nonblocking(server_fd);
    servers_fds[server_fd] = server;
  }
  return servers_fds;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    print("Need 2 arguments.");
    return FAILURE;
  }
  Conf *conf_c = init_conf(argv[1]);
  if (conf_c == NULL) {
    return FAILURE;
  }
  std::map<int, Server> servers = create_server(*conf_c);
  int status = manage_events(servers, *conf_c);
  delete conf_c;
  if (status == ERROR)
    return FAILURE;
  return SUCCESS;
}
