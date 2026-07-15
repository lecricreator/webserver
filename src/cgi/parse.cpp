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

static int parse_content_type(const std::string &line, t_cgi_info &cgi_info)
{
  std::string prefix = "Content-Type: ";
  std::string content_type = get_content(line, prefix);
  if (content_type.empty())
    return ERROR;
  if (content_type == "text/plain")
  {
    cgi_info.content_type = content_type;
    return SUCCESS;
  }
  return FAILURE;
}

//no 3xx statuses as only document type cgi is implemented
//5xx statuses are for server only
//other code related to getting data were erased as well
static bool is_valid_status_code(std::string status)
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
  std::string prefix = "Status: ";
  std::string status = get_content(line, prefix);
  if (status.empty())
    return ERROR;
  if (is_valid_status_code(status))
  {
    cgi_info.status = status;
    return SUCCESS;
  }
  return FAILURE;
}

static int parse_cgi_line(std::string line, t_cgi_info &cgi_info, bool reset)
{
  static bool is_header = 1;
  if (reset)
  {
    is_header = 1;
    return SUCCESS;
  }

  if (is_header)
  {
    if (parse_content_type(line, cgi_info) == FAILURE)
      return print_error("content_type invalid"), ERROR;
    else if (parse_status(line, cgi_info) == FAILURE)
      return print_error("status code invalid"), ERROR;
    else if (line.empty())
    {
      is_header = 0;
      cgi_info.body = std::string();
    }
  }
  else
    cgi_info.body.append(line + '\n');
  return SUCCESS;
}

t_cgi_info  parse_cgi(const std::string &cgi_output)
{
  std::istringstream  stream(cgi_output);
  t_cgi_info          cgi_info;
  std::string         line;

  cgi_info.content_type = "None";
  cgi_info.status = "None";
  cgi_info.body = "None";

  while (std::getline(stream, line))
    if (parse_cgi_line(line, cgi_info, false) == ERROR)
    {
      print_error("CGI parsing failed");
      break;
    }
  parse_cgi_line(line, cgi_info, true);
  return cgi_info;
}
