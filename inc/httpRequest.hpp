#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include <cerrno>
# include <cstdio>
# include <fcntl.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <sys/socket.h>
# include <sys/epoll.h>
# include <sys/stat.h>
# include <arpa/inet.h>
# include <iostream>
# include <fstream>
# include <list>
# include <string>
# include <cstring>
# include <sstream>
# include <map>

#define URI_MAX 2048
#define	BODY_MAX 8192 //use conf file to set this value instead of define
#define	HEADER_MAX 100


enum RequestStatus {
    REQ_EMPTY,			// Nothing received yet
    REQ_START_LINE,		// Parsing request line
    REQ_HEADERS,		// Parsing headers
	REQ_BODY,			// Parsing body
    REQ_PARSED,			// Ready to be processed
	REQ_EXECUTED,		// Executed appropriate method, ready to create response
    REQ_PROCESSED,		// Fully processed, Response created
    REQ_ERROR,			// Error during parsing
	POST_CHUNK
};

enum ChunkBodyStatus {
	CHUNK_SIZE,
	CHUNK_DATA
};

typedef struct s_response_data
{
  std::string content_type;
  std::string body;
  std::string status;
  std::string location;
} t_response_data;

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

		//post request variables
		int		_fileFd;
		bool	_pathValidated;
		bool	_isChunkedPost;
		size_t	_bytesWritten;

		//response-related variables
		std::string	_responseBody;

		//requestParser
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

		//getRequest
    unsigned int    getRequest(const Server &server, t_response_data &data);
    t_response_data generateResponseData(const Server &server, const bool &is_cgi_script);

		//postRequest
		//int	postRequest();

		//deleteRequest
		unsigned int	deleteRequest();//not implemented

		//requestProcessing

	public:
		httpRequest();
		httpRequest(const httpRequest& copy);
		httpRequest	&operator=(const httpRequest& copy);
		~httpRequest();

		std::string 	getPath() const;
		int				getFileFd() const;
		RequestStatus	getStatus() const;
		void			setStatus(RequestStatus newStatus);
		unsigned int	getErrorCode() const;
		void			setErrorCode(unsigned int code);
		void			printRequest();

		bool	        parseRequest(std::string& str);
    	std::string	  	executeRequest(const Server &server);

    char	        **set_cgi_env();
};

std::string code_to_string(const unsigned int code);
int			ft_stoi(std::string n);
int 		hexToInt(const std::string& hexStr);
int     cgi(const std::string &path, t_response_data &data);

#endif
