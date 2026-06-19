#include "webserv.hpp"

#define MAX_EVENTS 64

static void set_event(struct epoll_event *event, int flag, int fd)
{
  event->events = flag;
  event->data.fd = fd;
}

const char* response =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Content-Length: 21\r\n"
    "Connection: close\r\n"
    "\r\n"
    "<h1>Hello World!</h1>";

static void manage_requests(struct epoll_event event, int server_fd, int epoll_fd)
{
  int fd = event.data.fd;

  if (fd == server_fd)
  {
    int client_fd = accept_client(server_fd);
    if (client_fd == ERROR)
      return ;
    set_nonblocking(client_fd);
    struct epoll_event s_event;
    set_event(&s_event, EPOLLIN, client_fd);
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &s_event);
  }
  else if (event.events & (EPOLLERR | EPOLLHUP))
  {
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
    close(fd);
  }
  else if (event.events & EPOLLIN)
    handle_client(fd);
  else
    send(fd, response, strlen(response), 0);
}

int manage_events(int server_fd)
{
  int epoll_fd = epoll_create1(0);
  if (epoll_fd == ERROR)
    return ERROR;
  struct epoll_event s_event;
  set_event(&s_event, EPOLLIN, server_fd);
  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &s_event) == ERROR)
    return close(epoll_fd), ERROR;

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
      manage_requests(events[i], server_fd, epoll_fd);
  }
  close(epoll_fd);
  return SUCCESS;
}
