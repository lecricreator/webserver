#include "webserv.hpp"

void    httpRequest::appendError()
{
    if (this->_errorCode == 301) {
        this->_responseBody = "http/1.1 301 " + code_to_string(301) + "\r\n";
        this->_responseBody += "Location: " + this->_path + "/\r\n\r\n";

    }
}

bool    httpRequest::addHeaders()
{
    return (true);
}

bool httpRequest::generateResponse(std::string &response, Server &server)
{
    std::string status = "HTTP/1.1 ";
    std::string content_type = "Content-Type: ";
    std::string content_type_end = "text/html";
    std::string content_lenght_start = "Content-Length: ";
    std::string content_lenght_middle;
    std::string end_line = "\r\n";
    int size_http = -1;
    if (_errorCode != 0) {
        appendError();
        response = _responseBody;
    }
    else
        status.append("200 OK\r\n");
    //addHeaders();
    if (!this->getResponse(server, content_type_end)) {
        appendError();
        response = _responseBody;
        return true;
    }
    size_http = this->_responseBody.length();
    content_lenght_middle = to_str(size_http) + "\r\n";
    response = status + content_type + content_type_end + end_line + content_lenght_start + content_lenght_middle + end_line + this->_responseBody + "\r\n";
    //print("Responce is " + response);
  return (true);
}

/*std::string httpRequest::processRequest()
{
    if (_method == "GET")
        getRequest();
    if (_method == "POST")
        postRequest();
    if (_method == "DELETE")
        deleteRequest();

    std::string response = generateResponse();
    return response;
}*/
