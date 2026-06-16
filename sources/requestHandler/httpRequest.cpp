#include "httpRequest.hpp"

httpRequest::httpRequest() : _headers()
{
	_requestBuffer = "";
	_body = "";
	_method = "";
	_httpVersion = "";
	_status = REQ_START_LINE;
}

httpRequest::httpRequest(const httpRequest& copy) : _headers(copy._headers)
{
	_requestBuffer = copy._requestBuffer;
	_body = copy._body;
	_method = copy._method;
	_httpVersion = _httpVersion;
	_status = copy._status;
}

httpRequest&	httpRequest::operator=(const httpRequest& copy)
{
	_headers = copy._headers;
	_requestBuffer = copy._requestBuffer;
	_body = copy._body;
	_method = copy._method;
	_httpVersion = _httpVersion;
	_status = copy._status;	
}

httpRequest::~httpRequest() {}

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
			return false; //wrong method
	}
	else
		return false; //not enough params
	if (startLine.find(" ") != std::string::npos)
	{
		buffer = startLine.substr(0, startLine.find(" "));
		startLine.erase(0, startLine.find(" ") + 1);
		_path = buffer;
		buffer.clear();

		//idk how to check for correct path
	}
	else
		return false; //not enough params
	if (startLine.find(" ") != std::string::npos)
	{
		buffer = startLine.substr(0, startLine.find(" "));
		startLine.erase(0, startLine.find(" ") + 1);
		_httpVersion = buffer;
		buffer.clear();

		if (_httpVersion != "HTTP/1.1")
			return false; //wrong http version
	}
	else
		return false; //not enough params
	if (startLine.find(" ") != std::string::npos)
		return false; //too many params
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