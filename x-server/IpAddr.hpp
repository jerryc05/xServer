#ifndef XSERVER_IPADDR_HPP
#define XSERVER_IPADDR_HPP

#include "env-settings.hpp"
#include <cassert>
#include <cstring>

#ifdef unix

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

using SockAddrFamily = sa_family_t;
using SockAddr = sockaddr;
using SockAddrIn = sockaddr_in;
using SockAddrIn6 = sockaddr_in6;
using SockCallLen = socklen_t;
#endif

enum struct IpAddrType : char {
  IpAddrV4, IpAddrV6
};

class IpAddr {
public:
  const IpAddrType addr_type;

  [[nodiscard]] const SockAddrFamily &addr_family() const;

  [[nodiscard]] Pair<const SockAddr *, SockCallLen> sock_addr_info() const;

protected:
  explicit IpAddr(IpAddrType addr_type_);
};

class [[maybe_unused]] IpAddrV4 : public IpAddr {
public:
  const SockAddrIn sock_addr_in;

  [[maybe_unused]] explicit IpAddrV4(const char *ip_addr_v4, uint16_t port_num);
};

class [[maybe_unused]] IpAddrV6 : public IpAddr {
public:
  const SockAddrIn6 sock_addr_in6;

  [[maybe_unused]] explicit IpAddrV6(const char *ip_addr_v6, uint16_t port_num);
};

#endif // XSERVER_IPADDR_HPP
