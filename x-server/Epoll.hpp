#ifndef XSERVER_EPOLL_HPP
#define XSERVER_EPOLL_HPP

#ifdef unix

#include <sys/epoll.h>

using EpollEvent = epoll_event;

#endif

#include "env-settings.hpp"

constexpr int DEFAULT_CLOEXEC = EPOLL_CLOEXEC;

class Epoll {
public:
  explicit Epoll(int sockfd, int cloexec_flag = DEFAULT_CLOEXEC);
  ~Epoll();

  bool ctl_interface(int operations, int fd, EpollEvent* event_ptr) const;
  [[nodiscard]] Pair<uint, EpollEvent *> ready_count();

private:
  /* Epoll file descriptor */
  int        epfd;
  /* Socket file descriptor */
  int        sockfd;
  /* Size is configurable via macro MAX_EPOLL_EVENT_COUNT_OVERRIDE */
  EpollEvent epoll_arr[MAX_EPOLL_EVENT_COUNT];
};

#endif // XSERVER_EPOLL_HPP
