#include "Socket.hpp"
#include "env-settings.hpp"

extern int errno;

Socket::Socket(const IpAddr &ip_addr, int type, int protocol) {
  sockfd = socket(ip_addr.addr_family(), type, protocol);

  if (sockfd < 0) {
#ifndef NDEBUG
    assert(errno != 0);
#endif
    cout << "ERR!  " << strerror(errno);
    exit(ERR_CODE_CREATE_SOCKET_ERROR);
  }


  /*












  */

//  int reuse = 1;
//  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) ==
//      -1) {
//    err_exit("setsockopt");
//  }
//  if (bind(sockfd, (sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
//    err_exit("bind");
//  }
//  if (listen(sockfd, 5) == -1) {
//    err_exit("listen");
//  }
  //  return sockfd;
}

[[maybe_unused]] TcpSocket::TcpSocket(IpAddr &ip_addr)
    : Socket(ip_addr, SOCK_STREAM, 0) {}

[[maybe_unused]] UdpSocket::UdpSocket(IpAddr &ip_addr)
    : Socket(ip_addr, SOCK_DGRAM, 0) {}

[[maybe_unused]] RawSocket::RawSocket(IpAddr &ip_addr, int protocol)
    : Socket(ip_addr, SOCK_RAW, protocol) {}
