#include "webserv.hpp"
#include "conf/Location.hpp"

Location::Location() {
}


void    Location::parse_location(std::ifstream *fd_file) {
    std::string line;
    size_t      posi;

    while (std::getline(*fd_file, line)) {
        std::cout << "Locations : " << line << std::endl;
        if ((posi = line.find("}")) != std::string::npos) {
            break;
        }
    }
}
