#include "httpRequest.hpp"

//#define REQUEST "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n"

int main()
{
    std::string getTest = "GET / HTTP/1.1\r\nHost: localhost\r\nTest: hello\r\n\r\n";
    std::string postTest = "POST / HTTP/1.1\r\nHost: localhost\r\nTransfer-Encoding: chunked\r\nTest: hello\r\n\r\n2\r\nbo\r\n5\r\nnjour\r\n\r\n";
    std::string test1 = "POST / HTTP/1.1\r\nHos";
    std::string test2 = "t: localhost\r\nTransfer-Encoding: chunked";
    std::string test3 = "\r\nTest: hello\r\n\r\n2\r\nbo\r\n5\r\nnjour\r\n\r\n";
    httpRequest request;
    //request.parseRequest(test1);
    //request.parseRequest(test2);
    //request.parseRequest(test3);
    request.parseRequest(postTest);
    request.printRequest();
    return 0;
}