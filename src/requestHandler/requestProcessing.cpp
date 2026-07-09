#include "webserv.hpp"

void    httpRequest::appendError()
{

}

bool    httpRequest::addHeaders()
{

}

std::string httpRequest::generateResponse()
{
    std::string res = "HTTP/1.1 ";
    if (_errorCode != 0)
        appendError();
    else
        res.append("200 OK\r\n");
    addHeaders();
    res.append(_responseBody);
    return res;
}

std::string httpRequest::processRequest()
{
    if (_method == "GET")
        getRequest();
    if (_method == "POST")
        postRequest();
    if (_method == "DELETE")
        deleteRequest();

    std::string response = generateResponse();
    return response;
}
