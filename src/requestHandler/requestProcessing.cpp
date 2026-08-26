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

//catches malformed errors
//static bool is_response_data_valid(t_response_data &response_data)
//{
  //bool status = !response_data.status.empty();
  //bool content_type = !response_data.content_type.empty();
  //bool location = !response_data.location.empty();
  //bool body = !response_data.body.empty();
//
  //bool has_body = content_type && body;
  //bool has_no_body = !content_type && !body;
//
  //bool is_200 = response_data.status == "200 ok" && !location && (has_body || has_no_body);
  //bool is_301 = response_data.status == "301 moved permanently" && location;
//
  //bool is_valid = status && (is_200 || (!content_type && !body && (is_301 || (!is_301 && !location))));
  //return is_valid;
//}

t_response_data set_response_data(
                const std::string &status,
                const std::string &location,
                const std::string &content_type,
                const std::string &body
                )
{
  t_response_data result;
  result.status = status;
  result.location = location;
  result.content_type = content_type;
  result.body = body;
  return result;
}

t_response_data set_error_response(const Server &server, int &status_code, const std::string &path)
{
  std::string status;
  std::string location;
  std::string content_type;
  std::string body;

  if (status_code == 301)
    location = path + "/";
  if (server.get_error_page().find(status_code) != server.get_error_page().end()) {
    std::ifstream file;
    std::string error_page_path = "www/" + server.get_error_page().find(status_code)->second;
    file.open(error_page_path.c_str());
    if (access(error_page_path.c_str(), F_OK) == -1) {
      status_code = 404;
    } else if (access(error_page_path.c_str(), R_OK) == -1 || !file.is_open()) {
      status_code = 500;
    }
  }
  status = to_str((int)status_code) + " " + code_to_string(status_code);
  content_type = "text/html";
  body = "<body style=\"background-color: green;\"><h1 style=\"position: absolute; left: 20%; top: 30%; text-align: center;color:red; transform: rotate(150deg);\">" + status + "</h1></body>\n";
  return set_response_data(status, location, content_type, body);
}

//POST method is implemented/called in the parsing therefore not needed here
t_response_data httpRequest::generateResponseData(const Server &server, t_parse_data &parse_data, t_response_data response_data)
{
  int             status_code = _errorCode;
  std::string     script_name;

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
  //when cgi is triggered only cgi_fd must be returned as we must wait for cgi response before responding
  //we try to give fd to epoll loop to offer it to the holy epoll
  
  print("SSSSSSSSSSSTATUS: " + to_str(status_code));
  if (status_code == 200 && parse_data.cgi_fd != ERROR)
    return response_data;

  if (status_code != 200)
    return set_error_response(server, status_code, _path);
  response_data.status = to_str((int)status_code) + " " + code_to_string(status_code);
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
