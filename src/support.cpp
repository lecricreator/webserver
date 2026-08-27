#include "support.hpp"

std::string to_str(int n)
{
    std::ostringstream ss;
    ss << n;
    return ss.str();
}

std::string to_str(size_t n)
{
  std::ostringstream ss;
  ss << n;
  return ss.str();
}

std::string to_str(float n)
{
    std::ostringstream ss;
    ss << n;
    return ss.str();
}

int         to_int(std::string str) {
  std::istringstream ss(str);
  int i;
  ss >> i;
  return i;
}

void        print(std::string str)
{
	std::cout << str << std::endl;
}

void	      print(int n)
{
	std::cout << to_str(n) << std::endl;
}

void	      print(char c)
{
	std::cout << c << std::endl;
}

void	      print_error(std::string str)
{
	std::cerr << str << std::endl;
}

void	      print_bool(bool value)
{
  if (value)
    std::cout << "true" << std::endl;
  else
    std::cout << "false" << std::endl;
}
