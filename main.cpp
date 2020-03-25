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

int main([[maybe_unused]] int argc, char *argv[]) try {
  init_env();

  auto tcp = [argv]() {
    uint16_t port_num = static_cast<uint16_t>(std::stoi(argv[2]));
    if (inet_aton(argv[1], nullptr) != 0) {
      IpAddrV4 ip(argv[1], port_num);
      return TcpSocket(ip);
    } else {
      IpAddrV6 ip(argv[1], port_num);
      return TcpSocket(ip);
    }
  }();
  tcp.loop();

} catch (const RuntimeError &e) {
  return -1;
}