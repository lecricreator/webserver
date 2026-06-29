#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP
#include "webserv.hpp"
#include <string>
#include <sstream>
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

enum ChunkBodyStatus {
	CHUNK_SIZE,
	CHUNK_DATA
};

typedef std::map<std::string, std::string> HeaderMap;

class httpRequest
{
	private:
		RequestStatus	_status;
		ChunkBodyStatus	_chunkStatus;
		std::string		_requestBuffer;
		HeaderMap		_headers;
		std::string		_body;
		
		std::string		_method;
		std::string		_path;
		std::string		_httpVersion;
		unsigned int	_errorCode;
		int				_bodySize;
		int				_chunkSize;


	public:
		httpRequest();
		httpRequest(const httpRequest& copy);
		httpRequest	&operator=(const httpRequest& copy);
		~httpRequest();

		void	setErrorCode(unsigned int code);

		bool	parseRequest(std::string str);
		bool	parseStartLine(std::string &startLine);
		bool	parseHeaders();
		bool	checkPath(); //not implemented
		bool	parseFixedLength();
		bool	parseChunked();
		bool	parseBody();
		bool	parseHexSize(); //not implemented
		bool	parseChunkData(); //not implemented
};

std::string code_to_string(const unsigned int code);
int			ft_stoi(std::string n);
int 		hexToInt(const std::string& hexStr);

#endif