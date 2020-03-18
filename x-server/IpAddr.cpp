#include "IpAddr.hpp"

IpAddr::IpAddr() = default;

SockAddrFamily IpAddr::addr_family() const { return this->sock_addr.sa_family; }

[[maybe_unused]] IpAddrV4::IpAddrV4(const char *ip_addr_v4, uint16_t port_num) {
  auto sock_addr_in = *reinterpret_cast<SockAddrIn *> (
          const_cast<SockAddr *>(&sock_addr)
  );

  sock_addr_in.sin_family = AF_INET;
  // host to network byte order as unsigned short
  sock_addr_in.sin_port   = htons(port_num);

  // address to network byte order
  if (inet_aton(ip_addr_v4, &(sock_addr_in.sin_addr)) == 0) {
    cout << "ERR!  inet_aton() parsing address to network byte order failed!";
    exit(ERR_CODE_INET_ATON_FAILED);
  }
}

[[maybe_unused]] IpAddrV6::IpAddrV6(const char *ip_addr_v6, uint16_t port_num) {
  auto sock_addr_in6 = *reinterpret_cast<SockAddrIn6 *> (
          const_cast<SockAddr *>(&sock_addr)
  );

  sock_addr_in6.sin6_family = AF_INET6;
  // host to network byte order as unsigned short
  sock_addr_in6.sin6_port   = htons(port_num);

  // address to network byte order
  if (inet_pton(AF_INET6, ip_addr_v6, &(sock_addr_in6.sin6_addr)) == 0) {
    cout << "ERR!  inet_aton() parsing address to network byte order failed!";
    exit(ERR_CODE_INET_ATON_FAILED);
  }
}
