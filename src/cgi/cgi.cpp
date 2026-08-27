#include "cgi.hpp"

static std::string  get_relative_path(const std::string &path, const std::string &cgi_root)
{
  size_t      cgi_root_size = cgi_root.size();

  if (path.compare(1, cgi_root_size, cgi_root))
    return std::string();
  std::string relative_path = path.substr(cgi_root_size + 1);
  size_t param_pos = relative_path.find("?");
  if (param_pos != std::string::npos)
    relative_path.erase(param_pos);
  return (relative_path);
}

int cgi(const std::string &path, t_parse_data &parse_data, char *env[], const char *cgi_input_data)
{
  std::string relative_path = get_relative_path(path, CGI_ROOT);
  if (relative_path.empty())
    return 401;
	if (execute_cgi(relative_path, env, cgi_input_data, parse_data) == ERROR)
		return 500;
  return 200;
}

//if (parse_cgi(cgi_output, data) == FAILURE)
//	return 502;
