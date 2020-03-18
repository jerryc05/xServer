#include "Socket.hpp"
#include "env-settings.hpp"

extern int errno;

Socket::Socket(const IpAddr &ip_addr, int type, int protocol, int queue_len)
        : ip_addr_(ip_addr) {
  /* Create socket for listening (client requests) */
  sockfd = socket(ip_addr.addr_family(), type, protocol);

  if (sockfd < 0) {
#ifndef NDEBUG
    assert(errno != 0);
#endif
    cerr << "ERR!  " << strerror(errno);
    exit(ERR_CODE_CREATE_SOCKET_ERROR);
  }

  /* Set socket to reuse address */
  uint8_t reuse = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) != 0) {
#ifndef NDEBUG
    assert(errno != 0);
#endif
    cerr << "ERR!  " << strerror(errno);
    exit(ERR_CODE_SET_SOCK_OPT_REUSE_ERROR);
  }

  /* Bind address and socket together */
  SockAddrInfo info;
  auto[sock_addr_ptr, sock_addr_len] = ip_addr.sock_addr_info(info);
  if (bind(sockfd, sock_addr_ptr, sock_addr_len) != 0) {
#ifndef NDEBUG
    assert(errno != 0);
#endif
    cerr << "ERR!  " << strerror(errno);
    exit(ERR_CODE_BIND_SOCK_ERROR);
  }

  /* Create listening queue (client requests) */
  if (listen(sockfd, queue_len) != 0) {
#ifndef NDEBUG
    assert(errno != 0);
#endif
    cerr << "ERR!  " << strerror(errno);
    exit(ERR_CODE_LISTEN_SOCK_ERROR);
  }
}

Socket::~Socket() {
  close(sockfd);
}

[[noreturn]] void Socket::loop() {
  for (;;) {
    /* Do TCP handshake with client */
//    int client_sockfd;
//    switch (ip_addr)= accept4(sockfd,
//                              (struct sockaddr *) &client_addr,
//                              &client_addr_len,
//                              SOCK_NONBLOCK | SOCK_CLOEXEC);
//    if (client_sockfd == -1) {
//      perror("accept()");
//      close(listen_sock_fd);
//      return EXIT_FAILURE;
//    }
//
//  inet_ntop(AF_INET6, &(client_addr.sin6_addr),
//            str_addr, sizeof(str_addr));
//  printf("New connection from: %s:%d ...\n",
//         str_addr,
//         ntohs(client_addr.sin6_port));
//
//  /* Wait for data from client */
//  ret = read(client_sock_fd, &ch, 1);
//  if (ret == -1) {
//    perror("read()");
//    close(client_sock_fd);
//    continue;
//  }
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

[[maybe_unused]] TcpSocket::TcpSocket(IpAddr &ip_addr)
        : Socket(ip_addr, SOCK_STREAM, 0) {}

[[maybe_unused]] UdpSocket::UdpSocket(IpAddr &ip_addr)
        : Socket(ip_addr, SOCK_DGRAM, 0) {}

[[maybe_unused]] RawSocket::RawSocket(IpAddr &ip_addr, int protocol)
        : Socket(ip_addr, SOCK_RAW, protocol) {}
