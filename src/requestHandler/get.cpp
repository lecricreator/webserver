#include "webserv.hpp"

//line.find("}")) != std::string::npos

static std::string get_extension(const std::string &path)
{
  size_t dot_pos = path.rfind('.');
  if (dot_pos == std::string::npos)
    return std::string();
  return path.substr(dot_pos + 1);
}

static std::string  choice_content_type(std::string path) {
    std::string content_type;
    std::string extension = get_extension(path);
    if (extension == "png") {
        content_type = "image/png";
    } else if (extension == "css") {
        content_type = "text/css";
    } else if (extension == "html") {
        content_type = "text/html";
    } else if (extension == "jpg") {
        content_type = "image/jpg";
    } else if (extension == "ico") {
        content_type = "image/vnd.microsoft.icon";
    }
    return (content_type);
}

Location get_location_of_path(const Server &server, const std::string &path)
{
	std::vector<Location>::const_iterator it_location;
  std::string                           longest_subpath;
  Location                              location_of_path;

	for (it_location = server.get_location().begin(); it_location != server.get_location().end(); it_location++)
	{
    std::string subpath = it_location->get_path_location();
    if (subpath.size() < longest_subpath.size() || path.compare(0, subpath.size(), subpath))
      continue ;
    longest_subpath = subpath;
    location_of_path = *it_location;
  }
  return location_of_path;
}

static std::string  get_absolute_path(const Location &location, std::string &path)
{
  std::string location_root = location.get_root();
  std::string current_path = location.get_path_location();

  if (current_path + "/" == path)
    path += location.get_index()[0];
  std::string absolute_path = location_root + path;
  return absolute_path;
}

std::string copy_file_to_str(std::ifstream &file)
{
  std::string   line;
  std::string   copy;

  while (std::getline(file, line))
  {
    copy += line;
    if (!file.eof())
      copy += '\n';
  }
  return copy;
}

static bool should_redirect(const Location &location, const std::string &path)
{
  return location.get_path_location() == path;
}

static int validate_file(const Server &server, std::string &path, std::ifstream &file)
{
  Location location_of_path = get_location_of_path(server, path);
  if (location_of_path.empty())
    return 404;
  if (should_redirect(location_of_path, path))
    return 301;
  std::string absolute_path = get_absolute_path(location_of_path, path);

	file.open(absolute_path.c_str());
	if (access(absolute_path.c_str(), F_OK) == -1)
		return 404;
  if (access(absolute_path.c_str(), R_OK) == -1 || !file.is_open())
    return 403;
  return 200;
}

unsigned int	httpRequest::getRequest(const Server &server, t_response_data &data)
{
	std::ifstream file;

  bool          is_favicon = get_extension(_path) == "ico";
  std::string   path = is_favicon ? "/favicon.ico" : _path;

  int status_code = validate_file(server, path, file);
  if (status_code == 200)
  {
    data.content_type = choice_content_type(path);
    data.body = copy_file_to_str(file);
  }
  else if (is_favicon)
  {
    data.content_type = "text/plain";
    data.status = "200 OK";
    status_code = 200;
  }
  return status_code;
}
