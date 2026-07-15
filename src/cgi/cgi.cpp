#include "cgi.hpp"

std::string create_response(const t_cgi_info &cgi_info)
{
  return cgi_info.body;
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
  t_cgi_info cgi_info = parse_cgi(cgi_output);
  std::string status = cgi_info.status;
  if (status == "None" || status == "202 OK")
    response = create_response(cgi_info);
  else
    response = "status not 200 OK, fetch page"; //must get the correct status page
  return SUCCESS;
}
