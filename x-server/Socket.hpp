#ifndef XSERVER_SOCKET_HPP
#define XSERVER_SOCKET_HPP

#include "IpAddr.hpp"

class Socket {
protected:
  Socket(const IpAddr &ip_addr, int type, int protocol);

private:
  int sockfd; // socket file descriptor
};

class TcpSocket : Socket {
public:
  [[maybe_unused]] explicit TcpSocket(IpAddr &ip_addr);
};

class UdpSocket : Socket {
public:
  [[maybe_unused]] explicit UdpSocket(IpAddr &ip_addr);
};

class RawSocket : Socket {
public:
  [[maybe_unused]] RawSocket(IpAddr &ip_addr, int protocol);
};

#endif // XSERVER_SOCKET_HPP
