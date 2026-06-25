#include "webserv.hpp"

Conf    *init_conf(char *path_conf) {
    Conf  *conf_c = new Conf();
    if (!conf_c->parse(path_conf)) {
        std::cout << "The syntax of your conf is not correct, replace by the file: " << CONF_SAFE << '\n';
        delete conf_c;
        conf_c = new Conf();
    if (!conf_c->parse(CONF_SAFE)) {
        std::cout << "ERROR of syntax with the file: " << CONF_SAFE << ". Stop the program.\n";
        delete conf_c;
        return NULL;
    }
  }
  return (conf_c);
}
