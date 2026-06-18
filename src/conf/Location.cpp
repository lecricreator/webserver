#include "webserv.hpp"
#include "conf/Location.hpp"

Location::Location() {
}


void    Location::parse_location(std::ifstream *fd_file, std::string pline, size_t posi) {
    std::string line;

    this->add_in_var(pline, posi, &this->_path_location);
    while (std::getline(*fd_file, line)) {
        if ((posi = line.find("#")) != std::string::npos) {
            continue ;
        } else if ((posi = line.find("root ")) != std::string::npos) {
            this->add_in_var(line, posi + 5, &this->_root);
        } else if ((posi = line.find("index ")) != std::string::npos) {
            this->add_in_var(line, posi + 6, &this->_index);
        } else if ((posi = line.find("}")) != std::string::npos) {
            break;
        }
    }
}

void    Location::add_in_var(std::string line, size_t posi, std::string *at_replace) {
    posi = put_index_after_space(line, posi);
    for (; posi < line.length(); posi++) {
        if (line[posi] == ';' || line[posi] == ' ' || line[posi] == '{') {
            return ;
        } else if (line[posi] >= 33 && line[posi] <= 125) {
            *at_replace += line[posi];
        } else {
            *at_replace = "NULL";
            return ;
        }
    }
    print("Error need to add ';'. Next step need to replace by another file\nLocation.cpp / add_in_var");
    *at_replace = "NULL";
    return ;
}

void    Location::add_in_var(std::string line, size_t posi, int *at_replace) {
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
            return ;
        }
    }
    print("Error need to add ';'. Next step need to replace by another file\nLocation.cpp:13 / add_in_var");
    *at_replace = -1;
    return ;
}

void    Location::add_in_var(std::string line, size_t posi, bool *at_replace) {
    if ((posi = line.find("true")) != std::string::npos) {
        *at_replace = true;
    }
}
