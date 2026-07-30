#include "webserv.hpp"
#include "cgi.hpp"

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
  else
    _errorCode = cgi(_path, status, content_type, body);
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
