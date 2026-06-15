#include "httpRequest.hpp"

httpRequest::httpRequest() : _headers()
{
	_requestBuffer = "";
	_startLine = "";
	_body = "";
	_method = "";
	_httpVersion = "";
	_status = REQ_START_LINE;
}

httpRequest::httpRequest(const httpRequest& copy) : _headers(copy._headers)
{
	_requestBuffer = copy._requestBuffer;
	_startLine = copy._startLine;
	_body = copy._body;
	_method = copy._method;
	_httpVersion = _httpVersion;
	_status = copy._status;
}

httpRequest&	httpRequest::operator=(const httpRequest& copy)
{
	_headers = copy._headers;
	_requestBuffer = copy._requestBuffer;
	_startLine = copy._startLine;
	_body = copy._body;
	_method = copy._method;
	_httpVersion = _httpVersion;
	_status = copy._status;	
}

httpRequest::~httpRequest() {}

bool	httpRequest::parseRequest(std::string str)
{
	_requestBuffer.append(str);
	if (_status == REQ_START_LINE)
	{
		if (_requestBuffer.find("\r\n") != std::string::npos)
		{
			_startLine = _requestBuffer;
			_requestBuffer.clear();
			//insert parsing/error checking here
			_status = REQ_HEADERS;
		}
	}
	if (_status == REQ_HEADERS)
	{
		while (_requestBuffer.find("\r\n") != std::string::npos)
		{
			if (_requestBuffer.find("\r\n\r\n") != std::string::npos)
			{
				//set status to body/complete accordingly
				_status = REQ_BODY;
				break;
			}
			else
			{	//very ugly, might not even work
				std::string	key = _requestBuffer;
				key.erase(key.begin() + key.find(": "), key.end());
				std::string	value = _requestBuffer;
				value.erase(0, value.find(": ") + 2);
				value.erase(value.begin() + value.find("\r\n"), value.end());
				_headers.insert(key, value);
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