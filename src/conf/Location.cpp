#include "conf/Location.hpp"
#include "conf/Set_variable.hpp"
#include "conf/Conf.hpp"

bool    Location::parse_location(std::ifstream *fd_file, const std::string pline, size_t posi) {
    std::string line;

    this->set.add_in_var(pline, posi, &this->_path_location);
    while (std::getline(*fd_file, line)) {
        if ((posi = line.find("#")) != std::string::npos) {
            continue ;
        } else if ((posi = line.find("root ")) != std::string::npos) {
            this->set.add_in_var(line, posi + 5, &this->_root);
        } else if ((posi = line.find("index ")) != std::string::npos) {
            this->set.add_in_var(line, posi + 6, &this->_index);
        } else if ((posi = line.find("}")) != std::string::npos) {
            return (true);
        }
    }
    print_error_conf(NO_END_BRACKET_LOCATION);
    return (false);
}
