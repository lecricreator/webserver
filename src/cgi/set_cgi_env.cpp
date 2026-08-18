#include "webserv.hpp"

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

char	**httpRequest::set_cgi_env(const std::string &script_name)
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
		path_info = _path;
	env.push_back("REQUEST_METHOD=" + _method);
	env.push_back("QUERY_STRING=" + query);
	env.push_back(std::string("CONTENT_LENGTH=") + to_str(_bodySize));
  env.push_back("SCRIPT_NAME=" + script_name);
	env.push_back("PATH_INFO=" + path_info);
  for (long unsigned i = 0;i<env.size();i++)
    print(env[i]);
  return (vector_to_env(env));
}
