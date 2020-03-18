#include "x-server/env-settings.hpp"

auto init_env() {
#ifndef NDEBUG
  // debug mode
#else
  // non-debug mode
  std::ios::sync_with_stdio(false);
#endif
}

int main(int argc, char *argv[]) {
  init_env();


  return 0;
}