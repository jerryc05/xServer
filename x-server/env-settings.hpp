#include <iostream>

auto init_env() {
#ifndef NDEBUG
// debug mode
#else
  // non-debug mode
  std::ios::sync_with_stdio(false);
#endif
}