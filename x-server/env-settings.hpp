#ifndef XSERVER_ENV_SETTINGS_HPP
#define XSERVER_ENV_SETTINGS_HPP

#include <iostream>
#include <tuple>
#include <optional>
#include "../utils/logger.hpp"

using std::cout, std::cerr, std::make_tuple;
template<typename T1, typename T2> using Tuple = std::tuple<T1, T2>;
template<typename T> using Optional = std::optional<T>;


#ifndef LISTEN_BACKLOG
#define LISTEN_BACKLOG (16)
#endif
#ifndef MAX_EPOLL_EVENT_COUNT
#define MAX_EPOLL_EVENT_COUNT (static_cast<int>(1.5 * LISTEN_BACKLOG))
#endif

#define ERR_CODE_EPOLL_CREATE1              ( 1)
#define ERR_CODE_CREATE_SOCKET              ( 2)
#define ERR_CODE_INET_ATON                  ( 3)
#define ERR_CODE_SET_SOCK_OPT_REUSE         ( 4)
#define ERR_CODE_BIND_SOCK                  ( 5)
#define ERR_CODE_LISTEN_SOCK                ( 6)
#define ERR_CODE_EPOLL_CTL_ADD              ( 7)
#define ERR_CODE_INET_NTOP                  ( 8)
#define ERR_CODE_EPOLL_WAIT                 ( 9)
#define ERR_CODE_REACH_END_OF_NON_VOID_FUNC (10)
#define ERR_STR_REACH_END_OF_NON_VOID_FUNC  ("Control reaches end of non-void function\n")

#endif // XSERVER_ENV_SETTINGS_HPP