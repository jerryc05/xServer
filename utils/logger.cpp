#include "logger.hpp"
#include <iomanip>

using std::setw;
template<typename T> using BasicOutStream = std::basic_ostream<T>;

constexpr auto DELIMITER_COUNT = 16;
constexpr auto DELIMITER_CHAR  = '-';

template<typename T>
inline auto &prefix(BasicOutStream<T> &stream) {
  return stream << setw(DELIMITER_COUNT) << cout.fill(DELIMITER_CHAR) << '\n';
}

[[maybe_unused]] ostream &log_e() {
  return prefix(cerr) << "ERR | ";
}

[[maybe_unused]] ostream &log_i() {
  return prefix(cout) << "INFO| ";
}

[[maybe_unused]] ostream &log_d() {
  return prefix(cout) << "DBG | ";
}