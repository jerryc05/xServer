#ifndef XSERVER_EPOLL_HPP
#define XSERVER_EPOLL_HPP

//#include "env-settings.hpp"
#include <cassert>
#include <cstring>

#ifdef unix

#include <sys/epoll.h>

#endif

class Epoll {
public:
  explicit Epoll(int cloexec_flag = EPOLL_CLOEXEC);

private:
  int epfd; // epoll file descriptor
};

#endif // XSERVER_EPOLL_HPP
