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

bool httpRequest::generateResponse(const Conf &conf_c, std::string &response, Server &server)
{
    std::string status = "HTTP/1.1 ";
    std::string content_type = "Content-Type: text/html\r\n";
    std::string content_lenght_start = "Content-Length: ";
    std::string content_lenght_middle;
    std::string content_lenght_end = "\r\n";
    int size_http = -1;
    //if (this->_path == "")
    (void)conf_c,(void)server;
    if (_errorCode != 0) {
        appendError();
        response = _responseBody;
    }
    else
        status.append("200 OK\r\n");
    //addHeaders();
    if (!this->getResponse(conf_c, response, server)) {
        appendError();
        response = _responseBody;
        return true;
    }
    size_http = this->_responseBody.length();
    content_lenght_middle = to_str(size_http) + "\r\n";
    response = status + content_type + content_lenght_start + content_lenght_middle + content_lenght_end + this->_responseBody + "\r\n";
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
