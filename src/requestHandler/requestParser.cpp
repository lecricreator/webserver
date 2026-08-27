#include "webserv.hpp"
#include "cgi.hpp"

/**********************************************************************************************/

bool	httpRequest::parseHexSize()
{
	size_t		crlf = _requestBuffer.find("\r\n");
	std::string	hexStr = "";

	if (crlf == 0)
	{
		_chunkSize = 0;
		return true;
	}
	for (size_t i = 0; i < crlf; i++)
	{
		if (!isxdigit(_requestBuffer[i]))
		{
			if (i > 0 && _requestBuffer[i] == ';')
				break;
			setErrorCode(400);
			return false;
		}
		hexStr += _requestBuffer[i];
	}

	_chunkSize = hexToInt(hexStr);
	if (_chunkSize == -1)
	{
		setErrorCode(400);
		return false;
	}
	_requestBuffer.erase(0, crlf + 2);
	return true;
}

bool	httpRequest::parseChunkData()
{
	/*this will return 400 if the body is the right size
	but contains a crlf, compare against nginx maybe*/
	int	trueSize = _requestBuffer.find("\r\n");
	if (_chunkSize != trueSize)
	{
		setErrorCode(400);
		return false;
	}
	std::string script_name;
	if (is_cgi(_path, script_name))
	{
		//std::cout << "CGI REQUEST1\n";
		_body.append(_requestBuffer.substr(0, trueSize));
		_bytesWritten += trueSize;
	}
	else
	{
		if (write(_fileFd, _requestBuffer.substr(0, trueSize).c_str(), trueSize) != -1)
			_bytesWritten += trueSize;
		else
		{
			//std::cout << "parseChunkData() ERROR\n errno: " << errno << "\n";
			setErrorCode(500);
			return false;
		}
	}
	_requestBuffer.erase(0, trueSize + 2);
	return true;
}

bool	httpRequest::parseChunked()
{
	std::cout << "parseChunked() call\n";
	if (_headers["Transfer-Encoding"] != "chunked")
	{
		setErrorCode(400);
		return false;
	}
	while (_requestBuffer.find("\r\n") != std::string::npos)
	{
		if (_bytesWritten > (size_t)_bodyMax)
		{
			setErrorCode(413); //413 Payload too large
			return false;
		}
		if (_chunkStatus == CHUNK_SIZE)
		{
			if (!parseHexSize())
				return false;
			_chunkStatus = CHUNK_DATA;
			continue;
		}
		if (_chunkSize == 0)
		{
			_status = REQ_PARSED;
			//std::cout << "calling close() with return code " << close(_fileFd) << "\n";
			return true;
		}
		if (_chunkStatus == CHUNK_DATA)
		{
			if (!parseChunkData())
				return false;
			_chunkStatus = CHUNK_SIZE;
			continue;
		}
	}
	return true;
}

/**
 * if the client sends a body size larger than the actual body,
 * the function will never set the status to REQ_PARSED, eventually
 * causing a timeout, this is expected behavior
 */
bool	httpRequest::parseFixedLength()
{
	//std::cout << "parseFixedLength() call\n";
	if (_bodySize == -1)
	{
		_bodySize = ft_stoi(_headers.find("Content-Length")->second);
		if (_bodySize < 0)
		{
			setErrorCode(400);
			return false;
		}
		if (_bodySize == 0)
		{
			_status = REQ_PARSED;
			//std::cout << "calling close() with return code " << close(_fileFd) << "\n";
			return true;
		}
		if (_bodySize > _bodyMax)
		{
			setErrorCode(413); //413 Payload too large
			return false;
		}
	}

	size_t	i = 0;
	while (i < _requestBuffer.size())
	{
		//std::cout << "_bytesWritten: " << _bytesWritten << "\n";
		//std::cout << "_bodySize: " << _bodySize << "\n";
		std::string script_name;
		if (is_cgi(_path, script_name))
		{
			_body += _requestBuffer[i];
			_bytesWritten++;
		}
		else
		{
			if (write(_fileFd, &_requestBuffer[i], 1) != -1)
				_bytesWritten++;
			else
			{
				//std::cout << "parseFixedLength() ERROR\n errno: " << errno << "\n";
				setErrorCode(500);
				return false;
			}
		}
		if (_bytesWritten == (size_t)_bodySize)
		{
			_status = REQ_PARSED;
			//std::cout << "calling close() with return code " << close(_fileFd) << "\n";
			return true;
		}
		i++;
	}
	return true;
}

bool	httpRequest::parseBody()
{
	if (_headers.find("Transfer-Encoding") != _headers.end())
	{
		if (!parseChunked())
			return false;
		return true;
	}
	else if (_headers.find("Content-Length") != _headers.end())
	{
		if (!parseFixedLength())
			return false;
		//std::cout << "status: " << _status << "\n";
		//std::cout << "REQ_PARSED: " << REQ_PARSED << "\n";
		return true;
	}
	else
	{
		_status = REQ_PARSED;
		//std::cout << "calling close() with return code " << close(_fileFd) << "\n";
		return true;
	}
	return true;
}

/**********************************************************************************************/

bool	httpRequest::isValidHeaderValue(const std::string& value)
{
	std::string validChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_ :;.,\\/\"'?!(){}[]@<>=-+*#$&`|~^%";
	if (value.find_first_not_of(validChars) != std::string::npos)
		return false;
	return true;
}

bool	httpRequest::isValidHeaderKey(const std::string& key)
{
	std::string validChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_.~:/?#[]@!$&'()*+,;=";
	if (key.find_first_not_of(validChars) != std::string::npos)
		return false;
	return true;
}

bool	httpRequest::parseHeaders()
{
	while (_requestBuffer.find("\r\n") != std::string::npos)
	{
		if (_requestBuffer.find("\r\n") == 0)
		{
			_requestBuffer.erase(0, 2);
			if (_method == "POST")
				_status = REQ_BODY;
			else
				_status = REQ_PARSED;
			return true;
		}
		if (_headers.size() > HEADER_MAX)
		{
			setErrorCode(413); //413 Payload Too Large
			return false;
		}
		if (_requestBuffer.find(": ") == std::string::npos)
		{
			setErrorCode(400);
			return false;
		}
		size_t	sep = _requestBuffer.find(": ");
		size_t	crlf = _requestBuffer.find("\r\n");
		std::string	key = _requestBuffer.substr(0, sep);
		std::string	value = _requestBuffer.substr(sep + 2, crlf - (sep + 2));
		if (!isValidHeaderKey(key) || !isValidHeaderValue(value))
		{
			setErrorCode(400);
			return false;
		}
		if (_headers.find(key) != _headers.end())
		{
			setErrorCode(400);
			return false;
		}
		_headers.insert(std::pair<std::string, std::string>(key, value));
		_requestBuffer.erase(0, crlf + 2);
	}
	return true;
}

/**********************************************************************************************/

std::string httpRequest::decodePath()
{
    std::string decoded;
    decoded.reserve(_path.size());

    for (size_t i = 0; i < _path.size(); ++i) {
        if (_path[i] == '%') {
            if (i + 2 < _path.size()) {
                std::string hexStr;
                hexStr += _path[i + 1];
                hexStr += _path[i + 2];
                int hexInt = hexToInt(hexStr);
                decoded += static_cast<char>(hexInt);
                i += 2;
                continue;
            }
        }
        decoded += _path[i];
    }
    return decoded;
}

bool httpRequest::isValidPercentEncoding(const std::string& path) {
    for (size_t i = 0; i < path.length(); i++) {
        if (path[i] == '%') {
            if (i + 2 >= path.length() || !isxdigit(path[i+1]) || !isxdigit(path[i+2]))
                return false;
            i += 2;  // skip the two hex digits
        }
    }
    return true;
}

bool	httpRequest::isValidUriString(const std::string& uri)
{
	std::string validChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_ :;.,\\/\"'?!(){}[]@<>=-+*#$&`|~^%";
	if (uri.find_first_not_of(validChars) != std::string::npos)
		return false;
	return true;
}

bool	httpRequest::checkPath()
{
	if (_path.empty() || _path[0] != '/')
	{
		setErrorCode(400);
		return false;
	}
	if (_path.find("/../") != std::string::npos
	|| (_path.length() >= 3 && _path.rfind("/..") == _path.length() - 3)
	|| _path == "/.." || _path == "..")
	{
		setErrorCode(400);
		return false;
	}
	if (!isValidUriString(_path))
	{
		setErrorCode(400);
		return false;
	}
	if (!isValidPercentEncoding(_path))
	{
		setErrorCode(400);
		return false;
	}
	for (size_t i = 0; i < _path.size(); i++)
	{
		if (_path[i] <= 32 || _path[i] == 127)
		{
			setErrorCode(400);
			return false;
		}
	}
	if (_path.size() > URI_MAX)
	{
		setErrorCode(414); //414 Uri Too Long
		return false;
	}
	if (_path.find('\0') != std::string::npos)
	{
		setErrorCode(400);
		return false;
	}
	return true;
}

bool	httpRequest::parseStartLine(std::string &startLine)
{
	std::string buffer;
	if (startLine.find(" ") != std::string::npos) //method
	{
		buffer = startLine.substr(0, startLine.find(" "));
		startLine.erase(0, startLine.find(" ") + 1);
		_method = buffer;
		buffer.clear();

		if (_method != "GET" && _method != "POST" && _method != "DELETE")
		{
			setErrorCode(405); //405
			return false;
		}
	}
	else
	{
		setErrorCode(400);
		return false;
	}

	if (startLine.find(" ") != std::string::npos) //path
	{
		buffer = startLine.substr(0, startLine.find(" "));
		startLine.erase(0, startLine.find(" ") + 1);
		_path = buffer;
		buffer.clear();
		if (!checkPath()) //the error code is set inside checkPath
			return false;
		_path = decodePath();
	}
	else
	{
		setErrorCode(400);
		return false;
	}
	if (startLine.find("HTTP/1.1") == 0 && startLine.size() == 8) //http version, hard coded the condition
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
	_status = REQ_HEADERS;
	return true;
}

/**********************************************************************************************/

/**
 * The status of the request gets updated inside the functions parseRequest() calls.
 *
 * In case of an error parseRequest() (or the functions it calls)
 * will set the appropriate error code and return false.
 * It is then up to the function that called parseRequest()
 * to check the error code and handle it appropriately.
  */
int	httpRequest::parseRequest(std::string& str, int bodyMax, const Server &server)
{
	_bodyMax = bodyMax;
	if (!_requestBuffer.empty())
		_requestBuffer.clear();
	_requestBuffer.append(str);
	//print(_requestBuffer);
	if (_status == REQ_START_LINE)
	{
		if (_requestBuffer.size() > URI_MAX)
		{
			setErrorCode(414); //414 URI too long
			return ERROR;
		}
		if (_requestBuffer.find("\r\n") != std::string::npos)
		{
			std::string	startLine = _requestBuffer.substr(0, _requestBuffer.find("\r\n"));
			_requestBuffer.erase(0, _requestBuffer.find("\r\n") + 2);
			if (!parseStartLine(startLine))
				return ERROR;
		}
	}
	if (_status == REQ_HEADERS)
	{
		if (!parseHeaders())
			return ERROR;
	}
	if (_status == REQ_BODY) //Now parses and writes to a file at the same time instead of storing everything in ram
	{
		if (_errorCode == 200 || _errorCode == 0)
		{
			if (can_requested(server, _method) == -1)
			{
				std::cout << _method << "lol\n";
				setErrorCode(405);
				return ERROR;
			}
		}
		std::string	script_name;
	    if (_fileFd == -1 && !is_cgi(_path, script_name))
    	{
			_path.erase(0, 1);
        	_fileFd = open(_path.c_str(), O_CREAT | O_RDWR | O_TRUNC | O_NONBLOCK, 0600);
        	if (_fileFd < 0)
        	{
				//std::cout << "parseRequest() ERROR\n errno: " << errno << "\n";
				//std::cout << "path: " << _path << "\n";
				remove(_path.c_str());
        	    setErrorCode(500);
        	    return ERROR;
        	}
    	}
		if (!parseBody())
		{
			if (!is_cgi(_path, script_name))
				remove(_path.c_str());
			return ERROR;
		}
	}
	if (_status == REQ_PARSED)
		return SUCCESS;
	//std::cout << "parseRequest() return\n";
	return UNFINISHED;
}
