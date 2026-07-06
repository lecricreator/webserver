#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include <cerrno>
# include <fcntl.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <sys/socket.h>
# include <sys/epoll.h>
# include <arpa/inet.h>
# include <iostream>
# include <fstream>
# include <list>

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
		
		std::string		_method;
		std::string		_path;
		std::string		_httpVersion;
		HeaderMap		_headers;
		std::string		_body;

		unsigned int	_errorCode;
		int				_bodySize;
		int				_chunkSize;

		httpRequest(const httpRequest& copy);
		httpRequest	&operator=(const httpRequest& copy);
	
		bool	parseHexSize();
		bool	parseChunkData();
		bool	parseChunked();
		bool	parseFixedLength();
		bool	parseBody();
		
		bool	isValidHeaderValue(const std::string& value);
		bool	isValidHeaderKey(const std::string& key);
		bool	parseHeaders();
		
		std::string	decodePath();
		bool		isValidPercentEncoding(const std::string& path);
		bool		isValidUriString(const std::string& uri);
		bool		checkPath();
		bool		parseStartLine(std::string& startLine);

	public:
		httpRequest();
		~httpRequest();

		unsigned int	getErrorCode() const;
		void			setErrorCode(unsigned int code);
		void			printRequest();

		bool	parseRequest(std::string& str);

};

std::string code_to_string(const unsigned int code);
int			ft_stoi(std::string n);
int 		hexToInt(const std::string& hexStr);

#endif