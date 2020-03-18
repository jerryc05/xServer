#include "Epoll.hpp"
#include "env-settings.hpp"

extern int errno;

Epoll::Epoll(int cloexec_flag) {
  epfd = epoll_create1(cloexec_flag);

  if (epfd < 0) {
#ifndef NDEBUG
    assert(errno != 0);
#endif
    cerr << "ERR!  " << strerror(errno);
    exit(ERR_CODE_EPOLL_CREATE1_ERROR);
  }
}