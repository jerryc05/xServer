#include "Socket.hpp"
#include <vector>

template<typename T> using Vec = std::vector<T>;

extern int errno;

constexpr uint16_t BUFFER_CHUNK_SIZE = 128;

Socket::Socket(const IpAddr &ip_addr_, int type, int protocol)
        : ip_addr(ip_addr_),
          sockfd(socket(ip_addr_.addr_family(), type, protocol)),
          epoll(sockfd) {
  /* Verify socket file descriptor (for client requests) */  {
    if (sockfd < 0) {
#ifndef NDEBUG
      assert(errno != 0);
#endif
      ErrLogger() << "ERR!  " << strerror(errno);
      throw RuntimeError(ERR_STR_CREATE_SOCKET);
    }
#ifndef NDEBUG
    DbgLogger() << "Socket file descriptor: " << sockfd;
#endif
  }

  /* Set socket to reuse address */  {
    SockCallLen reuse = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) != 0) {
#ifndef NDEBUG
      assert(errno != 0);
#endif
      ErrLogger() << strerror(errno);
      throw RuntimeError(ERR_STR_SET_SOCK_OPT_REUSE);
    }
#ifndef NDEBUG
    DbgLogger() << "Socket set to reuse address!";
#endif
  }

  /* Bind address and socket together */ {
    auto[sock_addr_ptr, sock_addr_len] = ip_addr_.sock_addr_info();
    if (bind(sockfd, sock_addr_ptr, sock_addr_len) != 0) {
#ifndef NDEBUG
      assert(errno != 0);
#endif
      ErrLogger() << strerror(errno);
      throw RuntimeError(ERR_STR_BIND_SOCK);
    }

    constexpr auto STR_SOCKET_BOUND_TO_ADDR = "Socket bound to address: ";
    switch (ip_addr.addr_type) {
      case IpAddrType::IpAddrV4: {
        auto ip_addr_v4_ptr = reinterpret_cast<const IpAddrV4 *>(&ip_addr_);
        char client_addr6_str[INET_ADDRSTRLEN];
        auto addr           = inet_ntop(AF_INET,
                                        &(ip_addr_v4_ptr->sock_addr_in.sin_addr),
                                        client_addr6_str, sizeof(client_addr6_str));
        if (addr == nullptr) {
          assert(errno != 0);
          ErrLogger() << strerror(errno);
          throw RuntimeError(ERR_STR_REACH_END_OF_NON_VOID_FUNC);
        }
        InfoLogger() << STR_SOCKET_BOUND_TO_ADDR
                    << addr << ':' << ntohs(ip_addr_v4_ptr->sock_addr_in.sin_port);
        break;
      }
      case IpAddrType::IpAddrV6: {
        auto ip_addr_v6_ptr = reinterpret_cast<const IpAddrV6 *>(&ip_addr_);
        char client_addr6_str[INET6_ADDRSTRLEN];
        auto addr           = inet_ntop(AF_INET6,
                                        &(ip_addr_v6_ptr->sock_addr_in6.sin6_addr),
                                        client_addr6_str, sizeof(client_addr6_str));
        if (addr == nullptr) {
          assert(errno != 0);
          ErrLogger() << strerror(errno);
          throw RuntimeError(ERR_STR_INET_NTOP);
        }
        InfoLogger() << STR_SOCKET_BOUND_TO_ADDR
                     << addr << ':' << ntohs(ip_addr_v6_ptr->sock_addr_in6.sin6_port);
        break;
      }
    }
    cout.flush();
  }

  /* Create listening queue (client requests) */ {
    // LISTEN_BACKLOG is configurable via macro LISTEN_BACKLOG_OVERRIDE
    if (listen(sockfd, LISTEN_BACKLOG) != 0) {
#ifndef NDEBUG
      assert(errno != 0);
#endif
      ErrLogger() << strerror(errno);
      throw RuntimeError(ERR_STR_LISTEN_SOCK);
    }
#ifndef NDEBUG
    DbgLogger() << "Socket listening with backlog: " << +LISTEN_BACKLOG;
#endif
  }
}

Socket::~Socket() {
  close(sockfd);
#ifndef NDEBUG
  DbgLogger() << "Socket (fd: " << sockfd << ") closed";
#endif
}

[[noreturn]] void Socket::loop() {
#ifndef NDEBUG
  DbgLogger() << "Socket::loop() Beep!";
#endif

  for (;;) {
    auto[ready_count, events] = epoll.ready_count();
#ifndef NDEBUG
    DbgLogger() << "Queried ready epolls: " << ready_count;
#endif
    if (ready_count <= 0)
      continue;

    switch (ip_addr.addr_type) {
      case IpAddrType::IpAddrV4: {
        for (decltype(ready_count) i = 0; i < ready_count; ++i) {
          if (events[i].data.fd == sockfd) {
            SockAddrIn client_addr;
            int        client_sockfd;

            /* TCP handshake with client */ {
              SockCallLen client_addr_len = sizeof(client_addr);

              client_sockfd = accept4(sockfd, reinterpret_cast<SockAddr *>(&client_addr),
                                      &client_addr_len, SOCK_NONBLOCK | SOCK_CLOEXEC);

              if (client_sockfd < 0) {
#ifndef NDEBUG
                assert(errno != 0);
#endif
                ErrLogger() << "Socket::loop() >> IpAddrV4 >> accept4(): "
                            << strerror(errno);
                continue;
              }
            }

            /* Display IP Address */ {
              char client_addr_str[INET_ADDRSTRLEN];
              inet_ntop(AF_INET, &(client_addr.sin_addr),
                        client_addr_str, sizeof(client_addr_str));
              InfoLogger() << "New connection from: "
                           << client_addr_str << ':' << ntohs(client_addr.sin_port);
            }

            /* Re-register client's sockfd */ {
              events[i].data.fd = client_sockfd;
              events[i].events  = EPOLLIN | EPOLLET;
              epoll.ctl_interface(EPOLL_CTL_ADD, client_sockfd, &events[i]);
            }

          } else {
            if (events[i].events & EPOLLIN) {
              auto        client_sockfd = events[i].data.fd;
              Vec<u_char> r_buf;
              uintmax_t   r_buf_size    = 0;

              /* Wait for data from client */ {
                for (;;) {
                  r_buf.reserve(r_buf_size + BUFFER_CHUNK_SIZE);
                  auto max_recv_size = r_buf.capacity() - r_buf_size - 1;

                  auto n = recv(client_sockfd, &r_buf[r_buf_size],
                                max_recv_size, MSG_DONTWAIT);

                  if (n < 0) {
#ifndef NDEBUG
                    assert(errno != 0);
#endif
                    ErrLogger() << "Socket::loop() >> IpAddrV4 >> recv(): "
                                << strerror(errno);
                    close(client_sockfd);
                    continue;
                  }

                  r_buf_size += static_cast<decltype(r_buf_size)>(n);
                  if (static_cast<decltype(max_recv_size)>(n) == max_recv_size)
                    continue;

                  r_buf[r_buf_size] = 0;
                  cout << r_buf.data();
                  cout.flush();
                  r_buf.pop_back();
                  break;
                }
              }

              /* Re-register client's sockfd */ {
                events[i].data.fd = client_sockfd;
                events[i].events  = EPOLLIN | EPOLLOUT | EPOLLET;
                epoll.ctl_interface(EPOLL_CTL_MOD, client_sockfd, &events[i]);
              }
            }
            if (events[i].events & EPOLLOUT) {
              /* Send response to client */ {
                int  client_sockfd = events[i].data.fd;
                char w_buf[]{
                        "HTTP/1.1 200 OK\r\n"
                        "Content-Type: text/html\r\n"
                        "\r\n"
                        "Hello, World!"};

                if (send(client_sockfd, &w_buf, sizeof(w_buf), MSG_DONTWAIT) < 0) {
#ifndef NDEBUG
                  assert(errno != 0);
#endif
                  ErrLogger() << "Socket::loop() >> IpAddrV4 >> send(): "
                              << strerror(errno);
                  close(client_sockfd);
                  continue;
                }
              }

              /* TCP teardown */ {
                int client_sockfd = events[i].data.fd;

                if (close(client_sockfd) != 0) {
#ifndef NDEBUG
                  assert(errno != 0);
#endif
                  ErrLogger() << "Socket::loop() >> IpAddrV4 >> close(): "
                              << strerror(errno);
                  continue;
                }
              }
            }
          }
        }
        break;
      }

      case IpAddrType::IpAddrV6: {
        ErrLogger() << "Not implemented";
        throw RuntimeError("Not implemented");
      }
    }
  }
}

[[maybe_unused]] TcpSocket::TcpSocket(IpAddr &ip_addr_)
        : Socket(ip_addr_, SOCK_STREAM, 0) {}

[[maybe_unused]] UdpSocket::UdpSocket(IpAddr &ip_addr_)
        : Socket(ip_addr_, SOCK_DGRAM, 0) {}

[[maybe_unused]] RawSocket::RawSocket(IpAddr &ip_addr_, int protocol)
        : Socket(ip_addr_, SOCK_RAW, protocol) {}
