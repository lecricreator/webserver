#include "webserv.hpp"

void    httpRequest::appendError()
{

}

bool    httpRequest::addHeaders()
{
    return true;
}

bool httpRequest::generateResponse(const Conf &conf_c, std::string &response, Server &server)
{
    //if (this->_path == "")
    (void)conf_c,(void)server;
    response = "HTTP/1.1 ";
    if (_errorCode != 0)
        appendError();
    else
        response.append("200 OK\r\n");
    addHeaders();
    response.append(_responseBody);
    response.append(
        "Content-Type: text/html\r\n"
        "Content-Length: 10000\r\n");
    if (!this->getResponse(conf_c, response, server)) {
        print("error in generate_response");
    }
    response += _responseBody + "\r\n";
    print("responnnnnnnnnccceee :");
    print(response);
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
