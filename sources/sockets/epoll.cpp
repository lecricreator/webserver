#include "webserv.hpp"

#define MAX_EVENTS 64

void set_event(struct epoll_event *event, int flag, int fd)
{
  event->events = flag;
  event->data.fd = fd;
}

int manage_events(const char *response, int server_fd)
{
  int epoll_fd = epoll_create1(0);
  struct epoll_event s_event;
  set_event(&s_event, EPOLLIN, server_fd);
  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &s_event);

  struct epoll_event events[MAX_EVENTS];
  while (true)
  {
    int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
    if (nfds == ERROR)
    {
      if (errno == EINTR) continue;
      perror("epoll_wait");
      break;
    }
    for (int i = 0; i < nfds; i++)
    {
      int fd = events[i].data.fd;
      //uint32_t event = events[i].events;
      
      if (fd == server_fd)
      {
        int client_fd = accept_client(server_fd);
        if (client_fd == ERROR)
          return ERROR;
        set_nonblocking(client_fd);
        set_event(&s_event, EPOLLIN, client_fd);
        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &s_event);
        continue;
      }
      if (events[i].events & (EPOLLERR | EPOLLHUP))
      {
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
        close(fd);
        continue;
      }
      if (events[i].events & EPOLLIN)
        handle_client(fd, response);
      else
        send(fd, response, strlen(response), 0);
    }
  }
  return SUCCESS;
}
