#include "webserv.hpp"
#include "conf/Events.hpp"

Events::Events() {
    _worker_connections = 5000;
	_use = "";
}

Events::Events(const Events& copy)
{
	_worker_connections = copy._worker_connections;
	_use = copy._use;
}

Events	&Events::operator=(const Events& copy)
{
	_worker_connections = copy._worker_connections;
	_use = copy._use;
	return *this;	
}

Events::~Events() {}

bool    Events::parse_events(std::ifstream *fd_file) {
    std::string line;
    size_t      posi;

    while (std::getline(*fd_file, line)) {
        if ((posi = line.find("#")) != std::string::npos) {
            continue ;
        } else if ((posi = line.find("}")) != std::string::npos) {
            return (true);
        } else if ((posi = line.find("http") != std::string::npos) || (posi = line.find("server")) != std::string::npos) {
            print_error_conf(NO_END_BRACKET_EVENTS);
            return (false);
        }
    }
    print_error_conf(NO_END_BRACKET_EVENTS);
    return (false);
}
