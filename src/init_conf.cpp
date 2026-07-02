#include "webserv.hpp"

void    print_error_conf(ErrorConf error_numb) {
    std::cout << "conf() failed: ";
    if (error_numb == NO_END_BRACKET_HTTP) {
        std::cout << "'}' is missing in http in the conf file" << std::endl;
    } else if (error_numb == NO_END_BRACKET_SERVER) {
        std::cout << "'}' is missing in server in the conf file" << std::endl;
    } else if (error_numb == NO_END_BRACKET_LOCATION) {
        std::cout << "'}' is missing in location in the conf file" << std::endl;
    } else if (error_numb == NO_END_BRACKET_EVENTS) {
        std::cout << "'}' is missing in EVENTS in the conf file" << std::endl;
    } else if (error_numb == TOO_MUCH_BRACKET) {
        std::cout << "Too much '}'" << std::endl;
    } else if (error_numb == EMPTY_OR_MISSING) {
        std::cout << "The file is empty or an element is missing" << std::endl;
    } else if (error_numb == FILE_NOT_EXIST) {
        std::cout << "The path file not existing" << std::endl;
    } else if (error_numb == NO_SEMICOLON) {
        std::cout << "';' is missing" << std::endl;
    } else if (error_numb == VALUE_NOT_EMPTY) {
        std::cout << "The variable in not empty, has already been write" << std::endl;
    }
}

Conf    *init_conf(char *path_conf) {
    std::ifstream   fd_file;

    fd_file.open(path_conf);

    Conf  *conf_c = new Conf();
    if (fd_file.is_open()) {
        if (!conf_c->parse(fd_file)) {
            std::string str_path = path_conf;
            if (str_path == CONF_SAFE) {
                print("You use the safe file and he's wrong. he canno't continue. Stop the program.");
                fd_file.close();
                return NULL;
            }
            std::cout << "Replace by the file: " << CONF_SAFE << '\n';
            delete conf_c;
            conf_c = new Conf();
        } else {
            fd_file.close();
            return (conf_c);
        }
    } else {
        print_error_conf(FILE_NOT_EXIST);
    }
    fd_file.close();
    fd_file.open(CONF_SAFE);
    if (fd_file.is_open()) {
        if (!conf_c->parse(fd_file)) {
            std::cout << "ERROR of syntax with the file: " << CONF_SAFE << ". Stop the program.\n";
            delete conf_c;
            fd_file.close();
            return NULL;
        }
    } else {
        print_error_conf(FILE_NOT_EXIST);
    }
    fd_file.close();
    return (conf_c);
}
