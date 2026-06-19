#include "conf/Set_variable.hpp"

void    Set_variable::add_in_var(const std::string line, size_t posi, std::string *at_replace) {
    posi = put_index_after_space(line, posi);
    for (; posi < line.length(); posi++) {
        if (line[posi] == ';' || line[posi] == ' ' || line[posi] == '{') {
            return ;
        } else if (line[posi] >= 33 && line[posi] <= 126) {
            *at_replace += line[posi];
        } else {
            break ;
        }
    }
    print("Error need to add ';'. Next step need to replace by another file\nSet_variable.cpp:13 / add_in_var // string line probleme :");
    print(line);
    *at_replace = "NULL";
    return ;
}

void    Set_variable::add_in_var(const std::string line, size_t posi, int *at_replace) {
    std::string tmp_val = "";
    posi = put_index_after_space(line, posi);
    for (; posi < line.length(); posi++) {
        if (line[posi] == ';') {
            *at_replace = to_int(tmp_val);
            return ;
        } else if (line[posi] >= '0' && line[posi] <= '9') {
            tmp_val += line[posi];
        } else {
            *at_replace = -1;
        }
    }
    print("Error need to add ';'. Next step need to replace by another file\nSet_variable.cpp:13 / add_in_var // string line probleme :");
    print(line);
    *at_replace = -1;
    return ;
}

void    Set_variable::add_in_var(const std::string line, size_t posi, bool *at_replace) {
    if ((posi = line.find("true")) != std::string::npos) {
        *at_replace = true;
    }
}

void    Set_variable::add_in_var(const std::string line, size_t posi, std::vector<std::string> *at_replace) {
    std::string tmp_val;
    posi = put_index_after_space(line, posi);
    for (; posi < line.length(); posi++) {
        if (line[posi] == ';' || line[posi] == '{') {
            if (!line.empty())
                at_replace->push_back(tmp_val);
            return ;
        } else if (line[posi] == ' ') {
            at_replace->push_back(tmp_val);
            tmp_val = "";
        } else if (line[posi] >= 33 && line[posi] <= 126) {
            tmp_val += line[posi];
        } else {
            break ;
        }
    }
    print("Error need to add ';'. Next step need to replace by another file\nSet_variable.cpp:13 / add_in_var // string line probleme :");
    print(line);
    return ;
}

void        Set_variable::add_in_var(const std::string line, size_t posi, std::vector<int> *at_replace) {
    std::string tmp_val;
    posi = put_index_after_space(line, posi);
    for (; posi < line.length(); posi++) {
        if (line[posi] == ';' || line[posi] == '{') {
            if (!tmp_val.empty()){
                at_replace->push_back(to_int(tmp_val));
                print(tmp_val);
            }
            return ;
        } else if (line[posi] == ' ') {
            at_replace->push_back(to_int(tmp_val));
            print(tmp_val);
            tmp_val = "";
        } else if (line[posi] >= 33 && line[posi] <= 126) {
            tmp_val += line[posi];
        } else {
            break ;
        }
    }
    print("Error need to add ';'. Next step need to replace by another file\nSet_variable.cpp:13 / add_in_var // string line probleme :");
    print(line);
    return ;
}
