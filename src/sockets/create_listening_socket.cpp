#include "webserv.hpp"

//handles only IPv4
int create_server_socket()
{
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (server_fd == ERROR)
    return print_function_error("socket"), ERROR;
  int opt = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == ERROR)
    return print_function_error("setsockopt"), ERROR;
  
  print_success("socket", "fd", server_fd);
  return server_fd;
}

//handles only IPv4
int bind_address(int server_fd, int port)
{
  struct sockaddr_in addr;

  memset(&addr, 0, sizeof(addr));
  addr.sin_family      = AF_INET;
  addr.sin_port        = htons(port);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) == ERROR)
    return print_function_error("bind"), ERROR;
  print_success("bind", "port", port);
  return SUCCESS;
}

int listen_address(int server_fd, int connexion_queue_size)
{
  if (listen(server_fd, connexion_queue_size) == ERROR)
    return print_function_error("listen"), close(server_fd);
  print_success("listen", "queue", connexion_queue_size);
  print("Server listening on http://localhost:8080");
  return SUCCESS;
}

int create_listening_socket(int port)
{
  int server_fd;
  int connexion_queue_size = 10;
  if ((server_fd = create_server_socket()) == ERROR)
    return ERROR;
  if (bind_address(server_fd, port) == ERROR)
    return ERROR;
  if (listen_address(server_fd, connexion_queue_size) == ERROR)
    return ERROR;
  return server_fd;
}
