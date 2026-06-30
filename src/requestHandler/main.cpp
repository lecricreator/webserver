#include "httpRequest.hpp"

//#define REQUEST "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n"

int main()
{
    std::string test = "GET / HTTP/1.1\r\nHost: localhost\r\nTest: bruhhost\r\naaaaa: bbbbb\r\n\r\n";
    httpRequest request;
    request.parseRequest(test);
    request.printRequest();
    return 0;
}