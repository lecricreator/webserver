#include "webserv.hpp"

void  print_function_error(std::string function_name)
{
  std::cerr << function_name + "() failed: " << strerror(errno) << std::endl;
}

void  print_success(std::string function_name, std::string output_name, int output)
{
  std::cerr << function_name + "() ok - " + output_name + " = " << output << std::endl;
}

void  set_nonblocking(int fd)
{
  int flags = fcntl(fd, F_GETFL, 0);
  fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}
