#include "webserv.hpp"
#include <sstream>

typedef struct s_cgi_info
{
  std::string content_type;
  std::string body;
  std::string status;
} t_cgi_info;

bool is_prefix_present(const std::string& str, const std::string& prefix)
{
  return str.compare(0, prefix.size(), prefix) == 0;
}

std::string get_content(const std::string &line, const std::string &prefix)
{
  if (!is_prefix_present(line, prefix))
    return std::string();
  std::string content_type = line.substr(prefix.size());
  return content_type;
}

int parse_content_type(const std::string &line, t_cgi_info &cgi_info)
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
bool is_valid_status_code(std::string status)
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


int parse_status(const std::string &line, t_cgi_info &cgi_info)
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

int parse_cgi_line(std::string line, t_cgi_info &cgi_info, bool reset)
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

void print_cgi_info(const t_cgi_info &cgi_info)
{
  print("");
  print("Parsed Content-Type: " + cgi_info.content_type);
  print("Parsed status: " + cgi_info.status);
  print("Parsed body:\n" + cgi_info.body);
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

std::string create_response(const t_cgi_info &cgi_info)
{
  return cgi_info.body;
}

//normally we might reuse the body of other codes than 202 but we can simplify
std::string get_response(const t_cgi_info &cgi_info)
{
  std::string response;
  std::string status = cgi_info.status;
  if (status == "None" || status == "202 OK")
    response = create_response(cgi_info);
  else
    response = "status not 202 OK, fetch page";
  //must get the correct status page
  return response;
}

//py test files are named from 1 to 7
int main()
{
  std::string response;
  std::string cgi_dir = "cgi/";
  char *env[] = {
      (char*)"REQUEST_METHOD=GET",
      (char*)"QUERY_STRING=name=Alice",
      NULL
  };
  for (char i = '1'; i <= (char)'7'; i++)
  {
    std::string cgi_output;
    std::string full_path = cgi_dir + i + ".py";
    if ((cgi_output = execute_cgi(full_path.c_str(), env, NULL)) == "")
      return print(full_path + ": failure"), FAILURE;
    t_cgi_info cgi_info = parse_cgi(cgi_output);
    //print_cgi_info(cgi_info);
    response = get_response(cgi_info);
    print("------------" + full_path + " response:");
    print(response);
    print("");
  }
  print("/!\\ must implement response creation");
  //should divide into multiple files before continuing
  //must check if same header is present twice or if there's an invalid header
  return SUCCESS;
}
