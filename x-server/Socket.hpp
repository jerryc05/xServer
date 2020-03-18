#ifndef XSERVER_SOCKET_HPP
#define XSERVER_SOCKET_HPP

#include "IpAddr.hpp"

class Socket {
public:
  [[noreturn]] void loop();

protected:
  Socket(const IpAddr &ip_addr, int type, int protocol, int queue_len = 16);

  ~Socket();

private:
  int sockfd; // socket file descriptor
  const IpAddr &ip_addr_;
};

class TcpSocket : public Socket {
public:
  [[maybe_unused]] explicit TcpSocket(IpAddr &ip_addr);
};

class UdpSocket : public Socket {
public:
  [[maybe_unused]] explicit UdpSocket(IpAddr &ip_addr);
};

class RawSocket : public Socket {
public:
  [[maybe_unused]] RawSocket(IpAddr &ip_addr, int protocol);
};

#endif // XSERVER_SOCKET_HPP
