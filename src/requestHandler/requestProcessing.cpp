#include "webserv.hpp"
#include "httpRequest.hpp"
#include "cgi.hpp"

static bool is_response_data_valid(t_response_data &data)
{
  bool status = !data.status.empty();
  bool content_type = !data.content_type.empty();
  bool location = !data.location.empty();
  bool body = !data.body.empty();

  bool is_200 = data.status == "200 OK" && content_type && body && !location;
  bool is_301 = data.status == "301 Moved Permanently" && location;

  bool is_valid = status && (is_200 || (!content_type && !body && (is_301 || (!is_301 && !location))));
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

  std::string content_lenght_value = to_str(data.body.size());
  add_line_to_response(response, content_lenght, content_lenght_value);

  add_body_to_response(response, data.body);
	return response;
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

t_response_data httpRequest::generate_response_data(const Server &server, const bool &is_cgi_script)
{
  t_response_data data;
  int             status_code;

  status_code = is_cgi_script ? cgi(_path, data) : getResponse(server, data);
  data.status = to_str((int)status_code) + " " + code_to_string(status_code);

  if (data.status == "301 Moved Permanently")
    data.location = _path + "/";
  if (!is_response_data_valid(data))
  {
    data.status = "500 Internal Server Error";
    data.content_type.erase(0);
  }
  if (data.status != "200 OK")
    data.body = data.status + "\n";
  print_response_data(data);
  return data;
}

std::string httpRequest::generateResponse(const Server &server)
{
  std::string     response;
  std::string     script_name;
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

  response = create_response(data);
  return response;
}
