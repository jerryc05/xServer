#ifndef XSERVER_ENV_SETTINGS_HPP
#define XSERVER_ENV_SETTINGS_HPP

#include <iostream>
using std::cout;

const int ERR_CODE_EPOLL_CREATE1_ERROR = 1;

auto init_env() {
#ifndef NDEBUG
  // debug mode
#else
  // non-debug mode
  std::ios::sync_with_stdio(false);
#endif
}

#endif // XSERVER_ENV_SETTINGS_HPP