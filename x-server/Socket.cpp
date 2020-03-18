#include "Socket.hpp"

using Byte = std::byte;

extern int errno;

Socket::Socket(const IpAddr &ip_addr_, int type, int protocol, int queue_len)
        : ip_addr(ip_addr_) {
  /* Create socket for listening (client requests) */  {
    sockfd = socket(ip_addr_.addr_family(), type, protocol);
    if (sockfd < 0) {
#ifndef NDEBUG
      assert(errno != 0);
#endif
      log_e() << "ERR!  " << strerror(errno);
      exit(ERR_CODE_CREATE_SOCKET_ERROR);
    }
    log_d() << "Socket created w/ file descriptor: " << sockfd << '\n';
  }

  /* Set socket to reuse address */  {
    SockCallLen reuse = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) != 0) {
#ifndef NDEBUG
      assert(errno != 0);
#endif
      log_e() << strerror(errno);
      exit(ERR_CODE_SET_SOCK_OPT_REUSE_ERROR);
    }
    log_d() << "Socket set to reuse address!\n";
  }

  /* Bind address and socket together */ {
    SockAddrInfo info;
    auto[sock_addr_ptr, sock_addr_len] = ip_addr_.sock_addr_info(info);
    if (bind(sockfd, sock_addr_ptr, sock_addr_len) != 0) {
#ifndef NDEBUG
      assert(errno != 0);
#endif
      log_e() << strerror(errno);
      exit(ERR_CODE_BIND_SOCK_ERROR);
    }
    log_i() << "Socket bound to address: ";
#ifndef NDEBUG
    switch (ip_addr_.addr_type) {
      case IpAddrType::IpAddrV4: {
        char       client_addr_str[INET_ADDRSTRLEN];
        const char *addr = inet_ntop(
                AF_INET,
                &(reinterpret_cast<const IpAddrV4 *>(
                        &ip_addr_)->sock_addr_in.sin_addr),
                client_addr_str,
                sizeof(client_addr_str));
        if (addr == nullptr) {
          assert(errno != 0);
          log_e() << strerror(errno);
          exit(ERR_CODE_INET_NTOP_ERROR);
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
          log_e() << strerror(errno);
          exit(ERR_CODE_INET_NTOP_ERROR);
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

  /* Create listening queue (client requests) */{
    if (listen(sockfd, queue_len) != 0) {
#ifndef NDEBUG
      assert(errno != 0);
#endif
      log_e() << strerror(errno);
      exit(ERR_CODE_LISTEN_SOCK_ERROR);
    }
  }
}

Socket::~Socket() {
  close(sockfd);
}

[[noreturn]] void Socket::loop() const {
#ifndef NDEBUG
  log_d() << "Socket::loop() Beep!\n";
#endif

  switch (ip_addr.addr_type) {
    case IpAddrType::IpAddrV4: {
      for (;;) {
        SockAddrIn clientAddr;
        int        clientSockfd;

        /* Do TCP handshake with client */ {
          SockCallLen clientAddrLen = sizeof(clientAddr);

          clientSockfd = accept4(sockfd, reinterpret_cast<SockAddr *>(&clientAddr),
                                 &clientAddrLen, SOCK_NONBLOCK | SOCK_CLOEXEC);

          if (clientSockfd != 0) {
#ifndef NDEBUG
            assert(errno != 0);
#endif
            log_e() << "Socket::loop() >> IpAddrV4 >> accept4():\n"
                    << strerror(errno);
            continue;
          }
        }

        /* Display IP Address */ {
          char clientAddrStr[INET_ADDRSTRLEN];
          inet_ntop(AF_INET, &(clientAddr.sin_addr),
                    clientAddrStr, sizeof(clientAddrStr));
          log_i() << "New connection from: "
                  << clientAddrStr << ':' << ntohs(clientAddr.sin_port) << '\n';
        }

        /* Wait for data from client */
        Byte buffer[512];
        if (recv(clientSockfd, &buffer, sizeof(buffer),
                 MSG_DONTWAIT) != 0) {
#ifndef NDEBUG
          assert(errno != 0);
#endif
          log_e() << "Socket::loop() >> IpAddrV4 >> recv():\n"
                  << strerror(errno);
          close(clientSockfd);
          continue;
        }
//
//  /* Do very useful thing with received data :-) */
//  ch++;
//
//  /* Send response to client */
//  ret = write(client_sock_fd, &ch, 1);
//  if (ret == -1) {
//    perror("write()");
//    close(client_sock_fd);
//    continue;
//  }
//
//  /* Do TCP teardown */
//  ret = close(client_sock_fd);
//  if (ret == -1) {
//    perror("close()");
//    client_sock_fd = -1;
//  }
//
//  printf("Connection closed\n");
      }
    }

    case IpAddrType::IpAddrV6: {
    }
  }
}


[[maybe_unused]] TcpSocket::TcpSocket(IpAddr &ip_addr)
        : Socket(ip_addr, SOCK_STREAM, 0) {}

[[maybe_unused]] UdpSocket::UdpSocket(IpAddr &ip_addr)
        : Socket(ip_addr, SOCK_DGRAM, 0) {}

[[maybe_unused]] RawSocket::RawSocket(IpAddr &ip_addr, int protocol)
        : Socket(ip_addr, SOCK_RAW, protocol) {}
