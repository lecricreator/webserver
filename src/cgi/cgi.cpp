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

//normally we might reuse the body of other codes than 200 but we can simplify
int cgi(const std::string &path, t_response_data &data, char *env[])
{
	std::string cgi_output;

  std::string relative_path = get_relative_path(path, CGI_ROOT);
  if (relative_path.empty())
    return 401;
	cgi_output = execute_cgi(relative_path, env, NULL);
	if (cgi_output == std::string())
		return 500;
	if (parse_cgi(cgi_output, data) == FAILURE)
		return 502;
	//else if (is_implemented(data) == FAILURE)
		//return 501;
  return 200;
}
