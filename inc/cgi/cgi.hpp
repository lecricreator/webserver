#pragma once

#include "webserv.hpp"

#include <sstream>
#include <sys/wait.h>

#define WWW_ROOT "www/"
#define CGI_ROOT "cgi-bin/"

std::string execute_cgi(std::string filename, char **env, char *cgi_data);
int         parse_cgi(const std::string &cgi_output, t_response_data &data);
bool        is_cgi(const std::string &path, std::string &file);
