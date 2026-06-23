#include "webserv.hpp"

#define MAX_EVENTS 64

void set_event(struct epoll_event *event, int flag, int fd)
{
  event->events = flag;
  event->data.fd = fd;
}

int  send_response(int client_fd, std::string *response)
{
  ssize_t bytes_sent;
  bytes_sent = send(client_fd, response->data(), response->size(), 0);
  if (bytes_sent == ERROR)
    print_error("Failed to send response");
  if (bytes_sent == (ssize_t)response->size())
    return DONE;
  response->erase(0, bytes_sent);
  return UNFINISHED;
}

static void manage_requests(struct epoll_event event, int server_fd, int epoll_fd)
{
  int fd = event.data.fd;
  std::string *response = NULL;

  if (fd == server_fd)
  {
    int client_fd = accept_client(server_fd);
    if (client_fd == ERROR)
      return ;
    set_nonblocking(client_fd);
    struct epoll_event s_event;
    set_event(&s_event, EPOLLIN, client_fd);
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &s_event) == ERROR)
      return print_error("epoll_ctl"), void();
  }
  else if (event.events & (EPOLLERR | EPOLLHUP))
  {
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
    close(fd);
  }
  else if (event.events & EPOLLIN)
  {
    if (handle_client(fd, response) == SUCCESS)
    {
      set_event(&event, EPOLLIN, fd);
      if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event) == ERROR)
        return print_error("epoll_ctl"), void();
    }
  }
  else
  {
    if (send_response(fd, response) == DONE)
    {
      set_event(&event, EPOLLIN, fd);
      if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event) == ERROR)
        return print_error("epoll_ctl"), void();
    }
  }
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
