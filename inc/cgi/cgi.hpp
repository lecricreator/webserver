#pragma once

#include "webserv.hpp"

#include <sstream>
#include <sys/wait.h>

#define EMPTY_FIELD "None"
#define ROOT_DIR_FOR_CGI_EXEC "www/cgi-bin/"

typedef struct s_cgi_info
{
  std::string content_type;
  std::string body;
  std::string status;
} t_cgi_info;

std::string execute_cgi(std::string filename, char **env, char *cgi_data);
int         parse_cgi(const std::string &cgi_output, t_cgi_info &cgi_info);
bool        is_cgi(const std::string &path, std::string &file);
