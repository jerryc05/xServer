#include "logger.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <chrono>
#include <csignal>

using std::setw, std::move;
using OutFileStream = std::ofstream;
using TimeUnit = std::chrono::microseconds;
using Sec = std::chrono::seconds;
using HighResClock = std::chrono::high_resolution_clock;
using RuntimeError = std::runtime_error;

constexpr auto TIME_FMT1 = "%FT%T";
constexpr auto TIME_FMT2 = "%z";

#ifdef unix
constexpr auto RED_FMT = "\033[1;91m";  // Bold High Intensity Red
constexpr auto GRN_FMT = "\033[1;92m";  // Bold High Intensity Green
constexpr auto LGR_FMT = "\033[0;37m";  // Light Grey
constexpr auto NOC_FMT = "\033[0m";  // No Colors
#endif

#ifndef LOG_FILENAME_OVERRIDE
#define LOG_FILENAME_OVERRIDE
constexpr auto LOG_FILENAME = "log.log";
#endif

auto &write_time(OutStream &stream) {
  auto now = HighResClock::now();

  constexpr auto UNIT   = (Sec(1) - TimeUnit(0)).count();
  constexpr auto DIGITS = []() {
    switch (UNIT) {
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
  }();
  auto           sub_s  = duration_cast<TimeUnit>(now.time_since_epoch()) % UNIT;

  auto time_ = HighResClock::to_time_t(now);
  auto tm    = *std::localtime(&time_);

  return stream << std::put_time(&tm, TIME_FMT1)
                << '.' << std::setfill('0') << std::setw(DIGITS) << sub_s.count()
                << std::put_time(&tm, TIME_FMT2);
}

inline auto &file_logger() {
  static OutFileStream o_file;
  if (!o_file.is_open()) {
    o_file.open(
            LOG_FILENAME,
            std::ios_base::out | std::ios_base::binary | std::ios_base::app);
    if (!o_file.is_open())
      cerr << "Failed to open log file: " << LOG_FILENAME << '\n';

    o_file << '\n';

    constexpr auto sig_handler = [](auto sig) {
      write_time(o_file << '\n')
              << " | ERR  | Program terminated by signal: " << sig;
      o_file.flush();
    };
    signal(SIGABRT, sig_handler);
    signal(SIGFPE, sig_handler);
    signal(SIGILL, sig_handler);
    signal(SIGINT, sig_handler);
    signal(SIGSEGV, sig_handler);
    signal(SIGTERM, sig_handler);
  }
  return o_file;
}

template<typename T>
inline void log_append(OutStream &stream, T msg) {
  file_logger() << msg;
  stream << msg;
}

BaseLogger::BaseLogger(OutStream &stream,
                       const char *const type_str,
                       const char *const color)
        : stream_(stream), type_str_(type_str) {
  write_time(file_logger() << '\n') << " | " << type_str << " | ";
  write_time(stream << color << '\n') << " | " << type_str << " | ";
}

BaseLogger::operator OutStream &() {
  return stream_;
}

template<typename T>
BaseLogger &BaseLogger::operator<<(T msg) {
  log_append(stream_, msg);
  return *this;
}

BaseLogger::~BaseLogger() {
  stream_ << NOC_FMT;
}

template<>
BaseLogger &BaseLogger::operator<<(const char *msg) {
  log_append(stream_, msg);
  return *this;
}

template<>
BaseLogger &BaseLogger::operator<<(char *msg) {
  log_append(stream_, msg);
  return *this;
}

template<>
BaseLogger &BaseLogger::operator<<(char msg) {
  log_append(stream_, msg);
  return *this;
}

template<>
BaseLogger &BaseLogger::operator<<(int msg) {
  log_append(stream_, msg);
  return *this;
}

template<>
BaseLogger &BaseLogger::operator<<(unsigned short msg) {
  log_append(stream_, msg);
  return *this;
}

template<>
BaseLogger &BaseLogger::operator<<(unsigned int msg) {
  log_append(stream_, msg);
  return *this;
}

template<>
BaseLogger &BaseLogger::operator<<(unsigned long msg) {
  log_append(stream_, msg);
  return *this;
}

ErrLogger::ErrLogger() : BaseLogger(cerr, "ERR ", RED_FMT) {
}

ErrLogger::~ErrLogger() {
  stream_.flush();
  file_logger().flush();
}

InfoLogger::InfoLogger() : BaseLogger(cout, "INFO", GRN_FMT) {
}

DbgLogger::DbgLogger() : BaseLogger(cout, "DBG ", LGR_FMT) {
}
