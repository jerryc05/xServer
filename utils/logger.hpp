#ifndef XSERVER_LOGGER_HPP
#define XSERVER_LOGGER_HPP

#include <iostream>

using std::cout, std::cerr, std::ostream;

[[maybe_unused]] ostream &log_e();

[[maybe_unused]] ostream &log_i();

[[maybe_unused]] ostream &log_d();

#endif //XSERVER_LOGGER_HPP
