#include "webserv.hpp"

void    httpRequest::appendError()
{
  if (this->_errorCode == 301)
  {
    this->_responseBody = "http/1.1 301 " + code_to_string(301) + "\r\n";
    this->_responseBody += "Location: " + this->_path + "/\r\n\r\n";
  }
}

bool httpRequest::generateResponse(std::string &response, Server &server)
{
  if (_errorCode == 0)
    _errorCode = 200;
  std::string status = to_str((int)_errorCode) + code_to_string(_errorCode);
  std::string content_type = "text/html";

  if (!this->getResponse(server, content_type))
  {
    appendError();
    response = _responseBody;
    return true;
  }
  response = create_response(status, content_type, _responseBody);
  if (response.empty())
    return false;
  return true;
}
