#include "webserv.hpp"

bool    httpRequest::getRequest()
{
    print("loi");
    if (access(_path.c_str(), F_OK) == -1)
    {
        setErrorCode(404); //404 Not Found
        return false;
    }
    if (access(_path.c_str(), R_OK) == -1)
    {
        setErrorCode(500); //500 Internal Server Error
        return false;
    }

    std::ifstream   file(_path.c_str());
    std::string     line;

    if (!file.is_open())
    {
        setErrorCode(500);
        return false;
    }
    while (std::getline(file, line))
    {
        _responseBody += line;
        if (!file.eof())
            _responseBody += '\n';
    }
    return true;
}
