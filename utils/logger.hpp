#ifndef XSERVER_LOGGER_HPP
#define XSERVER_LOGGER_HPP

#include <iostream>

using std::cout, std::cerr;
using OutStream = std::ostream;

template<typename ...Ts>
[[maybe_unused]] OutStream &&log_e(const Ts &...args);

template<typename ...Ts>
[[maybe_unused]] OutStream &&log_i(const Ts &...args);

#ifndef NDEBUG

template<typename ...Ts>
[[maybe_unused]] OutStream &&log_d(const Ts &...args);

#endif

#endif //XSERVER_LOGGER_HPP
