#include "cgi.hpp"

static bool is_extension_valid(const std::string &extension)
{
  if (extension == ".py")
    return true;
  return false;
}

static std::string extract_string(const std::string &path, const size_t &start, const size_t &end)
{
  if (end != std::string::npos)
    return path.substr(start, end - start);
  return path.substr(start);
}

//dot_string is the string coming after the '.'
bool is_cgi(const std::string &path, std::string &file)
{
  std::string upload_path = "www/upload/";
  bool        start_slash = path[0] == '/';
  if (path.compare(start_slash, upload_path.size(), upload_path) == 0)
    return false;
  for (size_t dot_position = path.find(".");
      dot_position != std::string::npos;
      dot_position = path.find(".", dot_position + 1)
      )
  {
    size_t dot_string_end = path.find_first_of("/?", dot_position);
    if (is_extension_valid(extract_string(path, dot_position, dot_string_end)))
    {
      size_t slash_pos = path.find_last_of("/?", dot_position);
      if (slash_pos != std::string::npos && path[slash_pos] == '?')
        break ;
      size_t file_start = (slash_pos == std::string::npos) ? 0 : slash_pos + 1;
      file = extract_string(path, file_start, dot_string_end);
      return true;
    }
  }
  return false;
}
