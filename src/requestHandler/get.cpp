#include "webserv.hpp"
#include <dirent.h>

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

std::string listing_directory(const Server &server, std::string _path) {
  (void)server;
  DIR *folder;
  std::string path_directory = "www" + _path;
  folder = opendir(path_directory.c_str());
  struct dirent *entry;
  std::string body = "<body style=\"background-color: purple;\"> <h1 style=\"text-align: center;color: white\">Mais bien le bonsoir venerable grand maistre !!!</h1></body>";
  body += "<h2>Voici vos fichier qui sont dans l'index de ";
  body += _path;
  body += ":</h2><hr><pre style=\"line-height: 20px;\"><ul>";
  while ((entry=readdir(folder))) {
    if ((entry->d_name[0] == '.' && entry->d_name[1] == 0) || (entry->d_name[0] == '.' && entry->d_name[1] == '.' && entry->d_name[2] == 0)) {
      body += "<li>";
      body += entry->d_name;
      body += "/</li>";
    } else {
      body += "<li><a href=";
      body += _path + entry->d_name;
      body += ">";
      body += entry->d_name;
      body += "     trereterterter</a></li>";
    }
  }
  body += "</ul></pre><hr></body>";
  return (body);
}

static int validate_file(const Server &server, std::string &path, std::ifstream &file)
{
  std::string file_location;
  if (!get_file_location(file_location, path, server))
    return 301;

  print("file location is " + file_location);
	file.open(file_location.c_str());
	if (access(file_location.c_str(), F_OK) == -1) {
    std::vector<Location>::const_iterator it_location;
    for (it_location = server.get_location().begin(); it_location != server.get_location().end(); it_location++)
	  {
      std::string current_path = it_location->get_path_location();
      bool is_target_path_a_dir = path[path.length() - 1] == '/';
      std::string total_path;
      std::cout << is_target_path_a_dir << std::endl;
      if (!it_location->get_index().empty()) {
        total_path = current_path + "/" + it_location->get_index()[0];
      } else {
        total_path = current_path + "/";
      }
    	if (total_path == path && !is_target_path_a_dir && it_location->get_autoindex()) {
		  	return 1001;
      }
		  else if (total_path == path && it_location->get_autoindex()) {
		  	return 1001;
      }
    }
		return 404;
  }
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
  if (status_code == 200) {
    data.content_type = choice_content_type(path);
    data.body = copy_file_to_str(file);
  } else if (status_code == 1001) {
    data.body = listing_directory(server, _path);
    data.content_type = "text/html";
    status_code = 200;
  } else if (is_favicon) {
    data.content_type = "text/plain";
    data.status = "200 OK";
    status_code = 200;
  }
  return status_code;
}
