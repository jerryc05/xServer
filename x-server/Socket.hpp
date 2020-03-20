#ifndef XSERVER_SOCKET_HPP
#define XSERVER_SOCKET_HPP

#include "IpAddr.hpp"
#include "Epoll.hpp"

class Socket {
public:
  [[noreturn]] void loop();

protected:
  /* Number of sockets to listen (aka backlog) is configurable via macro LISTEN_BACKLOG */
  Socket(const IpAddr &ip_addr_, int type, int protocol);

  ~Socket();

private:
  const IpAddr ip_addr;
  Epoll  epoll;
  /* socket file descriptor */
  int          sockfd;
};

class [[maybe_unused]] TcpSocket : public Socket {
public:
  [[maybe_unused]] explicit TcpSocket(IpAddr &ip_addr_);
};

class [[maybe_unused]] UdpSocket : public Socket {
public:
  [[maybe_unused]] explicit UdpSocket(IpAddr &ip_addr_);
};

class [[maybe_unused]] RawSocket : public Socket {
public:
  [[maybe_unused]] RawSocket(IpAddr &ip_addr_, int protocol);
};

#endif // XSERVER_SOCKET_HPP
