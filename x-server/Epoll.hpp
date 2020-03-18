#ifndef XSERVER_EPOLL_HPP
#define XSERVER_EPOLL_HPP

#include "env-settings.hpp"

class Epoll {
public:
  explicit Epoll(int cloexec_flag = EPOLL_CLOEXEC);

private:
  int epfd; // epoll file descriptor
};

#endif // XSERVER_EPOLL_HPP
