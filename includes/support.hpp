#pragma once

#include <string>
#include <iostream>
#include <sstream>

std::string	to_str(int n);
std::string to_str(float n);
void		print(std::string str);
void		print(int n);
void	  print(char c);
void	  print_bool(bool value);
void		print_error(std::string str);
