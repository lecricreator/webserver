#include "webserv.hpp"
#include "cgi.hpp"

#define MAX_EVENTS 64

void set_event(struct epoll_event *event, int flag, int fd)
{
  event->events = flag;
  event->data.fd = fd;
}

int  set_epoll_event(struct epoll_event &event, int fd, int epoll_fd, int new_flag, int change_flag)
{
  set_event(&event, new_flag, fd);
  if (epoll_ctl(epoll_fd, change_flag, fd, &event) == ERROR)
    return print_error("epoll_ctl() error"), ERROR;
  return SUCCESS;
}

t_parse_data create_parse_data(Conf& conf_c, Server &server)
{
  t_parse_data client_info;
  client_info.response = "";
  client_info.conf = &conf_c;
  client_info.server = &server;
  return client_info;
}

void end_connection(int fd, int epoll_fd, std::map<int, t_parse_data> &client_infos)
{
  print("Connection ceased with fd " + to_str(fd));
  epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
  close(fd);
  client_infos.erase(fd);
}

//check error?
int read_fd(const int &fd, std::string &response)
{
  char buf[4096];

  int n = read(fd, buf, sizeof(buf));
  if (n != ERROR)
    response.append(buf, n);
  return n;
}

//leaves cgi_response empty if all wasn't received or an err occured
int  cgi_event(const int &cgi_fd, const pid_t &cgi_pid, std::string &cgi_response)
{
  //int client_fd = cgi_response_fds[fd];
  int bytes_read = read_fd(cgi_fd, cgi_response);
  if (bytes_read == 0) {
    t_response_data parsed_data;
    int status;

    //print("got full cgi response");
    waitpid(cgi_pid, &status, 0);
    if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
      return 502;
    } else if (cgi_response.empty() || parse_cgi(cgi_response, parsed_data) == FAILURE) {
      return 500;
    } else {
      cgi_response = create_response(parsed_data);
    }
    return 200;
  }
  return UNFINISHED;
}

static void manage_requests(struct epoll_event event,
            int epoll_fd, Conf &conf_c, std::map<int, Server> &servers,
            std::map<int, t_parse_data> &client_infos)
{
  static std::map<int, int>           cgi_response_fds;
  int                                 fd = event.data.fd;
  t_response_data                     response_data;

  //print((int)client_infos[fd].request.getErrorCode());
  if (servers.find(fd) != servers.end()) {
    int client_fd = accept_client(fd);
    if (client_fd == ERROR) {
      close(client_fd);
      return;
    }
    set_nonblocking(client_fd);
    if (set_epoll_event(event, client_fd, epoll_fd, EPOLLIN, EPOLL_CTL_ADD) == ERROR)
      return ;
    client_infos.insert(std::pair<int, t_parse_data>(client_fd, create_parse_data(conf_c, servers[fd])));
    client_infos[client_fd].client_fd = client_fd;
  }
  else if (cgi_response_fds.find(fd) != cgi_response_fds.end()) {
    int status_cgi = cgi_event(fd, client_infos[fd].cgi_pid, client_infos[client_infos[fd].client_fd].response);
    if (status_cgi == UNFINISHED) {
      return ;
    } if (status_cgi != 200) {
      client_infos[client_infos[fd].client_fd].response = create_response(set_error_response(*client_infos[fd].server, status_cgi, std::string()));
    }
    if (set_epoll_event(event, client_infos[fd].client_fd, epoll_fd, EPOLLOUT, EPOLL_CTL_MOD) == ERROR)
      return ;
    end_connection(fd, epoll_fd, client_infos);
  }
  else if (event.events & EPOLLIN) {
    client_infos[fd].cgi_fd = INIT_CGI_FD;
    client_infos[fd].request.resetTimer();
    int request_status = handle_request(fd, client_infos[fd], response_data);
    if (request_status == SUCCESS) {
      if (set_epoll_event(event, fd, epoll_fd, EPOLLOUT, EPOLL_CTL_MOD) == ERROR)
        return ;
    }
    else if (request_status == UNFINISHED && client_infos[fd].cgi_fd != INIT_CGI_FD) {
      set_nonblocking(client_infos[fd].cgi_fd);
      //print(client_infos[fd].cgi_fd);
      if (set_epoll_event(event, client_infos[fd].cgi_fd, epoll_fd, EPOLLIN, EPOLL_CTL_ADD) == ERROR)
        return ;
      cgi_response_fds.insert(std::pair<int, int>(client_infos[fd].cgi_fd, fd));
      client_infos.insert(std::pair<int, t_parse_data>(client_infos[fd].cgi_fd, client_infos[fd]));
      client_infos[client_infos[fd].cgi_fd].cgi_fd = -2;
      //print("cgi was registered in epoll");
    }
  }
  else if (event.events & (EPOLLERR | EPOLLHUP)) {
    end_connection(fd, epoll_fd, client_infos);
  }
  else {
    if (send_response(fd, client_infos[fd].response) == UNFINISHED) {
      if (set_epoll_event(event, fd, epoll_fd, EPOLLOUT, EPOLL_CTL_MOD) == ERROR)
        return ;
    }
    else
      end_connection(fd, epoll_fd, client_infos);
  }
}

int manage_events(std::map<int, Server> &servers, Conf &conf_c)
{
  std::map<int, t_parse_data> client_infos;
  int epoll_fd = epoll_create1(0);
  if (epoll_fd == ERROR)
    return ERROR;
  struct epoll_event event;
  for (std::map<int, Server>::iterator it = servers.begin(); it != servers.end(); ++it)
  {
    int server_fd = it->first;
    set_event(&event, EPOLLIN, server_fd);
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) == ERROR)
      return close(epoll_fd), ERROR;
  }

  struct epoll_event events[MAX_EVENTS];
  while (gSignalStatus != 2)
  {
    std::cout << "\n--------------------EPOLL ITERATION--------------------\n\n";
    int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, 1000);
    std::cout << "nfds: "  << nfds << "\n";
    if (nfds == ERROR)
    {
      if (errno == EINTR) continue;
      perror("epoll_wait");
      return ERROR;
    }
    std::map<int, t_parse_data>::iterator it = client_infos.begin();
    while (it != client_infos.end())
    {
        if (it->second.request.isTimedOut(it->first))
        {
            int fd = it->first;
            ++it;
            if (client_infos[fd].cgi_fd == -2)
            {
              end_connection(fd, epoll_fd, client_infos);
              //end_connection(client_infos[fd].client_fd, epoll_fd, client_infos);
            }
            else {
              if (client_infos[fd].cgi_fd != 0)
                kill(client_infos[fd].cgi_pid, SIGKILL);
              int status_code;
              if (client_infos[fd].cgi_fd == -1)
                status_code = 408;
              else 
                status_code = 504;
              client_infos[fd].response = create_response(set_error_response(*client_infos[fd].server, status_code, std::string()));
              if (set_epoll_event(event, fd, epoll_fd, EPOLLOUT, EPOLL_CTL_MOD) == ERROR)
                continue ;
            }
        }
        else
        {
            ++it;
        }
    }
    for (int i = 0; i < nfds; i++)
      manage_requests(events[i], epoll_fd, conf_c, servers, client_infos);
  }

  for (std::map<int, Server>::iterator it = servers.begin(); it != servers.end(); ++it) {
    close(it->first);
  }
  close(epoll_fd);
  return SUCCESS;
}
