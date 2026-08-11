#include "cgi.hpp"

/*
int is_content_type_implemented(const std::string &content_type)
{
	if (content_type.find("text") != std::string::npos)
		return SUCCESS;
	return FAILURE;
}

int is_implemented(const t_response_data &data)
{
	if (is_content_type_implemented(data.content_type) == SUCCESS)
		return SUCCESS;
	return FAILURE;
}
*/

static std::string  get_relative_path(const std::string &path, const std::string &cgi_root)
{
  size_t      cgi_root_size = cgi_root.size();

  if (path.compare(1, cgi_root_size, cgi_root))
    return std::string();
  std::string relative_path = path.substr(cgi_root_size + 1);
  return (relative_path);
}

//normally we might reuse the body of other codes than 200 but we can simplify
int cgi(const std::string &path, t_response_data &data)
{
	std::string cgi_output;
	char *env[] = {
			(char*)"REQUEST_METHOD=GET",
			(char*)"QUERY_STRING=name=Alice",
			NULL
	};

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
