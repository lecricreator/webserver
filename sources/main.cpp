#include "webserv.hpp"

#include <cerrno>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void  print_function_error(std::string function_name)
{
  std::cerr << function_name + "() failed: " << strerror(errno) << std::endl;
}

void  print_success(std::string function_name, std::string output_name, int output)
{
  std::cerr << function_name + "() ok - " + output_name + " = " << output << std::endl;
}

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

int accept_client(int server_fd)
{
  struct sockaddr_in client_addr;
  socklen_t          client_len = sizeof(client_addr);

  int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
  if (client_fd == ERROR)
    return print_function_error("accept"), ERROR;
  if (fcntl(client_fd, F_SETFL, fcntl(client_fd, F_GETFD) | O_NONBLOCK) == ERROR)
    return print_function_error("fcntl"), ERROR;
  print_success("New client", "fd", client_fd);
  print_success("New client", "ip", (int)client_addr.sin_addr.s_addr);
  return client_fd;
}

std::string get_sender_ip(int client_fd)
{
  struct sockaddr_in peer;
  socklen_t len = sizeof(peer);
  getpeername(client_fd, (struct sockaddr*)&peer, &len);
  char ip[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &peer.sin_addr, ip, sizeof(ip));
  return ip;
}

//function should receive and send response
//limited buf prevents DoS attacks
//connection closed by client, error and no data received is answered the same way for now
//recv([...], MSG_PEEK) wouldn't consume the buffer
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
    if (send(client_fd, response, strlen(response), 0) == ERROR)
      print_error("Failed to send response to " + get_sender_ip(client_fd));
    close(client_fd);
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

//handle_client receives and sends a limited nbr of bytes
int main() {
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
