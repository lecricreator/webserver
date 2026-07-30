#include "webserv.hpp"

static size_t	find_first_invalid_char(const std::string& str,
              size_t start,
              size_t increment,
              int (*is_valid_char)(int))
{
	for (size_t i = start; i < str.length(); i += increment)
		if (!is_valid_char(str[i]))
			return i;
	return std::string::npos;
}

static bool get_filename_in_path(const std::string& path, std::string& file)
{
  size_t dot_pos = path.rfind('.');
  if (dot_pos == std::string::npos) return false;
  
  size_t start = find_first_invalid_char(path, dot_pos - 1, -1, isalnum);
  if (start == std::string::npos) start = 0;
  else start++;
  
  size_t end = find_first_invalid_char(path, dot_pos + 1, 1, isalnum);
  if (end == std::string::npos) end = path.length();
  
  if (start < dot_pos && end > dot_pos && (start == 0 || path[start - 1] == '/'))
  {
    file = path.substr(start, end - start);
    return true;
  }
  
  std::string truncated = path.substr(0, dot_pos);
  return get_filename_in_path(truncated, file);
}

static char** vector_to_env(const std::vector<std::string>& vec)
{
  const std::size_t size = vec.size();
  char** env = new char*[size + 1];

  for (std::size_t i = 0; i < size; ++i)
  {
    const std::string& s = vec[i];
    const std::size_t len = s.length();
    env[i] = new char[len + 1];
    std::strcpy(env[i], s.c_str());
  }
  env[size] = NULL;
  return env;
}

void free_env(char** env)
{
  if (env == NULL) return;
  for (std::size_t i = 0; env[i] != NULL; ++i)
    delete[] env[i];
  delete[] env;
}

char	**httpRequest::set_cgi_env()
{
	std::vector<std::string> env;
  std::string query;
  std::string path_info;

	size_t delimiter_pos = _path.find('?');
	if (delimiter_pos != std::string::npos)
	{
		query = _path.substr(delimiter_pos + 1);
		path_info = _path.substr(0, delimiter_pos);
	}
	else
	{
		query.clear();
		path_info = _path;
	}
  std::string script_name;
	env.push_back("REQUEST_METHOD=" + _method);
	env.push_back("QUERY_STRING=" + query);
	env.push_back("CONTENT_LENGTH=" + _bodySize);
  if (_headers.find("content_type") != _headers.end())
    env.push_back("CONTENT_TYPE=" + _headers["content_type"]);
  if (get_filename_in_path(_path, script_name))
    env.push_back("SCRIPT_NAME=" + script_name);
	env.push_back("PATH_INFO=" + path_info);
  for (long unsigned i = 0;i<env.size();i++)
    print(env[i]);
  return (vector_to_env(env));
}
