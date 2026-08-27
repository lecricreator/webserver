#pragma once

#include "webserv.hpp"

#include <sstream>
#include <sys/wait.h>

#define WWW_ROOT "www/"
#define CGI_ROOT "cgi-bin/"

int         execute_cgi(std::string path, char **env, const char *cgi_input_data, t_parse_data &parse_data);
int         parse_cgi(const std::string &cgi_output, t_response_data &data);
bool        is_cgi(const std::string &path, std::string &file);
