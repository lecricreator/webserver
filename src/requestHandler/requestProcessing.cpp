#include "webserv.hpp"
#include "cgi.hpp"

void    httpRequest::appendError()
{
  if (this->_errorCode == 301)
  {
    this->_responseBody = "HTTP/1.1 301 " + code_to_string(301) + "\r\n";
    this->_responseBody += "Location: " + this->_path + "/\r\n\r\n";
  }
}

bool httpRequest::generateResponse(std::string &response, Server &server)
{
  std::string status;
  std::string content_type;
  std::string body;
  std::string script_name;
  bool is_cgi_script = is_cgi(_path, script_name);
  //script name useful for env vars
  if (_errorCode == 0)
    _errorCode = 200;

  if (is_cgi_script)
    _errorCode = cgi(_path, status, content_type, body);
  if (!this->getResponse(server, content_type, is_cgi_script))
  {
    appendError();
    response = _responseBody;
    return true;
  }
  if (!is_cgi_script)
  {
    status = to_str((int)_errorCode) + " " + code_to_string(_errorCode);
    content_type = "text/html";
    body = _responseBody;
  }
  bool is_debug = true;
  if (is_debug)
  {
    print(status);
    print(content_type);
    print(body);
  }
  response = create_response(status, content_type, body);
  if (response.empty())
    return false;
  return true;
}
