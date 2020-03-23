#include "IpAddr.hpp"

IpAddr::IpAddr(IpAddrType addr_type_) : addr_type(addr_type_) {}

const SockAddrFamily &IpAddr::addr_family() const {
  switch (addr_type) {
    case IpAddrType::IpAddrV4:
      return reinterpret_cast<const IpAddrV4 *>(this)->sock_addr_in.sin_family;
    case IpAddrType::IpAddrV6:
      return reinterpret_cast<const IpAddrV6 *>(this)->sock_addr_in6.sin6_family;
  }
  ErrLogger() << "IpAddr::addr_family():\n\t" << ERR_STR_REACH_END_OF_NON_VOID_FUNC;
  throw RuntimeError(ERR_STR_REACH_END_OF_NON_VOID_FUNC);
}

Pair<const SockAddr *, SockCallLen> IpAddr::sock_addr_info() const {
  switch (addr_type) {
    case IpAddrType::IpAddrV4: {
      auto ptr = reinterpret_cast<const IpAddrV4 *>(this);
      return {
              reinterpret_cast<const SockAddr *>(&ptr->sock_addr_in),
              static_cast<SockCallLen >(sizeof(ptr->sock_addr_in))
      };
    }
    case IpAddrType::IpAddrV6: {
      auto ptr = reinterpret_cast<const IpAddrV6 *>(this);
      return {
              reinterpret_cast<const SockAddr *>(&ptr->sock_addr_in6),
              static_cast<SockCallLen >(sizeof(ptr->sock_addr_in6))};
    }
  }
  ErrLogger() << "IpAddr::sock_addr_info():\n\t" << ERR_STR_REACH_END_OF_NON_VOID_FUNC;
  throw RuntimeError(ERR_STR_REACH_END_OF_NON_VOID_FUNC);
}

[[maybe_unused]] IpAddrV4::IpAddrV4(const char *const &ip_addr_v4, uint16_t port_num)
        : IpAddr(IpAddrType::IpAddrV4), sock_addr_in() {
  auto ptr = const_cast<SockAddrIn *>(&sock_addr_in);

  ptr->sin_family = AF_INET;
  // host to network byte order as unsigned short
  ptr->sin_port   = htons(port_num);

  // address to network byte order
  if (inet_aton(ip_addr_v4, &(ptr->sin_addr)) == 0) {
    ErrLogger() << ("IpAddrV4::IpAddrV4() >> inet_aton()");
    throw RuntimeError(ERR_STR_INET_ATON);
  }
#ifndef NDEBUG
  DbgLogger() << "IPv4 constructed: " << ip_addr_v4;
#endif
}

[[maybe_unused]] IpAddrV6::IpAddrV6(const char *const &ip_addr_v6, uint16_t port_num)
        : IpAddr(IpAddrType::IpAddrV6), sock_addr_in6() {
  auto ptr = const_cast<SockAddrIn6 *>(&sock_addr_in6);

  ptr->sin6_family = AF_INET6;
  // host to network byte order as unsigned short
  ptr->sin6_port   = htons(port_num);

  // address to network byte order
  if (inet_pton(AF_INET6, ip_addr_v6, &(ptr->sin6_addr)) == 0) {
    ErrLogger() << "IpAddrV6::IpAddrV6() >> inet_aton()";
    throw RuntimeError(ERR_STR_INET_ATON);
  }
#ifndef NDEBUG
  DbgLogger() << "IPv6 constructed: " << ip_addr_v6;
#endif
}