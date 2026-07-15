#include "webserv.hpp"

//py test files are named from 1 to 7
int main()
{
  std::string response;
  std::string cgi_dir = "cgi/";
  for (char i = '1'; i <= (char)'7'; i++)
  {
    std::string full_path = cgi_dir + i + ".py";
    if (get_cgi_response(response, full_path) == FAILURE)
      continue;
    print("------------" + full_path + " response:");
    print(response);
    print("");
  }
  print("/!\\ must implement response creation");
  //must check if same header is present twice or if there's an invalid header
  return SUCCESS;
}

