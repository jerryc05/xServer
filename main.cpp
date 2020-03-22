#include "x-server/env-settings.hpp"
#include "x-server/IpAddr.hpp"
#include "x-server/Socket.hpp"
#include <string>

inline auto init_env() {
#ifndef NDEBUG
  // debug mode
#else
  // non-debug mode
  std::ios::sync_with_stdio(false);
#endif
}

int main(int argc, char *argv[]) {
  init_env();

  uint16_t port_num = static_cast<uint16_t>(std::stoi(argv[2]));
  if (inet_aton(argv[1], nullptr) != 0) {
    IpAddrV4  ip(argv[1], port_num);
    TcpSocket tcp(ip);
    tcp.loop();
  } else {
    IpAddrV6  ip(argv[1], port_num);
    TcpSocket tcp(ip);
    tcp.loop();
  }
}