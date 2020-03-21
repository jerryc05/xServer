#include "Epoll.hpp"

extern int errno;

Epoll::Epoll() : epfd(-1), sockfd(-1), epoll_arr() {}

[[maybe_unused]] Epoll::Epoll(int sockfd_, int cloexec_flag)
        : epfd(-1), sockfd(sockfd_), epoll_arr() {
  initialize(sockfd_, cloexec_flag);
}

void Epoll::initialize(int sockfd_, int cloexec_flag) {
  /* Create epoll file descriptor */ {
    epfd = epoll_create1(cloexec_flag);
    if (epfd < 0) {
#ifndef NDEBUG
      assert(errno != 0);
#endif
      log_e() << strerror(errno) << '\n';
      exit(ERR_CODE_EPOLL_CREATE1);
    }
    log_d() << "Epoll created w/ file descriptor: " << epfd << '\n';
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
  auto count = epoll_wait(
          epfd,
          epoll_arr,
          sizeof(epoll_arr) / sizeof(*epoll_arr),
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