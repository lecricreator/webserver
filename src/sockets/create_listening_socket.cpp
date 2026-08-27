#include "webserv.hpp"

//handles only IPv4
static int create_server_socket()
{
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (server_fd == ERROR)
    return print_function_error("socket"), ERROR;
  int opt = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == ERROR)
    return print_function_error("setsockopt"), ERROR;
  
  print_success("socket", "fd", server_fd);
  return server_fd;
}

uint32_t ip_to_addr(int a, int b, int c, int d)
{
    return htonl(
        (static_cast<uint32_t>(a) << 24) |
        (static_cast<uint32_t>(b) << 16) |
        (static_cast<uint32_t>(c) << 8)  |
        static_cast<uint32_t>(d)
    );
}

uint32_t assign_ip(std::string server_name) {
  if (server_name == "localhost" || server_name == "127.0.0.1") {
    return (ip_to_addr(127, 0, 0, 1));
  }
  //if (server_name.find(".") != std::string::npos && server_name.length() <= 15) {
  //  int ip[4] = {0, 0, 0, 0};
  //  std::string tmp;
  //  int i_ip = 0;
  //  for (long unsigned i = 0; i > server_name.length(); i++) {
  //    if (server_name[i] == '.') {
  //      ip[i_ip] = to_int(tmp);
  //      tmp.clear();
  //      print("Result is : " + ip[i_ip]);
  //    } else if (server_name[i] >= '0' && server_name[i] <= '9') {
  //      tmp += server_name[i];
  //    } else {
  //        print("Server name is not a number. \nIt has been replace by 127.0.0.1");
  //        return (ip_to_addr(127,0,0,1));
  //    }
  //  }
  //}
  print("\nServer name is incorrect. \nIt has been replace by 127.0.0.1\n");
  return (ip_to_addr(127,0,0,1));
}

//handles only IPv4
static int bind_address(int server_fd, int port, std::string server_name)
{
  struct sockaddr_in addr;

  if (port < 0)
    return print_error("negative port isn't valid"), ERROR;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family      = AF_INET;
  addr.sin_port        = htons(port);
  addr.sin_addr.s_addr = assign_ip(server_name);

  if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) == ERROR)
    return print_function_error("bind"), ERROR;
  print_success("bind", "port", port);
  return SUCCESS;
}

static int listen_address(int server_fd, int connexion_queue_size, int port, std::string server_name)
{
  (void)server_name;
  if (listen(server_fd, connexion_queue_size) == ERROR)
    return print_function_error("listen"), close(server_fd);
  print_success("listen", "queue", connexion_queue_size);
  print("Server listening on http://" + server_name + ":" + to_str(port));
  return SUCCESS;
}

int create_listening_socket(int port, std::string server_name)
{
  int connexion_queue_size = 10;
  print("");
  int server_fd = create_server_socket();
  if (server_fd == ERROR)
    return ERROR;
  if (bind_address(server_fd, port, server_name) == ERROR)
    return ERROR;
  if (listen_address(server_fd, connexion_queue_size, port, server_name) == ERROR)
    return ERROR;
  return server_fd;
}
