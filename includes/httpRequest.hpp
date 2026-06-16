#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP
#include "webserv.hpp"
#include <string>
#include <map>

enum RequestStatus {
    REQ_EMPTY,			// Nothing received yet
    REQ_START_LINE,		// Parsing request line
    REQ_HEADERS,		// Parsing headers
	REQ_BODY,			// Parsing body
    REQ_PARSED,			// Ready to be processed
    REQ_PROCESSED,		// Fully processed, Response created
    REQ_ERROR			// Error during parsing
};

class httpRequest
{
private:
	RequestStatus						_status;
	std::string							_requestBuffer;
	std::map<std::string, std::string>	_headers;
	std::string							_body;

	std::string							_method;
	std::string							_path;
	std::string							_httpVersion;


public:
	httpRequest();
	httpRequest(const httpRequest& copy);
	httpRequest	&operator=(const httpRequest& copy);
	~httpRequest();

	bool	parseStartLine(std::string &startLine);
	bool	parseRequest(std::string &str);
};

#endif