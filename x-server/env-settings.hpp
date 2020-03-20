#ifndef XSERVER_ENV_SETTINGS_HPP
#define XSERVER_ENV_SETTINGS_HPP

#include <iostream>
#include <tuple>
#include "../utils/logger.hpp"

using std::cout, std::cerr;
template<typename T1, typename T2> using Pair = std::pair<T1, T2>;


#ifndef LISTEN_BACKLOG_OVERRIDE
#define LISTEN_BACKLOG_OVERRIDE
constexpr uint8_t LISTEN_BACKLOG = 16;
#endif

#ifndef MAX_EPOLL_EVENT_COUNT_OVERRIDE
#define MAX_EPOLL_EVENT_COUNT_OVERRIDE
constexpr uint8_t MAX_EPOLL_EVENT_COUNT = static_cast<int>(1.5 * LISTEN_BACKLOG);
#endif

constexpr uint8_t ERR_CODE_EPOLL_CREATE1              = 1;
constexpr uint8_t ERR_CODE_CREATE_SOCKET              = 2;
constexpr uint8_t ERR_CODE_INET_ATON                  = 3;
constexpr uint8_t ERR_CODE_SET_SOCK_OPT_REUSE         = 4;
constexpr uint8_t ERR_CODE_BIND_SOCK                  = 5;
constexpr uint8_t ERR_CODE_LISTEN_SOCK                = 6;
constexpr uint8_t ERR_CODE_EPOLL_CTL_ADD              = 7;
constexpr uint8_t ERR_CODE_INET_NTOP                  = 8;
constexpr uint8_t ERR_CODE_EPOLL_WAIT                 = 9;
constexpr uint8_t ERR_CODE_REACH_END_OF_NON_VOID_FUNC = 10;
constexpr auto    ERR_STR_REACH_END_OF_NON_VOID_FUNC  =
                          "Control reaches end of non-void function\n";

#endif // XSERVER_ENV_SETTINGS_HPP