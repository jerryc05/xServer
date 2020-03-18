#ifndef XSERVER_IPADDR_HPP
#define XSERVER_IPADDR_HPP

#include "env-settings.hpp"

class IpAddr {
public:
  [[nodiscard]] virtual SockAddrFamily addr_family() const =0;
  [[nodiscard]] virtual Tuple<const SockAddr *, SockAddrLen> sock_addr_info() const = 0;
  virtual ~IpAddr();

protected:
  explicit IpAddr();
};

class [[maybe_unused]] IpAddrV4 : IpAddr {
public:
  [[maybe_unused]] explicit IpAddrV4(const char *ip_addr_v4, uint16_t port_num);
  [[nodiscard]]  SockAddrFamily addr_family() const override ;
  [[nodiscard]] Tuple<const SockAddr *, SockAddrLen> sock_addr_info() const override;

private:
  const SockAddrIn sock_addr_in{};
};

class [[maybe_unused]] IpAddrV6 : IpAddr {
public:
  [[maybe_unused]] explicit IpAddrV6(const char *ip_addr_v6, uint16_t port_num);
  [[nodiscard]]  SockAddrFamily addr_family() const override ;
  [[nodiscard]] Tuple<const SockAddr *, SockAddrLen> sock_addr_info() const override;

private:
  const SockAddrIn6 sock_addr_in6{};
};

#endif // XSERVER_IPADDR_HPP
