#include "webserv.hpp"
#include "httpRequest.hpp"

static void add_line_to_response(std::string &response, const std::string header, const std::string value)
{
  std::string end_line = "\r\n";
  response += header + value + end_line;
}

static void add_body_to_response(std::string &response, const std::string body)
{
  std::string end_line = "\r\n";
  response += end_line + body;
}

std::string create_response(const t_response_data &data)
{
  std::string response;
  std::string status          = "HTTP/1.1 ";
  std::string content_type    = "Content-Type: ";
  std::string content_lenght  = "Content-Length: ";
  std::string location        = "Location: ";


  add_line_to_response(response, status, data.status);

  if (!data.content_type.empty())
    add_line_to_response(response, content_type, data.content_type);
  if (!data.location.empty())
    add_line_to_response(response, location, data.location);
  response += "Server: Weebserver\r\n";

  std::string content_lenght_value = to_str(data.body.size());
  add_line_to_response(response, content_lenght, content_lenght_value);

  add_body_to_response(response, data.body);
	return response;
}
