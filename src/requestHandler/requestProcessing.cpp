#include "webserv.hpp"
#include "httpRequest.hpp"
#include "cgi.hpp"

static bool is_response_data_valid(t_response_data &data)
{
  bool has_status_code = !data.status.empty();

  bool is_200 = !(data.content_type.empty() || data.body.empty());
  bool is_301 = !(data.location.empty());
  bool is_error = data.content_type.empty() && data.body.empty();

  bool is_valid = has_status_code && (is_200 || is_301 || is_error);
  return is_valid;
}

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

static std::string create_response(const t_response_data &data)
{
  std::string status = "HTTP/1.1 ";
  std::string content_type = "Content-Type: ";
  std::string content_lenght = "Content-Length: ";
  std::string location = "Location: ";

  std::string content_lenght_value = to_str(data.body.size());
  std::string end_line = "\r\n";

  std::string response;
  if (!data.status.empty())
    add_line_to_response(response, status, data.status);
  if (!data.content_type.empty())
    add_line_to_response(response, content_type, data.content_type);
  if (!data.location.empty())
    add_line_to_response(response, location, data.location);
  if (!data.body.empty())
  {
    add_line_to_response(response, content_lenght, content_lenght_value);
    add_body_to_response(response, data.body);
  }
  add_line_to_response(response, std::string(), std::string());
	return response;
}

t_response_data httpRequest::generate_response_data(const Server &server, const bool &is_cgi_script)
{
  t_response_data data;
  int             status_code;

  status_code = is_cgi_script ? cgi(_path, data) : getResponse(server, data.content_type);
  data.status = to_str((int)status_code) + " " + code_to_string(status_code);

  if (!is_cgi_script)
  {
    if (data.status == "301 Moved Permanently")
      data.location = _path + "/";
    else
      data.body = _responseBody;
  }
  return data;
}

static void print_response_data(const t_response_data &data)
{
  print("-----------");
  print("response data:");
  print("status:        " + data.status);
  print("content_type:  " + data.content_type);
  print("location:      " + data.location);
  print("body:");
  print(data.body);
  print("-----------");
}

std::string httpRequest::generateResponse(const Server &server)
{
  std::string response;
  std::string script_name;
  t_response_data data;

  bool is_cgi_script = is_cgi(_path, script_name);
  //script name useful for env vars
 
  if (_errorCode == 0)
    _errorCode = 200;

  if (_errorCode == 200)
    data = generate_response_data(server, is_cgi_script);

  bool debug = true;
  if (debug)
    print_response_data(data);

  if (is_response_data_valid(data))
    response = create_response(data);
  else
    print_error("Invalid response type");
  return response;
}
