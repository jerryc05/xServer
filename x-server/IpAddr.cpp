#include "IpAddr.hpp"

#ifdef unix
#include <netinet/in.h>
#endif

IpAddr::IpAddr(const char *ip_addr, int port_num, int domain)
    : ip_addr(ip_addr), port_num(port_num), domain(domain) {}

[[maybe_unused]] IpAddrV4::IpAddrV4(const char *ip_addr_v4, int port_num)
    : IpAddr(ip_addr_v4, port_num, AF_INET) {}

[[maybe_unused]] IpAddrV6::IpAddrV6(const char *ip_addr_v6, int port_num)
    : IpAddr(ip_addr_v6, port_num, AF_INET6) {}
