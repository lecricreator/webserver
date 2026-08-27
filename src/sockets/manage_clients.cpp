#include "webserv.hpp"

int accept_client(int server_fd)
{
  struct sockaddr_in client_addr;
  socklen_t          client_len = sizeof(client_addr);

  int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
  if (client_fd == ERROR)
    return print_function_error("accept"), ERROR;
  if (fcntl(client_fd, F_SETFL, fcntl(client_fd, F_GETFL) | O_NONBLOCK) == ERROR)
    return print_function_error("fcntl"), ERROR;
  print_success("New client", "fd", client_fd);
  print_success("New client", "ip", (int)client_addr.sin_addr.s_addr);
  print("\n");
  return client_fd;
}

//connection closed by client, error and no data received is answered the same way for now
//recv([...], MSG_PEEK) wouldn't consume the buffer
int handle_request(int client_fd, t_parse_data &client_infos,
                    t_response_data response_data)
{
  char            buf[1024];
  int             bytes_received = recv(client_fd, buf, sizeof(buf) - 1, 0);
  std::string     request_packet(buf, bytes_received);
  t_response_data data;

  if (bytes_received <= 0)
    return ERROR;
  buf[bytes_received] = '\0';
  std::cout << "\n--- PACKET START " << client_fd << " ---\n";
  print(buf);
  print("\n--- PACKET END ---\n");
  int status_parsing = client_infos.request.parseRequest(request_packet, client_infos.server->get_client_max_body_size(), *client_infos.server);
  if (client_infos.request.getErrorCode() != 408)
  {
  std::cout << "error code for fd " << client_fd << ": " << client_infos.request.getErrorCode() << "\n";
    if (status_parsing == UNFINISHED)
      return UNFINISHED;
  }
  client_infos.response = client_infos.request.executeRequest(*client_infos.server, client_infos, response_data);
  if (client_infos.cgi_fd != INIT_CGI_FD) {
    client_infos.client_fd = client_fd;
    return UNFINISHED;
  } if (client_infos.response.empty())
    return ERROR;
  return SUCCESS;
}

int send_response(int client_fd, std::string &response)
{
  ssize_t bytes_sent = send(client_fd, response.data(), response.size(), 0);
  if (bytes_sent == ERROR)
    return print_error("Failed to send response"), ERROR;
  if (bytes_sent == (ssize_t)response.size())
    return SUCCESS;
  if (bytes_sent > 0)
    response.erase(0, bytes_sent);
  return UNFINISHED;
}
