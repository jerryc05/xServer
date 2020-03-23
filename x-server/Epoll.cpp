#include "Epoll.hpp"
#include <cstring>

#ifndef NDEBUG

#include <cassert>

#endif

extern int errno;

Epoll::Epoll(int sockfd_, int cloexec_flag)
        : epfd(epoll_create1(cloexec_flag)), sockfd(sockfd_), epoll_arr() {
  /* Verify epoll file descriptor */ {
    if (epfd < 0) {
#ifndef NDEBUG
      assert(errno != 0);
#endif
      ErrLogger() << strerror(errno);
      throw RuntimeError(ERR_STR_EPOLL_CREATE1);
    }
#ifndef NDEBUG
    DbgLogger() << "Epoll file descriptor: " << epfd;
#endif
  }

  /* Verify socket file descriptor */ {
    if (sockfd < 0) {
#ifndef NDEBUG
      assert(errno != 0);
#endif
      ErrLogger() << "Epoll::Epoll() got invalid sockfd: " << sockfd
                  << "\n\t" << strerror(errno);
      throw RuntimeError(ERR_STR_EPOLL_GOT_INVALID_SOCKFD);
    }
#ifndef NDEBUG
    DbgLogger() << "Epoll's socket file descriptor: " << sockfd;
#endif
  }

  /* Register epoll event */ {
    EpollEvent event;
    event.events  = EPOLLIN;  // available for read()
    event.data.fd = sockfd;
    if (!ctl_interface(EPOLL_CTL_ADD, sockfd, &event)) {
#ifndef NDEBUG
      assert(errno != 0);
#endif
      ErrLogger() << strerror(errno);
      throw RuntimeError(ERR_STR_EPOLL_CTL_ADD);
    }
#ifndef NDEBUG
    DbgLogger() << "Registered epoll (fd: " << epfd
    << ") with socket (fd: " << sockfd << ')';
#endif
  }
}

Epoll::~Epoll() {
  if (!ctl_interface(EPOLL_CTL_DEL, sockfd, nullptr)) {
#ifndef NDEBUG
    assert(errno != 0);
#endif
    ErrLogger() << "Epoll::~Epoll():\n\t" << strerror(errno);
  }
#ifndef NDEBUG
  DbgLogger() << "Epoll (fd: " << epfd << ") deleted";
#endif
}

bool Epoll::ctl_interface(int operations, int fd, EpollEvent *event_ptr) const {
  return epoll_ctl(epfd, operations, fd, event_ptr) == 0;
}

Pair<uint, EpollEvent *> Epoll::ready_count() {
  static_assert(MAX_EPOLL_EVENT_COUNT == sizeof(epoll_arr) / sizeof(*epoll_arr),
                "Epoll::ready_count() epoll_arr size(s) does not match\n");
  auto count = epoll_wait(
          epfd,
          epoll_arr,
          MAX_EPOLL_EVENT_COUNT,
          -1);
  if (count < 0) {
#ifndef NDEBUG
    assert(errno != 0);
#endif
    ErrLogger() << strerror(errno);
    throw RuntimeError(ERR_STR_EPOLL_WAIT);
  }
  return {static_cast<uint>(count), epoll_arr};
}