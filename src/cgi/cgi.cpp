#include "cgi.hpp"

t_cgi_info init_cgi_info()
{
	t_cgi_info cgi_info;

	cgi_info.content_type = EMPTY_FIELD;
	cgi_info.status = EMPTY_FIELD;
	cgi_info.body = EMPTY_FIELD;

	return cgi_info;
}

/*
int is_content_type_implemented(const std::string &content_type)
{
	if (content_type.find("text") != std::string::npos)
		return SUCCESS;
	return FAILURE;
}

int is_implemented(const t_cgi_info &cgi_info)
{
	if (is_content_type_implemented(cgi_info.content_type) == SUCCESS)
		return SUCCESS;
	return FAILURE;
}
*/

static std::string  get_relative_path(std::string path, std::string cgi_root)
{
  size_t      cgi_root_size = cgi_root.size();

  if (path.compare(1, cgi_root_size, cgi_root))
    return std::string();
  std::string relative_path = path.substr(cgi_root_size + 1);
  return (relative_path);
}

//normally we might reuse the body of other codes than 200 but we can simplify
int cgi(const std::string &path,
        std::string &status,
        std::string &content_type,
        std::string &body
        )
{
	std::string cgi_output;
	char *env[] = {
			(char*)"REQUEST_METHOD=GET",
			(char*)"QUERY_STRING=name=Alice",
			NULL
	};
	t_cgi_info cgi_info = init_cgi_info();

  if (cgi_info.status == EMPTY_FIELD)
    cgi_info.status = "200 OK";
  std::string relative_path = get_relative_path(path, CGI_ROOT);
  if (relative_path.empty())
    return 401;
	if ((cgi_output = execute_cgi(relative_path, env, NULL)) == std::string())
		return 500;
	if (parse_cgi(cgi_output, cgi_info) == FAILURE)
		return 502;
	//else if (is_implemented(cgi_info) == FAILURE)
		//return 501;
  status = cgi_info.status;
  content_type = cgi_info.content_type;
  body = cgi_info.body;
  return 200;
}
