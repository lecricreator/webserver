#include "webserv.hpp"

//line.find("}")) != std::string::npos

std::string choice_content_type(std::string path) {
    std::string content_type;
    if ((path.find(".png") != std::string::npos)) {
        content_type = "image/png";
    } else if ((path.find(".css") != std::string::npos)) {
        content_type = "text/css";
    } else if ((path.find(".html") != std::string::npos)) {
        content_type = "text/html";
    } else if ((path.find(".jpg") != std::string::npos)) {
        content_type = "image/jpg";
    }
    return (content_type);
}

bool    httpRequest::getResponse(const Conf &conf_c, std::string &response, Server &server, std::string &content_type)
{
    std::string     line;
    std::string     exact_path;
    std::vector<Location>::const_iterator it_location;
    (void)conf_c; (void)response;
    for (it_location = server.get_location().begin(); it_location != server.get_location().end(); it_location++) {
        if (it_location->get_path_location() == this->_path && this->_path[this->_path.length() - 1] != '/') {
            setErrorCode(301);
            return false;
        } else if (it_location->get_path_location() + "/" == this->_path) {
            if (this->_path[0] == '/' && this->_path.length() == 1) {
                exact_path = it_location->get_root() + "/" + it_location->get_path_location() + it_location->get_index()[0];
                content_type = "text/html";
                break ;
            } else {
                exact_path = it_location->get_root() + it_location->get_path_location() + "/" + it_location->get_index()[0];
                content_type = "text/html";
                break ;
            }
            exact_path = it_location->get_root() + it_location->get_path_location() + it_location->get_index()[0];
            content_type = "text/html";
        } else {
            content_type = choice_content_type(this->_path);
            exact_path = it_location->get_root() + this->_path;
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
