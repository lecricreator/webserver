#include "httpRequest.hpp"

httpRequest::httpRequest() : _headers()
{
	_requestBuffer = "";
	_body = "";
	_method = "";
	_httpVersion = "";
	_status = REQ_START_LINE;
	_errorCode = 0;
	_bodySize = -1;
}

httpRequest::httpRequest(const httpRequest& copy) : _headers(copy._headers)
{
	_requestBuffer = copy._requestBuffer;
	_body = copy._body;
	_method = copy._method;
	_httpVersion = _httpVersion;
	_status = copy._status;
	_errorCode = copy._errorCode;
	_bodySize = copy._bodySize;
}

httpRequest&	httpRequest::operator=(const httpRequest& copy)
{
	_headers = copy._headers;
	_requestBuffer = copy._requestBuffer;
	_body = copy._body;
	_method = copy._method;
	_httpVersion = _httpVersion;
	_status = copy._status;
	_errorCode = copy._errorCode;
	_bodySize = copy._bodySize;
}

httpRequest::~httpRequest() {}

void	httpRequest::setErrorCode(unsigned int code) { _errorCode = code; }

bool isValidPercentEncoding(const std::string& path) {
    for (size_t i = 0; i < path.length(); i++) {
        if (path[i] == '%') {
            if (i + 2 >= path.length() || !isxdigit(path[i+1]) || !isxdigit(path[i+2]))
                return false;
            i += 2;  // skip the two hex digits
        }
    }
    return true;
}

bool	httpRequest::checkPath()
{
	if (_path.empty() || _path[0] != '/')
		return false;
	if (_path.find("/../") != std::string::npos 
	|| _path.rfind("/..") == _path.length() - 3 
	|| _path == "/.." || _path == "..")
		return false;
	if (!isValidPercentEncoding(_path))
		return false;
	for (int i = 0; i < _path.size(); i++)
	{
		if (_path[i] <= 32 || _path[i] == 127)
			return false;
	}
	if (_path.size() > 2048)
		return false; //error code 414
	if (_path.find('\0') != std::string::npos)
		return false;
	return true;
} 

bool	httpRequest::parseStartLine(std::string &startLine)
{
	std::string buffer;
	if (startLine.find(" ") != std::string::npos)
	{
		buffer = startLine.substr(0, startLine.find(" "));
		startLine.erase(0, startLine.find(" ") + 1);
		_method = buffer;
		buffer.clear();

		if (_method != "GET" && _method != "POST" && _method != "DELETE")
		{
			setErrorCode(405);
			return false;
		}
	}
	else
	{
		setErrorCode(400);
		return false;
	}
	if (startLine.find(" ") != std::string::npos)
	{
		buffer = startLine.substr(0, startLine.find(" "));
		startLine.erase(0, startLine.find(" ") + 1);
		_path = buffer;
		buffer.clear();

		if (!checkPath())
		{
			setErrorCode(400);
			return false;
		}
	}
	else
	{
		setErrorCode(400);
		return false;
	}
	if (startLine.find(" ") != std::string::npos)
	{
		buffer = startLine.substr(0, startLine.find(" "));
		startLine.erase(0, startLine.find(" ") + 1);
		_httpVersion = buffer;
		buffer.clear();

		if (_httpVersion != "HTTP/1.1")
		{
			setErrorCode(400);
			return false;
		}
	}
	else
	{
		setErrorCode(400);
		return false;
	}
	if (startLine.find(" ") != std::string::npos)
	{
		setErrorCode(400);
		return false;
	}
	return true;
}

bool	httpRequest::parseChunked()
{

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

bool	httpRequest::parseFixedLength()
{
	if (_bodySize == -1)
	{
		_bodySize = ft_stoi(_headers.find("Content-Length")->second);
		if (_bodySize < 0)
		{
			setErrorCode(400);
			return false;
		}
	}

	int	i = 0;
	while (_body.size() < _bodySize && _requestBuffer[i])
	{
		_body += _requestBuffer[i];
		i++;
	}
	return true;
}

// no body/fixed length/chunked
bool	httpRequest::parseBody()
{
	bool	ret;

	if (_headers.find("Transfer-Encoding") != _headers.end())
		ret = parseChunked();
	if (_headers.find("Content-Length") != _headers.end())
		ret = parseFixedLength();
	if (!ret)
		return false;
	return true;
}

bool	httpRequest::parseRequest(std::string &str)
{
	_requestBuffer.append(str);
	if (_status == REQ_START_LINE)
	{
		if (_requestBuffer.find("\r\n") != std::string::npos)
		{
			std::string	startLine = _requestBuffer.substr(0, _requestBuffer.find("\r\n"));
			_requestBuffer.erase(0, _requestBuffer.find("\r\n") + 2);
			if (!parseStartLine(startLine))
				return false; //send appropriate error code
			_status = REQ_HEADERS;
		}
	}
	if (_status == REQ_HEADERS)
	{
		while (_requestBuffer.find("\r\n") != std::string::npos)
		{
			if (_requestBuffer.find("\r\n\r\n") != std::string::npos)
			{
				_requestBuffer.erase(0, _requestBuffer.find("\r\n\r\n"));
				if (_method == "POST")
					_status = REQ_BODY;
				else
					_status = REQ_PARSED;
				break;
			}
			else
			{
				size_t	sep = _requestBuffer.find(": ");
				size_t	crlf = _requestBuffer.find("\r\n");
				std::string	key = _requestBuffer.substr(0, sep);
				std::string	value = _requestBuffer.substr(sep + 2, crlf);
				_headers.insert(key, value);
				_requestBuffer.erase(0, crlf + 2);
			}
		}
	}
	if (_status == REQ_BODY)
	{
		parseBody();
	}
}

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


/*
-le serv ne va pas recevoir toute la request d'un coup, donc tu dois 
	call parse jusqu'a que tu soies sur que la request est complete
-figure out comment savoir que la request est complete

function parse(data):
    buffer += data
    
    if state == START_LINE:
        if buffer has \r\n:
            extract first line
            split into method, path, version
            state = HEADERS
    
    if state == HEADERS:
        while buffer has complete lines:
            if line is empty (\r\n):
                state = BODY or COMPLETE
                break
            else:
                add header to map
    
    if state == BODY:
        if buffer size >= content_length:
            body = first content_length bytes
            state = COMPLETE
    
    return state == COMPLETE
*/