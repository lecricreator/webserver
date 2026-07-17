#include "webserv.hpp"

bool    httpRequest::getResponse(const Conf &conf_c, std::string &response, Server &server)
{
    std::string     line;
    std::string     exact_path;
    std::vector<Location>::iterator it_location;
    for (it_location = server.get_location().begin(); it_location != server.get_location().end(); it_location++) {
        if () {}
        else if (it_location->get_path_location() == this->_path && this->_path[this->_path.length()] != '/') {
            setErrorCode(301);
            return false;
        } else if (it_location->get_path_location() + "/" == this->_path) {
            exact_path = it_location->get_root() + it_location->get_path_location() + it_location->get_index()[0];
        }
    }
    std::ifstream   file(exact_path.c_str());
    if (access(exact_path.c_str(), F_OK) == -1)
    {
        setErrorCode(404); //404 Not Found
        return false;
    }
    if (access(exact_path.c_str(), R_OK) == -1 || !file.is_open())
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

/*bool    httpRequest::getRequest()
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
*/
