#include "Socket.hpp"

using Byte = std::byte;

extern int errno;

Socket::Socket(const IpAddr &ip_addr_, int type, int protocol)
        : ip_addr(ip_addr_) {
  /* Create socket for listening (client requests) */  {
    sockfd = socket(ip_addr_.addr_family(), type, protocol);
    if (sockfd < 0) {
#ifndef NDEBUG
      assert(errno != 0);
#endif
      log_e() << "ERR!  " << strerror(errno) << '\n';
      exit(ERR_CODE_CREATE_SOCKET);
    }
    log_d() << "Socket created w/ file descriptor: " << sockfd << '\n';
  }

  /* Set socket to reuse address */  {
    SockCallLen reuse = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) != 0) {
#ifndef NDEBUG
      assert(errno != 0);
#endif
      log_e() << strerror(errno) << '\n';
      exit(ERR_CODE_SET_SOCK_OPT_REUSE);
    }
    log_d() << "Socket set to reuse address!\n";
  }

  /* Bind address and socket together */ {
    auto[sock_addr_ptr, sock_addr_len] = ip_addr_.sock_addr_info();
    if (bind(sockfd, sock_addr_ptr, sock_addr_len) != 0) {
#ifndef NDEBUG
      assert(errno != 0);
#endif
      log_e() << strerror(errno) << '\n';
      exit(ERR_CODE_BIND_SOCK);
    }
    log_i() << "Socket bound to address: ";
#ifndef NDEBUG
    switch (ip_addr_.addr_type) {
      case IpAddrType::IpAddrV4: {
        char       client_addr_str[INET_ADDRSTRLEN];
        const char *addr = inet_ntop(
                AF_INET,
                &(reinterpret_cast<const IpAddrV4 *>(&ip_addr_)->sock_addr_in.sin_addr),
                client_addr_str, sizeof(client_addr_str));
        if (addr == nullptr) {
          assert(errno != 0);
          log_e() << strerror(errno) << '\n';
          exit(ERR_CODE_REACH_END_OF_NON_VOID_FUNC);
        }
        cout << addr;
        break;
      }
      case IpAddrType::IpAddrV6: {
        char       client_addr_str[INET6_ADDRSTRLEN];
        const char *addr = inet_ntop(
                AF_INET6,
                &(reinterpret_cast<const IpAddrV6 *>(
                        &ip_addr_)->sock_addr_in6.sin6_addr),
                client_addr_str,
                sizeof(client_addr_str));
        if (addr == nullptr) {
          assert(errno != 0);
          log_e() << strerror(errno) << '\n';
          exit(ERR_CODE_INET_NTOP);
        }
        cout << addr;
        break;
      }
    }
    cout << '\n';
#else
    cout << "<omitted>\n";
#endif
  }

  /* Create listening queue (client requests) */ {
    // LISTEN_BACKLOG is configurable via macro LISTEN_BACKLOG_OVERRIDE
    if (listen(sockfd, LISTEN_BACKLOG) != 0) {
#ifndef NDEBUG
      assert(errno != 0);
#endif
      log_e() << strerror(errno) << '\n';
      exit(ERR_CODE_LISTEN_SOCK);
    }
  }

  /* Initialize epoll event */ {
    epoll = Epoll(sockfd);
  }
}

Socket::~Socket() {
  close(sockfd);
}

[[noreturn]] void Socket::loop() {
#ifndef NDEBUG
  log_d() << "Socket::loop() Beep!\n";
#endif

  auto[ready_count, event_array_ptr] = epoll.ready_count();

  switch (ip_addr.addr_type) {
    case IpAddrType::IpAddrV4: {
      for (;;) {
        SockAddrIn client_addr;
        int        client_sockfd;

        /* Do TCP handshake with client */ {
          SockCallLen client_addr_len = sizeof(client_addr);

          client_sockfd = accept4(sockfd, reinterpret_cast<SockAddr *>(&client_addr),
                                  &client_addr_len, SOCK_NONBLOCK | SOCK_CLOEXEC);

          if (client_sockfd < 0) {
#ifndef NDEBUG
            assert(errno != 0);
#endif
            log_e() << "Socket::loop() >> IpAddrV4 >> accept4():\n\t"
                    << strerror(errno) << '\n';
            continue;
          }
        }

        /* Display IP Address */ {
          char client_addr_str[INET_ADDRSTRLEN];
          inet_ntop(AF_INET, &(client_addr.sin_addr),
                    client_addr_str, sizeof(client_addr_str));
          log_i() << "New connection from: "
                  << client_addr_str << ':' << ntohs(client_addr.sin_port) << '\n';
        }

        /* Wait for data from client */ {
          Byte r_buf[512];
          if (recv(client_sockfd, &r_buf, sizeof(r_buf),
                   MSG_DONTWAIT) != 0) {
#ifndef NDEBUG
            assert(errno != 0);
#endif
            log_e() << "Socket::loop() >> IpAddrV4 >> recv():\n\t"
                    << strerror(errno) << '\n';
            close(client_sockfd);
            continue;
          }
        }

        /* Send response to client */ {
          char w_buf[]{"Hello, World!"};
          if (send(client_sockfd, &w_buf, sizeof(w_buf), MSG_DONTWAIT) != 0) {
#ifndef NDEBUG
            assert(errno != 0);
#endif
            log_e() << "Socket::loop() >> IpAddrV4 >> send():\n\t"
                    << strerror(errno) << '\n';
            close(client_sockfd);
            continue;
          }
        }

        /* TCP teardown */ {
//          auto client_sockfd_copy = client_sockfd;
//          client_sockfd = -1;
          if (close(client_sockfd) != 0) {
#ifndef NDEBUG
            assert(errno != 0);
#endif
            log_e() << "Socket::loop() >> IpAddrV4 >> close():\n\t"
                    << strerror(errno) << '\n';
            continue;
          }
        }

        cout << ("Connection closed\n");
      }
    }

    case IpAddrType::IpAddrV6: {
      for (;;) {}
    }
  }
  log_e() << "Socket::loop():\n\t" << ERR_STR_REACH_END_OF_NON_VOID_FUNC;
  exit(ERR_CODE_REACH_END_OF_NON_VOID_FUNC);
}


[[maybe_unused]] TcpSocket::TcpSocket(IpAddr &ip_addr_)
        : Socket(ip_addr_, SOCK_STREAM, 0) {}

[[maybe_unused]] UdpSocket::UdpSocket(IpAddr &ip_addr_)
        : Socket(ip_addr_, SOCK_DGRAM, 0) {}

[[maybe_unused]] RawSocket::RawSocket(IpAddr &ip_addr_, int protocol)
        : Socket(ip_addr_, SOCK_RAW, protocol) {}
