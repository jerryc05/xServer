#ifndef XSERVER_IPADDR_HPP
#define XSERVER_IPADDR_HPP

class IpAddr {
public:
  const char *ip_addr;
  const int port_num;
  const int domain;

protected:
  explicit IpAddr(const char *ip_addr, int port_num, int domain);
};

class [[maybe_unused]] IpAddrV4 : IpAddr {
public:
  [[maybe_unused]] explicit IpAddrV4(const char *ip_addr_v4, int port_num);
};

class [[maybe_unused]] IpAddrV6 : IpAddr {
public:
  [[maybe_unused]] explicit IpAddrV6(const char *ip_addr_v6, int port_num);
};

#endif // XSERVER_IPADDR_HPP
