#include "webserv.hpp"

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
    return print_error("epoll_ctl"), ERROR;
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

static void manage_requests(struct epoll_event event,
            int epoll_fd, Conf &conf_c, std::map<int, Server> &servers,
            std::map<int, t_parse_data> &client_infos)
{
  int fd = event.data.fd;

  if (servers.find(fd) != servers.end())
  {
    int client_fd = accept_client(fd);
    if (client_fd == ERROR)
      return ;
    set_nonblocking(client_fd);
    struct epoll_event s_event;
    if (set_epoll_event(s_event, client_fd, epoll_fd, EPOLLIN, EPOLL_CTL_ADD) == ERROR)
      return ;
    client_infos.insert(std::pair<int, t_parse_data>(client_fd, create_parse_data(conf_c, servers[fd])));
  }
  else if (event.events & (EPOLLERR | EPOLLHUP))
    end_connection(fd, epoll_fd, client_infos);
  else if (event.events & EPOLLIN)
  {
    if (handle_request(fd, client_infos[fd]) == SUCCESS)
      if (set_epoll_event(event, fd, epoll_fd, EPOLLOUT, EPOLL_CTL_MOD) == ERROR)
        return ;
  }
  else
  {
    if (send_response(fd, client_infos[fd].response) == UNFINISHED)
    {
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
  struct epoll_event s_event;
  for (std::map<int, Server>::iterator it = servers.begin(); it != servers.end(); ++it)
  {
    int server_fd = it->first;
    set_event(&s_event, EPOLLIN, server_fd);
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &s_event) == ERROR)
      return close(epoll_fd), ERROR;
  }

  struct epoll_event events[MAX_EVENTS];
  while (true)
  {
    int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
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
        ++it;                          // advance before erasing
        end_connection(fd, epoll_fd, client_infos);
    }
    else
    {
        ++it;
    }
}

    for (int i = 0; i < nfds; i++)
      manage_requests(events[i], epoll_fd, conf_c, servers, client_infos);
  }
  close(epoll_fd);
  return SUCCESS;
}
