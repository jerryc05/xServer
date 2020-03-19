#ifndef XSERVER_EPOLL_HPP
#define XSERVER_EPOLL_HPP

#ifdef unix

#include <sys/epoll.h>

using EpollEvent = epoll_event;

#endif

#include "env-settings.hpp"
#include <cassert>
#include <cstring>

class Epoll {
public:
  explicit Epoll(int sockfd, int cloexec_flag = EPOLL_CLOEXEC);

  [[nodiscard]] Tuple<uint, EpollEvent *> ready_count();

private:
  /* Epoll file descriptor */
  int        epfd;
  /* Socket file descriptor */
  const int  sockfd;
  /* Size is configurable via macro MAX_EPOLL_EVENT_COUNT */
  EpollEvent epoll_arr[MAX_EPOLL_EVENT_COUNT]{};
};

#endif // XSERVER_EPOLL_HPP
