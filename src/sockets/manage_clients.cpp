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

static std::string create_response(const std::string &status_value,
                            const std::string &content_type_value,
                            const std::string &body
                            )
{
  if (status_value.empty() || content_type_value.empty() || body.empty())
    return std::string();
  std::string status = "HTTP/1.1 ";
  std::string content_type = "Content-Type: ";
  std::string content_lenght = "Content-Length: ";

  std::string content_lenght_value = to_str(body.size());

  std::string end_line = "\r\n";

  std::string response;
  response  = status                + status_value          + end_line
            + content_type          + content_type_value    + end_line
            + content_lenght        + content_lenght_value  + end_line
            + end_line
            + body + end_line;
	return response;
}

static std::string    createResponse301(const std::string &path)
{
  std::string response;

  response = "HTTP/1.1 301 " + code_to_string(301) + "\r\n";
  response += "Location: " + path + "/\r\n\r\n";
  return response;
}

//connection closed by client, error and no data received is answered the same way for now
//recv([...], MSG_PEEK) wouldn't consume the buffer
int handle_request(int client_fd, t_parse_data &client_infos)
{
  char            buf[4096];
  int             bytes_received = recv(client_fd, buf, sizeof(buf) - 1, 0);
  std::string     request_packet(buf);
  t_response_data data;

  if (bytes_received <= 0)
    return ERROR;
  buf[bytes_received] = '\0';
  print("--- HTTP REQUEST ---\n");
  print(buf);

  if (!client_infos.request.parseRequest(request_packet))
    return ERROR;
  data = client_infos.request.executeRequest(*client_infos.server);
  if (data.status == "301 Moved Permanently")
    client_infos.response = createResponse301(client_infos.request.getPath());
  else
    client_infos.response = create_response(data.status, data.content_type, data.body);
  if (client_infos.response.empty())
    return ERROR;
  print(client_infos.response);
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

