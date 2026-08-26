#include "webserv.hpp"
#include "httpRequest.hpp"
#include "cgi.hpp"

//static void print_response_data(const t_response_data &response_data)
//{
//  print("-----------");
//  print("response response_data:");
//  print("status:        " + response_data.status);
//  print("content_type:  " + response_data.content_type);
//  print("location:      " + response_data.location);
//  print("body:");
//  print(response_data.body);
//  print("-----------");
//}

static bool is_response_data_valid(t_response_data &response_data)
{
  bool status = !response_data.status.empty();
  bool content_type = !response_data.content_type.empty();
  bool location = !response_data.location.empty();
  bool body = !response_data.body.empty();

  bool has_body = content_type && body;
  bool has_no_body = !content_type && !body;

  bool is_200 = response_data.status == "200 OK" && !location && (has_body || has_no_body);
  bool is_301 = response_data.status == "301 Moved Permanently" && location;

  bool is_valid = status && (is_200 || (!content_type && !body && (is_301 || (!is_301 && !location))));
  return is_valid;
}

//POST method is implemented/called in the parsing therefore not needed here
t_response_data httpRequest::generateResponseData(const Server &server, t_parse_data &parse_data, t_response_data response_data)
{
  int             status_code = _errorCode;
  std::string     script_name;

  //check existence
  if (response_data.body.empty())
  {
    if (_method == "GET")
      status_code = getRequest(server, response_data, parse_data);
    else if (_method == "DELETE")
      status_code = deleteRequest();
    else if (_method == "POST" && is_cgi(_path, script_name))
    {
      if (_path[0] != '/')
        _path = "/" + _path;
      char **env = set_cgi_env(script_name);
      status_code = cgi(_path, parse_data, env, _body.c_str());
      free_env(env);
    }
    else
      print("body was filled");
  }
  //when cgi is triggered only cgi_fd must be returned as we must wait for cgi response before responding
  //we try to give fd to epoll loop to offer it to the holy epoll
  if (status_code == 200 && parse_data.cgi_fd != ERROR)
    return print("cgi exited nicely"), response_data;
  print("SSSSSSSSSSSTATUS: " + to_str(status_code));

  response_data.status = to_str((int)status_code) + " " + code_to_string(status_code);
  if (response_data.status == "301 Moved Permanently")
    response_data.location = _path + "/";
  if (!is_response_data_valid(response_data))
  {
    response_data.status = "500 Internal Server Error";
    response_data.content_type.erase(0);
  }
  if (response_data.status != "200 OK") {
    if (server.get_error_page().find(status_code) != server.get_error_page().end()) {
      std::ifstream file;
      std::string path = "www/" + server.get_error_page().find(status_code)->second;
      file.open(path.c_str());
  	  if (access(path.c_str(), F_OK) == -1) {
        response_data.status = "404" + code_to_string(404);
      }
      if (access(path.c_str(), R_OK) == -1 || !file.is_open()) {
        response_data.status = "500" + code_to_string(500);
      }

      std::string copy_file_to_str(std::ifstream &file);
    }
  }
  if (response_data.status != "200 OK" && response_data.body.empty())
  {
    response_data.content_type = "text/html";
    response_data.body = "<body style=\"background-color: green;\"><h1 style=\"position: absolute; left: 20%; top: 30%; text-align: center;color:red; transform: rotate(150deg);\">" + response_data.status + "</h1></body>\n";
  }
  return response_data;
}

std::string httpRequest::executeRequest(const Server &server, t_parse_data &parse_data, t_response_data &response_data)
{
  std::string     response;
  std::string     script_name;

  if (_errorCode == 0)
    setErrorCode(200);

  response_data = generateResponseData(server, parse_data, response_data);
  if (parse_data.cgi_fd != ERROR)
    return std::string();

  //bool debug = false;
  //if (debug)
  //  print_response_data(response_data);

  response = create_response(response_data);
  return response;
}
