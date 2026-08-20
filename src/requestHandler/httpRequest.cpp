#include "webserv.hpp"

httpRequest::httpRequest() : _headers()
{
	_requestBuffer = "";
	_body = "";
	_method = "";
	_httpVersion = "";
	_status = REQ_START_LINE;
	_errorCode = 0;
	_bodySize = -1;
	_chunkSize = -1;
	_chunkStatus = CHUNK_SIZE;

    _path = "";
    _fileFd = -1;
    _pathValidated = false;
    _isChunkedPost = false;
    _bytesWritten = 0;
    _bodyMax = 0;
}

httpRequest::httpRequest(const httpRequest& copy) : _headers(copy._headers)
{
	_requestBuffer = copy._requestBuffer;
	_body = copy._body;
	_method = copy._method;
	_status = copy._status;
	_errorCode = copy._errorCode;
	_bodySize = copy._bodySize;
	_chunkSize = copy._chunkSize;
	_chunkStatus = copy._chunkStatus;

    _path = copy._path;
    _fileFd = copy._fileFd;
    _pathValidated = copy._pathValidated;
    _isChunkedPost = copy._isChunkedPost;
    _bytesWritten = copy._bytesWritten;
    _bodyMax = copy._bodyMax;
}

httpRequest&	httpRequest::operator=(const httpRequest& copy)
{
	_headers = copy._headers;
	_requestBuffer = copy._requestBuffer;
	_body = copy._body;
	_method = copy._method;
	_status = copy._status;
	_errorCode = copy._errorCode;
	_bodySize = copy._bodySize;
	_chunkSize = copy._chunkSize;
	_chunkStatus = copy._chunkStatus;

    _path = copy._path;
    _fileFd = copy._fileFd;
    _pathValidated = copy._pathValidated;
    _isChunkedPost = copy._isChunkedPost;
    _bytesWritten = copy._bytesWritten;
    _bodyMax = copy._bodyMax;
	return *this;
}

httpRequest::~httpRequest() {}

/**********************************************************************************************/

std::string 	httpRequest::getPath() const { return _path; }

int             httpRequest::getFileFd() const { return _fileFd; }

RequestStatus	httpRequest::getStatus() const { return _status; }

void			httpRequest::setStatus(RequestStatus newStatus) { _status = newStatus; }

unsigned int	httpRequest::getErrorCode() const { return _errorCode; }

void	httpRequest::setErrorCode(unsigned int code) { _errorCode = code; }

void	httpRequest::printRequest()
{
	std::cout << "status:" << _status << ";" << std::endl;
	std::cout << "errorCode:" << _errorCode << ";" << std::endl << std::endl;

	std::cout << "method:" << _method << ";" << std::endl;
	std::cout << "path:" << _path << ";" << std::endl;
	std::cout << "httpVersion:" << _httpVersion << ";" << std::endl << std::endl;

	std::cout << "headers:" << std::endl;
	for (HeaderMap::iterator it = _headers.begin(); it != _headers.end(); it++)
		std::cout << it->first << ":" << it->second << ";" << std::endl;
	std::cout << std::endl;

	std::cout << "body:" << std::endl;
	std::cout << _body << ";" << std::endl;
}

std::string remove_sup_after_slash(std::string path) {
    for (int i = path.length() - 1; i >= 0; i--) {
        if (path[i] == '/') {
            std::string test = path.substr(0, i + 1);
            return (test);
        }
    }
    return (path);
}

// return = 1 == Can Get / -1 block and error 405 / 0 can't get but can continue
int    httpRequest::can_requested(const Server &server, const std::string request) {
    std::string path = this->_path;
    if (path == "/favicon.ico") {return true;}
    if (path.find(".") != std::string::npos) {
        path = remove_sup_after_slash(path);
    }
    if (path[path.length() - 1] != '/') {
        path += "/";
    }
    std::vector<Location>::const_iterator it_location;
    for (it_location = server.get_location().begin(); it_location != server.get_location().end(); it_location++) {
        std::vector<std::string>::const_iterator it_limit;
        if (it_location->get_path_location() + "/" == path || (path == "/" && it_location->get_path_location() == "/")) {
            if (it_location->get_limit_except().empty()) {
                return (1);
            } else {
                for (it_limit = it_location->get_limit_except().begin(); it_limit != it_location->get_limit_except().end(); it_limit++) {
                    if (*it_limit == request) {
                        return (1);
                    }
                }
                return (-1);
            }
        }
    }
    return (0);
}


/**********************************************************************************************/

/**
 * @brief Converts an HTTP status code into an appropriate reason string.
 */
std::string code_to_string(const unsigned int code) {
    switch (code) {
        // 1xx Informational
        case 100:
            return "Continue";
        case 101:
            return "Switching Protocols";
        // 2xx Success
        case 200:
            return "OK";
        case 201:
            return "Created";
        case 202:
            return "Accepted";
        case 203:
            return "Non-Authoritative Information";
        case 204:
            return "No Content";
        case 205:
            return "Reset Content";
        case 206:
            return "Partial Content";
        // 3xx Redirection
        case 300:
            return "Multiple Choices";
        case 301:
            return "Moved Permanently";
        case 302:
            return "Found";
        case 303:
            return "See Other";
        case 304:
            return "Not Modified";
        case 305:
            return "Use Proxy";
        case 307:
            return "Temporary Redirect";
        case 308:
            return "Permanent Redirect";
        // 4xx Client Error
        case 400:
            return "Bad Request";
        case 401:
            return "Unauthorized";
        case 402:
            return "Payment Required";
        case 403:
            return "Forbidden";
        case 404:
            return "Not Found";
        case 405:
            return "Method Not Allowed";
        case 406:
            return "Not Acceptable";
        case 407:
            return "Proxy Authentication Required";
        case 408:
            return "Request Timeout";
        case 409:
            return "Conflict";
        case 410:
            return "Gone";
        case 411:
            return "Length Required";
        case 412:
            return "Precondition Failed";
        case 413:
            return "Payload Too Large";
        case 414:
            return "URI Too Long";
        case 415:
            return "Unsupported Media Type";
        case 416:
            return "Range Not Satisfiable";
        case 417:
            return "Expectation Failed";
        case 421:
            return "Misdirected Request";
        case 426:
            return "Upgrade Required";
        case 428:
            return "Precondition Required";
        case 429:
            return "Too Many Requests";
        case 431:
            return "Request Header Fields Too Large";
        // 5xx Server Error
        case 500:
            return "Internal Server Error";
        case 501:
            return "Not Implemented";
        case 502:
            return "Bad Gateway";
        case 503:
            return "Service Unavailable";
        case 504:
            return "Gateway Timeout";
        case 505:
            return "HTTP Version Not Supported";
        case 511:
            return "Network Authentication Required";
        default:
            return "Undefined";
    }
}

int	ft_stoi(std::string str)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		if (!isdigit(str[i]))
			return -1;
	}
	std::stringstream	ss;
	int n = 0;

	ss << str;
	ss >> n;
	return n;
}

int hexToInt(const std::string& hexStr)
{
    // Using stringstream with hex manipulator
    std::stringstream ss;
    ss << std::hex << hexStr;
    int result;
    ss >> result;

    if (ss.fail())
		return -1;
    return result;
}
