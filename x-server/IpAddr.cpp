#include "IpAddr.hpp"

IpAddr::IpAddr(IpAddrType addr_type_) : addr_type(addr_type_) {}

SockAddrFamily IpAddr::addr_family() const {
  switch (addr_type) {
    case IpAddrType::IpAddrV4:
      return reinterpret_cast<const IpAddrV4 *>(this)->sock_addr_in.sin_family;
    case IpAddrType::IpAddrV6:
      return reinterpret_cast<const IpAddrV6 *>(this)->sock_addr_in6.sin6_family;
  }
}

SockAddrInfo &IpAddr::sock_addr_info(SockAddrInfo &info) const {
  switch (addr_type) {
    case IpAddrType::IpAddrV4: {
      auto ptr = reinterpret_cast<const IpAddrV4 *>(this);
      get<0>(info) = reinterpret_cast<const SockAddr *>(&ptr->sock_addr_in);
      get<1>(info) = static_cast<SockCallLen >(sizeof(ptr->sock_addr_in));
      break;
    }

    case IpAddrType::IpAddrV6: {
      auto ptr = reinterpret_cast<const IpAddrV6 *>(this);
      get<0>(info) = reinterpret_cast<const SockAddr *>(&ptr->sock_addr_in6);
      get<1>(info) = static_cast<SockCallLen >(sizeof(ptr->sock_addr_in6));
      break;
    }
  }
  return info;
}

[[maybe_unused]] IpAddrV4::IpAddrV4(const char *ip_addr_v4, uint16_t port_num)
        : IpAddr(IpAddrType::IpAddrV4) {
  auto ptr = const_cast<SockAddrIn *>(&sock_addr_in);

  ptr->sin_family = AF_INET;
  // host to network byte order as unsigned short
  ptr->sin_port   = htons(port_num);

  // address to network byte order
  if (inet_aton(ip_addr_v4, &(ptr->sin_addr)) == 0) {
    cerr << "ERR!  inet_aton() parsing address to network byte order failed!";
    exit(ERR_CODE_INET_ATON_FAILED);
  }
}

[[maybe_unused]] IpAddrV6::IpAddrV6(const char *ip_addr_v6, uint16_t port_num)
        : IpAddr(IpAddrType::IpAddrV6) {
  auto ptr = const_cast<SockAddrIn6 *>(&sock_addr_in6);

  ptr->sin6_family = AF_INET6;
  // host to network byte order as unsigned short
  ptr->sin6_port   = htons(port_num);

  // address to network byte order
  if (inet_pton(AF_INET6, ip_addr_v6, &(ptr->sin6_addr)) == 0) {
    cerr << "ERR!  inet_aton() parsing address to network byte order failed!";
    exit(ERR_CODE_INET_ATON_FAILED);
  }
}