#include "webserv.hpp"

#define MAX_EVENTS 64

void set_event(struct epoll_event *event, int flag, int fd)
{
  event->events = flag;
  event->data.fd = fd;
}

static void manage_requests(struct epoll_event event, std::vector<int> server_fds, int epoll_fd, std::map<int, std::string> &responses)
{
  int fd = event.data.fd;

  if (std::find(server_fds.begin(), server_fds.end(), fd) != server_fds.end())
  {
    int client_fd = accept_client(fd);
    if (client_fd == ERROR)
      return ;
    set_nonblocking(client_fd);
    struct epoll_event s_event;
    set_event(&s_event, EPOLLIN, client_fd);
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &s_event) == ERROR)
      return print_error("epoll_ctl"), void();
    responses[fd] = "";
  }
  else if (event.events & (EPOLLERR | EPOLLHUP))
  {
    print("Connection ceased with fd " + to_str(fd));
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
    close(fd);
  }
  else if (event.events & EPOLLIN)
  {
    if (get_request(fd, responses[fd]) == SUCCESS)
    {
      set_event(&event, EPOLLOUT, fd);
      if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &event) == ERROR)
        return print_error("epoll_ctl"), void();
    }
  }
  else
  {
    if (send_response(fd, responses[fd]) == DONE)
    {
      set_event(&event, EPOLLOUT, fd);
      if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &event) == ERROR)
        return print_error("epoll_ctl"), void();
    }
  }
}

int manage_events(std::vector<int> server_fds)
{
  int epoll_fd = epoll_create1(0);
  if (epoll_fd == ERROR)
    return ERROR;
  struct epoll_event s_event;
  size_t nbr_of_fds = (size_t)server_fds.size();
  for (size_t server_index = 0; server_index < nbr_of_fds; server_index++)
  {
    int server_fd = server_fds[server_index];
    set_event(&s_event, EPOLLIN, server_fd);
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &s_event) == ERROR)
      return close(epoll_fd), ERROR;
  }

  struct epoll_event events[MAX_EVENTS];
  std::map<int, std::string> response;
  while (true)
  {
    int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
    if (nfds == ERROR)
    {
      if (errno == EINTR) continue;
      perror("epoll_wait");
      return ERROR;
    }
    for (int i = 0; i < nfds; i++)
      manage_requests(events[i], server_fds, epoll_fd, response);
  }
  close(epoll_fd);
  return SUCCESS;
}
