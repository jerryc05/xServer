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

// Windows
#if defined(_WIN32) || defined(_WIN64)
#if defined(_WIN64) && _WIN64
#define _ENV_64
#elif defined(_WIN32) && _WIN32
#define _ENV_32
#endif // _WIN64
#endif // defined(_WIN32) || defined(_WIN64)

// GCC
#if defined(__GNUC__) && __GNUC__
#if (defined(__x86_64__) && __x86_64__) || (defined(__ppc64__) && __ppc64__)
#define _ENV_64
#else
#define _ENV_32
#endif // (defined(__x86_64__) && __x86_64__) ||
// (defined(__ppc64__) &&__ppc64__)
#endif // defined(__GNUC__) && __GNUC__

#endif // XSERVER_ENV_SETTINGS_HPP