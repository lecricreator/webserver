#include "conf/Set_variable.hpp"
#include "conf/Conf.hpp"

void    Set_variable::add_in_var(const std::string line, size_t posi, std::string *at_replace) {
    if (!at_replace->empty()) {
        print_error_conf(VALUE_NOT_EMPTY);
        print(line);
        return ;
    }
    posi = put_index_after_space(line, posi);
    for (; posi < line.length(); posi++) {
        if (line[posi] == ';' || line[posi] == ' ' || line[posi] == '{') {
            if (at_replace[0] == "/" && at_replace->length() == 1) {
                *at_replace = "";
            }
            return ;
        } else if (line[posi] >= 33 && line[posi] <= 126) {
            *at_replace += line[posi];
        } else {
            break ;
        }
    }
    print_error_conf(NO_SEMICOLON);
    print(line);
    *at_replace = "NULL";
    return ;
}

void    Set_variable::add_in_var(const std::string line, size_t posi, int *at_replace) {
    if (*at_replace != -1) {
        print_error_conf(VALUE_NOT_EMPTY);
        print(line);
        return ;
    }

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
    print_error_conf(NO_SEMICOLON);
    print(line);
    *at_replace = -1;
    return ;
}

void    Set_variable::add_in_var(const std::string line, size_t posi, bool *at_replace) {
    if (*at_replace == true) {
        print_error_conf(VALUE_NOT_EMPTY);
        print(line);
        return ;
    } else if ((posi = line.find("on")) != std::string::npos) {
        *at_replace = true;
    }
    if ((posi = line.find(";")) != std::string::npos) {
        return ;
    } else {
        print_error_conf(NO_SEMICOLON);
        print(line);
    }
}

void    Set_variable::add_in_var(const std::string line, size_t posi, std::vector<std::string> *at_replace) {
    if (!at_replace->empty()) {
        print_error_conf(VALUE_NOT_EMPTY);
        print(line);
        return ;
    }
    std::string tmp_val;
    posi = put_index_after_space(line, posi);
    for (; posi < line.length(); posi++) {
        if (line[posi] == ';' || line[posi] == '{') {
            if (!line.empty()) {
                at_replace->push_back(tmp_val);
            }
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
    print_error_conf(NO_SEMICOLON);
    print(line);
    return ;
}

void        Set_variable::add_in_var(const std::string line, size_t posi, std::vector<int> *at_replace) {
    if (!at_replace->empty()) {
        print_error_conf(VALUE_NOT_EMPTY);
        return ;
    }
    std::string tmp_val;
    posi = put_index_after_space(line, posi);
    for (; posi < line.length(); posi++) {
        if (line[posi] == ';' || line[posi] == '{') {
            if (!tmp_val.empty()){
                at_replace->push_back(to_int(tmp_val));
            }
            return ;
        } else if (line[posi] == ' ') {
            at_replace->push_back(to_int(tmp_val));
            tmp_val = "";
        } else if (line[posi] >= 33 && line[posi] <= 126) {
            tmp_val += line[posi];
        } else {
            break ;
        }
    }
    print_error_conf(NO_SEMICOLON);
    print(line);
    return ;
}

void    Set_variable::add_in_var(const std::string line, size_t posi, std::map<int, std::string> *at_replace) {
    std::string tmp_val_s;
    int         tmp_val_i;
    int         step = 0;
    posi = put_index_after_space(line, posi);
    for (; posi < line.length(); posi++) {
        if (step == 0) {
            if (line[posi] == ' ') {
                tmp_val_i = to_int(tmp_val_s);
                tmp_val_s.empty();
                posi = put_index_after_space(line, posi);
                step = 1;
                continue ;
            } else if (line[posi] == ';') {
                print_error_conf(VALUE_IS_NOT_CORRECT);
            } else if (line[posi] >= '0' && line[posi] >= '9') {
                tmp_val_s += line[posi];
                continue ;
            } else {
                print_error_conf(VALUE_IS_NOT_INT);
                at_replace->insert(std::make_pair(-1, "error"));
                //at_replace[-1] = std::string("ERROR");
                return ;
            }
        } else if (step == 1) {
            if (line[posi] == ';') {
                at_replace->insert(std::make_pair(tmp_val_i, tmp_val_s));
                print("value is " + tmp_val_i + tmp_val_s);
                return ;
            } else {
                tmp_val_s += line[posi];
            }
        }
    }
    print_error_conf(NO_SEMICOLON);
    print(line);
    return ;
}
