#include "logger.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>

using std::setw,std::move;
using OutFileStream = std::ofstream;

constexpr auto DELIMITER_COUNT = 64;
constexpr auto DELIMITER_CHAR  = '-';

#ifndef LOG_FILENAME_OVERRIDE
#define LOG_FILENAME_OVERRIDE
constexpr auto LOG_FILENAME = "log.log";
#endif

static auto &file_logger() {
  static OutFileStream o_file;
  if (!o_file.is_open()) {
    o_file.open(
            LOG_FILENAME,
            std::ios_base::out | std::ios_base::binary | std::ios_base::ate);
    if (!o_file.is_open())
      cerr << "Failed to open log file: " << LOG_FILENAME;
  }
  return o_file;
}

inline auto &&prefix(OutStream &stream) {
  cout.fill(DELIMITER_CHAR);
  return stream << setw(DELIMITER_COUNT) << '\n';
}

template<typename... Ts>
inline OutStream &&process_content(OutStream &stream, const Ts &... args) {
  return move((stream << ...<< args) << '\n');
}

template<typename... Ts>
inline OutStream &&log_internal(OutStream &stream,
                           const char *const &log_type,
                           const Ts &... args) {
  process_content(prefix(file_logger()) << log_type, args...);
  return process_content(prefix(stream) << log_type, args...);
}

template<typename... Ts>
OutStream &&log_e(const Ts &... args) {
  return log_internal(cerr, "ERR | ", args...);
}
OutStream &&log_e(const char* & args) {
  return log_internal(cerr, "ERR | ", args);
}

template<typename... Ts>
OutStream &&log_i(const Ts &... args) {
  return log_internal(cout, "INFO| ", args...);
}

template<typename... Ts>
OutStream &&log_d(const Ts &... args) {
  return log_internal(cout, "DBG | ", args...);
}