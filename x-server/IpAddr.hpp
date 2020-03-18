#ifndef XSERVER_IPADDR_HPP
#define XSERVER_IPADDR_HPP

#include "env-settings.hpp"

class IpAddr {
public:
  const SockAddr sock_addr{};
  [[nodiscard]] SockAddrFamily addr_family() const;

protected:
  explicit IpAddr();
};

class [[maybe_unused]] IpAddrV4 : IpAddr {
public:
  [[maybe_unused]] explicit IpAddrV4(const char *ip_addr_v4, uint16_t port_num);
};

class [[maybe_unused]] IpAddrV6 : IpAddr {
public:
  [[maybe_unused]] explicit IpAddrV6(const char *ip_addr_v6, uint16_t port_num);
};

#endif // XSERVER_IPADDR_HPP
