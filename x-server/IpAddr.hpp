#ifndef XSERVER_IPADDR_HPP
#define XSERVER_IPADDR_HPP

#include "env-settings.hpp"
#include <cassert>
#include <cstring>
#include <tuple>

using std::get;
template<typename T1, typename T2> using Tuple = std::tuple<T1, T2>;

#ifdef unix

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

using SockAddrFamily = sa_family_t;
using SockAddr = sockaddr;
using SockAddrIn = sockaddr_in;
using SockAddrIn6 = sockaddr_in6;
using SockAddrLen = socklen_t;
using SockAddrInfo = Tuple<const SockAddr *, SockAddrLen>;
#endif

class IpAddr {
public:
  [[nodiscard]] inline virtual SockAddrFamily addr_family() const = 0;

  [[nodiscard]] inline virtual SockAddrInfo &sock_addr_info(SockAddrInfo &info) const = 0;

  virtual ~IpAddr();

protected:
  explicit IpAddr();
};

class [[maybe_unused]] IpAddrV4 : IpAddr {
public:
  [[maybe_unused]] explicit IpAddrV4(const char *ip_addr_v4, uint16_t port_num);

  [[nodiscard]] inline SockAddrFamily addr_family() const override;

  [[nodiscard]] inline SockAddrInfo &sock_addr_info(SockAddrInfo &info) const override;

private:
  const SockAddrIn sock_addr_in{};
};

class [[maybe_unused]] IpAddrV6 : IpAddr {
public:
  [[maybe_unused]] explicit IpAddrV6(const char *ip_addr_v6, uint16_t port_num);

  [[nodiscard]] inline SockAddrFamily addr_family() const override;

  [[nodiscard]] inline SockAddrInfo &sock_addr_info(SockAddrInfo &info) const override;

private:
  const SockAddrIn6 sock_addr_in6{};
};

#endif // XSERVER_IPADDR_HPP
