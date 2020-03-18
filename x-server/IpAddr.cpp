#include "IpAddr.hpp"

IpAddr::IpAddr() = default;

IpAddr::~IpAddr() = default;

[[maybe_unused]] IpAddrV4::IpAddrV4(const char *ip_addr_v4, uint16_t port_num) {
  auto sock_addr_in_ = *const_cast<SockAddrIn *>(&sock_addr_in);

  sock_addr_in_.sin_family = AF_INET;
  // host to network byte order as unsigned short
  sock_addr_in_.sin_port   = htons(port_num);

  // address to network byte order
  if (inet_aton(ip_addr_v4, &(sock_addr_in_.sin_addr)) == 0) {
    cout << "ERR!  inet_aton() parsing address to network byte order failed!";
    exit(ERR_CODE_INET_ATON_FAILED);
  }
}

inline SockAddrFamily IpAddrV4::addr_family() const {
  return sock_addr_in.sin_family;
}

inline SockAddrInfo &IpAddrV4::sock_addr_info(SockAddrInfo &info) const {
  get<0>(info) = reinterpret_cast<const SockAddr *>(&sock_addr_in);
  get<1>(info) = static_cast<SockAddrLen >(sizeof(sock_addr_in));
  return info;
}

[[maybe_unused]] IpAddrV6::IpAddrV6(const char *ip_addr_v6, uint16_t port_num) {
  auto sock_addr_in6_ = *const_cast<SockAddrIn6 *>(&sock_addr_in6);

  sock_addr_in6_.sin6_family = AF_INET6;
  // host to network byte order as unsigned short
  sock_addr_in6_.sin6_port   = htons(port_num);

  // address to network byte order
  if (inet_pton(AF_INET6, ip_addr_v6, &(sock_addr_in6_.sin6_addr)) == 0) {
    cout << "ERR!  inet_aton() parsing address to network byte order failed!";
    exit(ERR_CODE_INET_ATON_FAILED);
  }
}

inline SockAddrFamily IpAddrV6::addr_family() const {
  return sock_addr_in6.sin6_family;
}

inline SockAddrInfo &IpAddrV6::sock_addr_info(SockAddrInfo &info) const {
  get<0>(info) = reinterpret_cast<const SockAddr *>(&sock_addr_in6);
  get<1>(info) = static_cast<SockAddrLen >(sizeof(sock_addr_in6));
  return info;
}