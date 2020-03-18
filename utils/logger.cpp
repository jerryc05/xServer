#include "logger.hpp"

[[maybe_unused]] ostream &log_e() {
  return cerr << "ERR!  ";
}

[[maybe_unused]] ostream &log_i() {
  return cout << "INFO! ";
}

[[maybe_unused]] ostream &log_d() {
#ifndef NDEBUG
  return cout << "DBG!  ";
#endif
}