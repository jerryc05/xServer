#ifndef XSERVER_SOCKET_HPP
#define XSERVER_SOCKET_HPP

#include "IpAddr.hpp"

class Socket {
protected:
  Socket(const IpAddr &ip_addr, int port_num, int type, int protocol);

private:
  int sockfd; // socket file descriptor
};

class TcpSocket : Socket {
public:
  TcpSocket(IpAddr &ip_addr, int port_num);
};

class UdpSocket : Socket {
public:
  UdpSocket(IpAddr &ip_addr, int port_num);
};

class RawSocket : Socket {
public:
  RawSocket(IpAddr &ip_addr, int port_num, int protocol);
};

#endif // XSERVER_SOCKET_HPP
