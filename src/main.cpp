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
  Conf *conf_c = init_conf(argv[1]);
  if (conf_c == NULL) {
    return FAILURE;
  }
  std::map<int, Server> servers = create_server(*conf_c);
  int status = manage_events(servers, *conf_c);
  delete conf_c;
  if (status == ERROR)
    return FAILURE;
  }
  delete conf_c;
  return SUCCESS;
}
