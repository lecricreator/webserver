#include "cgi.hpp"

static bool is_prefix_present(const std::string& str, const std::string& prefix)
{
  return str.compare(0, prefix.size(), prefix) == 0;
}

static std::string get_content(const std::string &line, const std::string &prefix)
{
  if (!is_prefix_present(line, prefix))
    return std::string();
  std::string content_type = line.substr(prefix.size());
  return content_type;
}

//we assume that if the pattern x/y is respected the content_type is respected
//unimplemented content_types must be filtered later
int is_valid_content_type(const std::string &content_type)
{
  if (content_type.find_first_of(" \t") != std::string::npos)
    return FAILURE;
  size_t slash = content_type.find('/');
  if (slash == std::string::npos || slash == 0 || slash == content_type.size() - 1)
    return FAILURE;
  if (content_type.find('/', slash + 1) != std::string::npos)
    return FAILURE;
  return SUCCESS;
}

static int parse_content_type(const std::string &line, t_cgi_info &cgi_info)
{
  const std::string prefix = "Content-Type: ";
  std::string content_type = get_content(line, prefix);
  if (content_type.empty())
    return ERROR;
  else if (is_valid_content_type(content_type) == FAILURE || cgi_info.content_type != EMPTY_FIELD)
    return FAILURE;
  cgi_info.content_type = content_type;
  return SUCCESS;
}

//no 3xx statuses as only document type cgi is implemented
//5xx statuses are for server only
//other code related to getting data were erased as well
static bool is_valid_status_code(const std::string &status)
{
  /*const char* valid_status_arr[] = {"200 OK", "201 Created", "202 Accepted", "204 No Content", "206 Partial Content",
      "300 Multiple Choices", "301 Moved Permanently", "302 Found", "303 See Other", "304 Not Modified", "307 Temporary Redirect", "308 Permanent Redirect",
      "400 Bad Request", "401 Unauthorized", "402 Payment Required", "404 Not Found", "405 Method Not Allowed", "406 Not Acceptable", "409 Conflict", "410 Gone", "415 Unsupported Media Type", "422 Unprocessable Entity"};
      //"500 Internal Server Error", "501 Not Implemented", "502 Bad Gateway", "503 Service Unavailable", "504 Gateway Timeout"};*/
  const char* valid_status_arr[] = {"200 OK", "204 No Content", "206 Partial Content",
      "400 Bad Request", "401 Unauthorized", "404 Not Found", "405 Method Not Allowed", "406 Not Acceptable"};
  std::vector<std::string> valid_status(valid_status_arr, valid_status_arr + 8);
  return std::find(valid_status.begin(), valid_status.end(), status) != valid_status.end();
}

static int parse_status(const std::string &line, t_cgi_info &cgi_info)
{
  const std::string prefix = "Status: ";
  std::string status = get_content(line, prefix);
  if (status.empty())
    return ERROR;
  else if (!is_valid_status_code(status) || cgi_info.status != EMPTY_FIELD)
    return FAILURE;
  cgi_info.status = status;
  return SUCCESS;
}

static int parse_cgi_header_line(std::string &line, t_cgi_info &cgi_info)
{
  if (parse_content_type(line, cgi_info) == SUCCESS
      || parse_status(line, cgi_info) == SUCCESS)
    return SUCCESS;
  print_error("header invalid");
  return FAILURE;
}

static int parse_cgi_header(const std::string &cgi_output, t_cgi_info &cgi_info)
{
  std::string         line;
  std::istringstream  stream(cgi_output);
  while (std::getline(stream, line))
    if (parse_cgi_header_line(line, cgi_info) == FAILURE)
      return FAILURE;
  return SUCCESS;
}

int parse_cgi(const std::string &cgi_output, t_cgi_info &cgi_info)
{
    const char* delimiter = "\r\n\r\n";
    size_t delim_len = 4;
    size_t delimiter_pos = cgi_output.find(delimiter);
    if (delimiter_pos == std::string::npos)
    {
        delimiter = "\n\n";
        delim_len = 2;
        delimiter_pos = cgi_output.find(delimiter);
    }
    if (delimiter_pos == std::string::npos)
        return FAILURE;

    std::string header = cgi_output.substr(0, delimiter_pos);
    std::string body   = cgi_output.substr(delimiter_pos + delim_len);

    if (parse_cgi_header(header, cgi_info) == SUCCESS)
    {
        cgi_info.body = body;
        return SUCCESS;
    }
    return FAILURE;
}
