#include "x-server/env-settings.hpp"
#include "x-server/IpAddr.hpp"
#include "x-server/Socket.hpp"

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

  IpAddrV4  ip("127.0.0.1", 6006);
  TcpSocket tcp(ip);
  tcp.loop();
}