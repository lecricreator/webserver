#include "webserv.hpp"

//insufficient check. test.png.html would be evaluated as png
static std::string choice_content_type(std::string target_path) {
	std::string content_type;
	if ((target_path.find(".png") != std::string::npos)) {
		content_type = "image/png";
	} else if ((target_path.find(".css") != std::string::npos)) {
		content_type = "text/css";
	} else if ((target_path.find(".html") != std::string::npos)) {
		content_type = "text/html";
	} else if ((target_path.find(".jpg") != std::string::npos)) {
		content_type = "image/jpg";
	}
	return (content_type);
}

static bool  get_file_location(std::string &file_location,
                        const std::string target_path, const Server &server)
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
      file_location = location_root + target_path + it_location->get_index()[0];
      return true;
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

bool	httpRequest::getResponse(Server &server, std::string &content_type, bool is_cgi_script)
{
  std::string file_location;
  if (!get_file_location(file_location, this->_path, server))
  {
    setErrorCode(301);
    return false;
  }

	std::ifstream	 file(file_location.c_str());
	if (access(file_location.c_str(), F_OK) == -1)
	{
		setErrorCode(404); //404 Not Found
		return false;
	}
  if (access(file_location.c_str(), R_OK) == -1 || !file.is_open())
  {
    setErrorCode(500);
    return false;
  }
  if (!is_cgi_script)
  {
    _responseBody = copy_file_to_str(file);
    content_type = choice_content_type(file_location);
  }
	return true;
}
