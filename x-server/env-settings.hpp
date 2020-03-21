#ifndef XSERVER_ENV_SETTINGS_HPP
#define XSERVER_ENV_SETTINGS_HPP

#include <iostream>
#include <tuple>
#include "../utils/logger.hpp"

using std::cout, std::cerr;
template<typename T1, typename T2> using Pair = std::pair<T1, T2>;

using RuntimeError = std::runtime_error;

#ifndef LISTEN_BACKLOG_OVERRIDE
#define LISTEN_BACKLOG_OVERRIDE
constexpr uint8_t LISTEN_BACKLOG = 16;
#endif

#ifndef MAX_EPOLL_EVENT_COUNT_OVERRIDE
#define MAX_EPOLL_EVENT_COUNT_OVERRIDE
constexpr uint8_t MAX_EPOLL_EVENT_COUNT = static_cast<int>(1.5 * LISTEN_BACKLOG);
#endif

constexpr auto ERR_STR_EPOLL_CREATE1              = "EPOLL_CREATE1";
constexpr auto ERR_STR_CREATE_SOCKET              = "CREATE_SOCKET";
constexpr auto ERR_STR_INET_ATON                  = "INET_ATON";
constexpr auto ERR_STR_SET_SOCK_OPT_REUSE         = "SET_SOCK_OPT_REUSE";
constexpr auto ERR_STR_BIND_SOCK                  = "BIND_SOCK";
constexpr auto ERR_STR_LISTEN_SOCK                = "LISTEN_SOCK";
constexpr auto ERR_STR_EPOLL_CTL_ADD              = "EPOLL_CTL_ADD";
constexpr auto ERR_STR_INET_NTOP                  = "INET_NTOP";
constexpr auto ERR_STR_EPOLL_WAIT                 = "EPOLL_WAIT";
constexpr auto ERR_STR_REACH_END_OF_NON_VOID_FUNC = "REACH_END_OF_NON_VOID_FUNC";
constexpr auto ERR_STR_EPOLL_GOT_INVALID_SOCKFD   = "EPOLL_GOT_INVALID_SOCKFD";

#endif // XSERVER_ENV_SETTINGS_HPP