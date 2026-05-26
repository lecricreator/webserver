#include "webserv.hpp"

#include <cerrno>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void  print_error(std::string function_name)
{
  std::cerr << function_name + "() failed: " << strerror(errno) << std::endl;
}

void  print_success(std::string function_name, std::string output_name, int output)
{
  std::cerr << function_name + "() ok - " + output_name + " = " << output << std::endl;
}

int create_server_socket()
{
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (server_fd == ERROR)
    return print_error("socket"), ERROR;
  int opt = 1;
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  
  print_success("socket", "fd", server_fd);
  return server_fd;
}

int bind_address(int server_fd, int port)
{
    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(addr));
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) == ERROR)
      return print_error("bind"), ERROR;
    print_success("bind", "port", port);
    return SUCCESS;
}

int listen_address(int server_fd, int connexion_queue_size)
{
  if (listen(server_fd, connexion_queue_size) == ERROR)
    return print_error("listen"), close(server_fd);
  print_success("listen", "queue", connexion_queue_size);
  print("Server listening on http://localhost:8080");
  return SUCCESS;
}

int accept_client(int server_fd)
{
  struct sockaddr_in client_addr;
  socklen_t          client_len = sizeof(client_addr);

  int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
  if (client_fd == ERROR)
    return print_error("accept"), ERROR;
  print_success("New client", "fd", client_fd);
  print_success("New client", "ip", (int)client_addr.sin_addr.s_addr);
  return client_fd;
}

//limited buf prevents DoS attacks
void handle_client(int client_fd)
{
    char buf[4096];
    int bytes_received = recv(client_fd, buf, sizeof(buf) - 1, 0);
    if (bytes_received <= 0)
      return (close(client_fd), void());
    buf[bytes_received] = '\0';
    std::cout << "--- REQUETE HTTP ---\n" << buf << std::endl;
    const char* response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 21\r\n"
        "Connection: close\r\n"
        "\r\n"
        "<h1>Hello World!</h1>";

    send(client_fd, response, strlen(response), 0);
    close(client_fd);
}

int main()
{
  int server_fd;
  int client_fd;
  int port = 8080;
  int connexion_queue_size = 10;
  if ((server_fd = create_server_socket()) == ERROR)
    return FAILURE;
  if (bind_address(server_fd, port) == ERROR)
    return FAILURE;
  if (listen_address(server_fd, connexion_queue_size) == ERROR)
    return FAILURE;
  if ((client_fd = accept_client(server_fd)) == ERROR)
    return FAILURE;
  return SUCCESS;
}
