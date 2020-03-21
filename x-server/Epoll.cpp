#include "Epoll.hpp"
#include <cassert>
#include <cstring>

extern int errno;

[[maybe_unused]] Epoll::Epoll(int sockfd_, int cloexec_flag)
        : epfd(epoll_create1(cloexec_flag)), sockfd(sockfd_), epoll_arr() {
  /* Verify epoll file descriptor */ {
    if (epfd < 0) {
#ifndef NDEBUG
      assert(errno != 0);
#endif
      log_e() << strerror(errno) << '\n';
      exit(ERR_CODE_EPOLL_CREATE1);
    }
    log_d() << "Epoll created w/ file descriptor: " << epfd << '\n';
  }

  /* Verify socket file descriptor */ {
    if (sockfd < 0) {
#ifndef NDEBUG
      assert(errno != 0);
#endif
      log_e() << "Epoll::Epoll() got invalid sockfd: " << sockfd_ << "\n\t"
              << strerror(errno) << '\n';
      exit(ERR_CODE_EPOLL_GOT_INVALID_SOCKFD);
    }
    log_d() << "Epoll created w/ socket file descriptor: " << sockfd << '\n';
  }

  /* Register epoll event */ {
    EpollEvent event;
    event.events  = EPOLLIN;  // available for read()
    event.data.fd = sockfd_;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd_, &event) != 0) {
#ifndef NDEBUG
      assert(errno != 0);
#endif
      log_e() << strerror(errno) << '\n';
      exit(ERR_CODE_EPOLL_CTL_ADD);
    }
  }
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
    exit(ERR_CODE_EPOLL_WAIT);
  }
  return {static_cast<uint>(count), epoll_arr};
}