#include "webserv.hpp"
#include "conf/Events.hpp"

Events::Events() {
    this->_worker_connections = 5000;
}


void    Events::parse_events(std::ifstream *fd_file) {
    std::string line;
    size_t      posi;

    while (std::getline(*fd_file, line)) {
        if ((posi = line.find("#")) != std::string::npos) {
            continue ;
        } else if ((posi = line.find("}")) != std::string::npos) {
            break;
        }
    }
}
