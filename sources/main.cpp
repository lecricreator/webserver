#include "webserv.hpp"

//CGI specs (RFC 3875) instructs script data to be passed through stdin and metadata from env
//the output of the CGI must be a header, a blank line then a body with the output
int main()
{
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
    {
      print(full_path + ": failure");
      return FAILURE;
    }
    print(full_path + ": success");
    print(cgi_output);
  }
  return SUCCESS;
}
