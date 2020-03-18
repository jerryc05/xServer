#include "IpAddr.hpp"

IpAddr::IpAddr(IpAddrType addr_type) : addr_type_(addr_type) {}

SockAddrFamily IpAddr::addr_family() const {
  switch (addr_type_) {
    case IpAddrType::IpAddrV4:
      return reinterpret_cast<const IpAddrV4 *>(this)->sock_addr_in.sin_family;
    case IpAddrType::IpAddrV6:
      return reinterpret_cast<const IpAddrV6 *>(this)->sock_addr_in6.sin6_family;
  }
}

SockAddrInfo &IpAddr::sock_addr_info(SockAddrInfo &info) const {
  switch (addr_type_) {
    case IpAddrType::IpAddrV4: {
      auto sock_addr = *reinterpret_cast<const IpAddrV4 *>(this);
      get<0>(info) = reinterpret_cast<const SockAddr *>(&sock_addr.sock_addr_in);
      get<1>(info) = static_cast<SockAddrLen >(sizeof(sock_addr.sock_addr_in));
      break;
    }

    case IpAddrType::IpAddrV6: {
      auto sock_addr = *reinterpret_cast<const IpAddrV6 *>(this);
      get<0>(info) = reinterpret_cast<const SockAddr *>(&sock_addr.sock_addr_in6);
      get<1>(info) = static_cast<SockAddrLen >(sizeof(sock_addr.sock_addr_in6));
      break;
    }
  }
  return info;
}

[[maybe_unused]] IpAddrV4::IpAddrV4(const char *ip_addr_v4, uint16_t port_num)
        : IpAddr(IpAddrType::IpAddrV4) {
  auto sock_addr_in_ = *const_cast<SockAddrIn *>(&sock_addr_in);

  sock_addr_in_.sin_family = AF_INET;
  // host to network byte order as unsigned short
  sock_addr_in_.sin_port   = htons(port_num);

  // address to network byte order
  if (inet_aton(ip_addr_v4, &(sock_addr_in_.sin_addr)) == 0) {
    cerr << "ERR!  inet_aton() parsing address to network byte order failed!";
    exit(ERR_CODE_INET_ATON_FAILED);
  }
}

[[maybe_unused]] IpAddrV6::IpAddrV6(const char *ip_addr_v6, uint16_t port_num)
        : IpAddr(IpAddrType::IpAddrV6) {
  auto sock_addr_in6_ = *const_cast<SockAddrIn6 *>(&sock_addr_in6);

  sock_addr_in6_.sin6_family = AF_INET6;
  // host to network byte order as unsigned short
  sock_addr_in6_.sin6_port   = htons(port_num);

  // address to network byte order
  if (inet_pton(AF_INET6, ip_addr_v6, &(sock_addr_in6_.sin6_addr)) == 0) {
    cerr << "ERR!  inet_aton() parsing address to network byte order failed!";
    exit(ERR_CODE_INET_ATON_FAILED);
  }
}