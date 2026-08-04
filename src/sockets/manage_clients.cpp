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
  return client_fd;
}

//connection closed by client, error and no data received is answered the same way for now
//recv([...], MSG_PEEK) wouldn't consume the buffer
int get_request(int client_fd, t_parse_data &client_infos)
{
  char buf[4096];
  int bytes_received = recv(client_fd, buf, sizeof(buf) - 1, 0);
  if (bytes_received <= 0)
    return ERROR;
  buf[bytes_received] = '\0';
  print("--- HTTP REQUEST ---\n");
  print(buf);

  std::string chunked_request(buf);
  if (!client_infos.request.parseRequest(chunked_request))
    return ERROR;
  //std::string result = client_infos.request.getResponse();
  client_infos.response = client_infos.request.generateResponse(*client_infos.server);
  if (client_infos.response.empty())
    return ERROR;
  //print(client_infos.response);
  return SUCCESS;
}

int send_response(int client_fd, std::string &response)
{
  ssize_t bytes_sent = send(client_fd, response.data(), response.size(), 0);
  if (bytes_sent == ERROR && errno != EAGAIN)
    return print_error("Failed to send response"), ERROR;
  if (bytes_sent == (ssize_t)response.size())
    return DONE;
  if (bytes_sent > 0)
    response.erase(0, bytes_sent);
  return UNFINISHED;
}
