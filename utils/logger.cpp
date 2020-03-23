#include "logger.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <chrono>

using std::setw, std::move;
using OutFileStream = std::ofstream;
using TimeUnit = std::chrono::microseconds;
using Sec = std::chrono::seconds;
using HighResClock = std::chrono::high_resolution_clock;
using RuntimeError = std::runtime_error;

constexpr auto TIME_FMT1 = "%FT%T";
constexpr auto TIME_FMT2 = "%z";

#ifndef LOG_FILENAME_OVERRIDE
#define LOG_FILENAME_OVERRIDE
constexpr auto LOG_FILENAME = "log.log";
#endif

inline auto &file_logger() {
  static OutFileStream o_file(
          LOG_FILENAME,
          std::ios_base::out | std::ios_base::binary | std::ios_base::app);
  if (!o_file.is_open())
    cerr << "Failed to open log file: " << LOG_FILENAME << '\n';
  return o_file;
}

constexpr inline auto log10(long n) {
  switch (n) {
    case 1:
      return 0;
    case 1000:
      return 3;
    case 1000000:
      return 6;
    case 1000000000:
      return 9;
    default:
      throw RuntimeError("log10(): Invalid UNIT number");
  }
}

auto &write_time(OutStream &stream) {
  auto now = HighResClock::now();

  constexpr auto UNIT  = (Sec(1) - TimeUnit(0)).count();
  auto           sub_s = duration_cast<TimeUnit>(now.time_since_epoch()) % UNIT;

  auto time_ = HighResClock::to_time_t(now);
  auto tm    = *std::localtime(&time_);

  return stream << std::put_time(&tm, TIME_FMT1)
                << '.' << std::setfill('0') << std::setw(log10(UNIT)) << sub_s.count()
                << std::put_time(&tm, TIME_FMT2);
}

inline OutStream &log_start(OutStream &stream, const char *type_str) {
  write_time(file_logger()) << " | " << type_str << " | ";
  return write_time(stream) << " | " << type_str << " | ";
}

template<typename T>
inline OutStream &log_append(OutStream &stream, T msg) {
  file_logger() << msg;
  return stream << msg;
}

BaseLogger::BaseLogger(OutStream &stream, const char *type_str)
        : stream_(stream), type_str_(type_str) {
  log_start(stream, type_str);
}

BaseLogger::~BaseLogger() {
  *this << '\n';
}

BaseLogger::operator OutStream &() {
  return stream_;
}

template<typename T>
OutStream &BaseLogger::operator<<(T msg) {
  return log_append(stream_, msg);
}

template<>
OutStream &BaseLogger::operator<<(const char *msg) {
  return log_append(stream_, msg);
}

template<>
OutStream &BaseLogger::operator<<(int msg) {
  return log_append(stream_, msg);
}

ErrLogger::ErrLogger() : BaseLogger(cerr, "ERR ") {
}

InfoLogger::InfoLogger() : BaseLogger(cout, "INFO") {
}

DbgLogger::DbgLogger() : BaseLogger(cout, "DBG ") {
}
