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
  [[maybe_unused]] explicit Epoll(int sockfd, int cloexec_flag = DEFAULT_CLOEXEC);

//  /* Placeholder constructor, do not use it unless you have to */
//  Epoll();
//
//  /* Make this instance (created by the dummy constructor) usable */
//  void initialize(int sockfd, int cloexec_flag = DEFAULT_CLOEXEC);

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
