#include "webserv.hpp"

const char* response =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Content-Length: 21\r\n"
    "Connection: close\r\n"
    "\r\n"
    "<h1>Hello World!</h1>";

//handle_client receives and sends a limited nbr of bytes
int main(int argc, char **argv) {
  if (argc != 2) {
    print("Need 2 arguments.");
    return FAILURE;
  }
  Conf  *conf_c = new Conf();
  if (!conf_c->parse(argv[1])) {
    std::cout << "The syntax of your conf is not correct, replace by the file: " << CONF_SAFE << '\n';
    delete conf_c;
    Conf  *conf_c = new Conf();
    if (!conf_c->parse(CONF_SAFE)) {
      std::cout << "ERROR of syntax with the file: " << CONF_SAFE << ". Stop the program.\n";
      return FAILURE;
    }
  }
  int port = 8080;
  int server_fd = create_listening_socket(port);
  if (server_fd == ERROR)
    return FAILURE;
  set_nonblocking(server_fd);
  if (manage_events(response, server_fd) == ERROR)
    return FAILURE;
  return SUCCESS;
}
