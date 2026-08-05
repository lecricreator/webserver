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

static bool  get_file_location(std::string &file_location,
                        std::string &target_path, const Server &server)
{
	std::vector<Location>::const_iterator it_location;
    std::string location_root;

	for (it_location = server.get_location().begin(); it_location != server.get_location().end(); it_location++)
	{
    location_root = it_location->get_root();
    std::string current_path = it_location->get_path_location();

    bool is_target_path_a_dir = target_path[target_path.length() - 1] == '/';

		if (current_path == target_path && !is_target_path_a_dir)
			return false;
		else if (current_path + "/" == target_path)
		{
      target_path += it_location->get_index()[0];
      break ;
		}
		else
        continue ;
	}
  file_location = location_root + target_path;
  return true;
}

static std::string copy_file_to_str(std::ifstream &file)
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

static int validate_file(const Server &server, std::string &path, std::ifstream &file)
{
  std::string file_location;
  if (!get_file_location(file_location, path, server))
    return 301;

	file.open(file_location.c_str());
	if (access(file_location.c_str(), F_OK) == -1)
		return 404;
  if (access(file_location.c_str(), R_OK) == -1 || !file.is_open())
    return 500;
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
  return status_code;
}
