#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <stdlib.h>

std::string	to_str(int n);
std::string to_str(float n);
std::string to_str(size_t n);
void		print(std::string str);
void		print(int n);
void    print(char c);
void    print_bool(bool value);
void		print_error(std::string str);
int     to_int(std::string str);
void    *ft_memset(void *s, int c, size_t n);
