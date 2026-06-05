#include "webserv.hpp"

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

static std::string get_sender_ip(int client_fd)
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
    print("--- HTTP REQUEST ---\n");
    print(buf);
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
