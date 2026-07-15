#include "cgi.hpp"

std::string create_response(const t_cgi_info &cgi_info)
{
  return cgi_info.body;
}

t_cgi_info init_cgi_info()
{
  t_cgi_info cgi_info;

  cgi_info.content_type = EMPTY_FIELD;
  cgi_info.status = EMPTY_FIELD;
  cgi_info.body = EMPTY_FIELD;

  return cgi_info;
}

//normally we might reuse the body of other codes than 202 but we can simplify
int get_cgi_response(std::string &response, const std::string &full_path)
{
  std::string cgi_output;
  char *env[] = {
      (char*)"REQUEST_METHOD=GET",
      (char*)"QUERY_STRING=name=Alice",
      NULL
  };
  if ((cgi_output = execute_cgi(full_path.c_str(), env, NULL)) == "")
    return print(full_path + ": failure"), FAILURE;
  t_cgi_info cgi_info = init_cgi_info();
  if (parse_cgi(cgi_output, cgi_info) == FAILURE)
    print_error("CGI parsing failed");
  std::string status = cgi_info.status;
  if (status == EMPTY_FIELD || status == "200 OK")
    response = create_response(cgi_info);
  else
    response = "status not 200 OK, fetch page"; //must get the correct status page
  return SUCCESS;
}
