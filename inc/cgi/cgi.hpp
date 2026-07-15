#pragma once

#include "webserv.hpp"

#include <sstream>
#include <sys/wait.h>

typedef struct s_cgi_info
{
  std::string content_type;
  std::string body;
  std::string status;
} t_cgi_info;

//cgi
std::string execute_cgi(const char *filename, char **env, char *cgi_data);
t_cgi_info  parse_cgi(const std::string &cgi_output);
