#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <deque>
#include <cstdlib>
#include <ctime>

typedef struct s_current_jacob_data
{
  int previous_jacob_number;
  int jacob_number;
  int insertions_left;
  int maximal_insertion_index;
} t_current_jacob_data;

typedef struct s_loop_data
{
 int index_of_pair;
 int greatest_loser;
} t_loop_data;

std::string	to_str(int n);
std::string to_str(float n);
void		print(std::string str);
void		print(int n);
void	  print(char c);
void	  print_bool(bool value);
void		print_error(std::string str);

bool    is_sorted(std::deque<int> list);
void    fillDequeWithRandom(std::deque<int>& dq, int min, int max, std::size_t count);
void	  print_deque(std::deque<int> list);

//algo support
size_t  binary_search(std::deque<int> *main, int greatest_loser, unsigned long pair_size, int min, int max);
void    init_pend(std::deque<int> *main, std::deque<int> *pend, unsigned long pair_size);
void    set_current_jacob_data(bool is_first, t_current_jacob_data *current_jacob_data);
void    set_loop_data(t_loop_data *loop_data, std::deque<int> *pend, int insertions_left, unsigned int pair_size);
void    transfer_range(std::deque<int> *source, size_t source_start_index, unsigned int source_size,
                      std::deque<int> *destination, size_t destination_index);
