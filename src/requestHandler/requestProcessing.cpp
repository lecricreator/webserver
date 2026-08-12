#include "webserv.hpp"
#include "httpRequest.hpp"
#include "cgi.hpp"

static bool is_response_data_valid(t_response_data &data) { bool status = !data.status.empty(); bool content_type = !data.content_type.empty();
  bool location = !data.location.empty();
  bool body = !data.body.empty();

  bool is_200 = data.status == "200 OK" && content_type && !location;
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
  print(response);
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

t_response_data httpRequest::generateResponseData(const Server &server)
{
  t_response_data data;
  int             status_code = _errorCode;
  std::string     script_name;
  bool            is_cgi_script = is_cgi(_path, script_name);

  if (_method == "GET")
  {
    if (is_cgi_script)
    {
      char **env = set_cgi_env(script_name);
      status_code = cgi(_path, data, env);
    }
    else
      status_code = getRequest(server, data);
  }
  //POST method is implemented/called in the parsing therefore not needed here
  if (_method == "DELETE")
  {
    status_code = deleteRequest();
  }

  data.status = to_str((int)status_code) + " " + code_to_string(status_code);
  if (data.status == "301 Moved Permanently")
    data.location = _path + "/";
  if (!is_response_data_valid(data))
  {
    data.status = "500 Internal Server Error";
    data.content_type.erase(0);
  }
  if (data.status != "200 OK") {
    if (server.get_error_page().find(status_code) != server.get_error_page().end()) {
      std::ifstream file;
      std::string path = "www/" + server.get_error_page().find(status_code)->second;
      file.open(path.c_str());
  	  if (access(path.c_str(), F_OK) == -1) {
        data.status = "404" + code_to_string(404);
      }
      if (access(path.c_str(), R_OK) == -1 || !file.is_open()) {
        data.status = "500" + code_to_string(500);
      }

      std::string copy_file_to_str(std::ifstream &file);
    }
  }
  return data;
}

std::string httpRequest::executeRequest(const Server &server)
{
  std::string     response;
  std::string     script_name;
  t_response_data data;

  if (_errorCode == 0)
    setErrorCode(200);

  data = generateResponseData(server);

  bool debug = true;
  if (debug)
    print_response_data(data);

  if (data.status != "200 OK" && data.body.empty())
    data.body = "<body style=\"background-color: green;\"><h1 style=\"position: absolute; left: 20%; top: 30%; text-align: center;color:red; transform: rotate(150deg);\">" + data.status + "</h1></body>\n";
  response = create_response(data);
  return response;
}
