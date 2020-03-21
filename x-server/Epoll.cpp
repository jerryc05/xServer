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
      log_e() << strerror(errno) << '\n';
      throw RuntimeError(ERR_STR_EPOLL_CREATE1);
    }
#ifndef NDEBUG
    log_d() << "Epoll file descriptor: " << epfd << '\n';
#endif
  }

  /* Verify socket file descriptor */ {
    if (sockfd < 0) {
#ifndef NDEBUG
      assert(errno != 0);
#endif
      log_e() << "Epoll::Epoll() got invalid sockfd: " << sockfd << "\n\t"
              << strerror(errno) << '\n';
      throw RuntimeError(ERR_STR_EPOLL_GOT_INVALID_SOCKFD);
    }
#ifndef NDEBUG
    log_d() << "Epoll's socket file descriptor: " << sockfd << '\n';
#endif
  }

  /* Register epoll event */ {
    EpollEvent event;
    event.events  = EPOLLIN;  // available for read()
    event.data.fd = sockfd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &event) != 0) {
#ifndef NDEBUG
      assert(errno != 0);
#endif
      log_e() << strerror(errno) << '\n';
      throw RuntimeError(ERR_STR_EPOLL_CTL_ADD);
    }
#ifndef NDEBUG
    log_d() << "Registered epoll (fd: " << epfd
            << ") with socket (fd: " << sockfd << ")\n";
#endif
  }
}

Epoll::~Epoll() {
  if (epoll_ctl(epfd, EPOLL_CTL_DEL, sockfd, nullptr) != 0) {
#ifndef NDEBUG
    assert(errno != 0);
#endif
    log_e() << "Epoll::~Epoll():\n\t"
            << strerror(errno) << '\n';
  }
#ifndef NDEBUG
  log_d() << "Epoll (fd: " << epfd << ") deleted\n";
#endif
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
    log_e() << strerror(errno) << '\n';
    throw RuntimeError(ERR_STR_EPOLL_WAIT);
  }
  return {static_cast<uint>(count), epoll_arr};
}