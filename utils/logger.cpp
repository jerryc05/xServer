#include "logger.hpp"

[[maybe_unused]] ostream &log_e() {
  return cerr << "------\nERR!  ";
}

[[maybe_unused]] ostream &log_i() {
  return cout << "------\nINFO! ";
}

[[maybe_unused]] ostream &log_d() {
#ifndef NDEBUG
  return cout << "------\nDBG!  ";
#endif
}