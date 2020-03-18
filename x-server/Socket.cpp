#include "Socket.hpp"
#include "env-settings.hpp"

extern int errno;

Socket::Socket(const IpAddr &ip_addr, int type, int protocol, int queue_len) {
  /* Create socket for listening (client requests) */
  sockfd = socket(ip_addr.addr_family(), type, protocol);

  if (sockfd < 0) {
#ifndef NDEBUG
    assert(errno != 0);
#endif
    cout << "ERR!  " << strerror(errno);
    exit(ERR_CODE_CREATE_SOCKET_ERROR);
  }

  /* Set socket to reuse address */
  uint8_t reuse = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) != 0) {
#ifndef NDEBUG
    assert(errno != 0);
#endif
    cout << "ERR!  " << strerror(errno);
    exit(ERR_CODE_SET_SOCK_OPT_REUSE_ERROR);
  }

  /* Bind address and socket together */
  SockAddrInfo info;
  auto[sock_addr_ptr, sock_addr_len] = ip_addr.sock_addr_info(info);
  if (bind(sockfd, sock_addr_ptr, sock_addr_len) != 0) {
#ifndef NDEBUG
    assert(errno != 0);
#endif
    cout << "ERR!  " << strerror(errno);
    exit(ERR_CODE_BIND_SOCK_ERROR);
  }

  /* Create listening queue (client requests) */
  if (listen(sockfd, queue_len) != 0) {
#ifndef NDEBUG
    assert(errno != 0);
#endif
    cout << "ERR!  " << strerror(errno);
    exit(ERR_CODE_LISTEN_SOCK_ERROR);
  }
}

Socket::~Socket() {
  close(sockfd);
}

void Socket::loop() {

}

[[maybe_unused]] TcpSocket::TcpSocket(IpAddr &ip_addr)
        : Socket(ip_addr, SOCK_STREAM, 0) {}

[[maybe_unused]] UdpSocket::UdpSocket(IpAddr &ip_addr)
        : Socket(ip_addr, SOCK_DGRAM, 0) {}

[[maybe_unused]] RawSocket::RawSocket(IpAddr &ip_addr, int protocol)
        : Socket(ip_addr, SOCK_RAW, protocol) {}
