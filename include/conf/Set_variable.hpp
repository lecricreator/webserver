#ifndef SET_VAR
# define SET_VAR
# include <string>
# include <fstream>
# include "support.hpp"

size_t  put_index_after_space(std::string line, size_t index);

class Set_variable {
    public:
        void        add_in_var(std::string line, size_t posi, std::string *at_replace);
        void        add_in_var(std::string line, size_t posi, int *at_replace);
        void        add_in_var(std::string line, size_t posi, bool *at_replace);
};

#endif
