#ifndef XSERVER_ENV_SETTINGS_HPP
#define XSERVER_ENV_SETTINGS_HPP

#include <iostream>
#include "../utils/logger.hpp"

using std::cout, std::cerr;

const int ERR_CODE_EPOLL_CREATE1_ERROR      = 1;
const int ERR_CODE_CREATE_SOCKET_ERROR      = 2;
const int ERR_CODE_INET_ATON_ERROR          = 3;
const int ERR_CODE_SET_SOCK_OPT_REUSE_ERROR = 4;
const int ERR_CODE_BIND_SOCK_ERROR          = 5;
const int ERR_CODE_LISTEN_SOCK_ERROR        = 6;

const int ERR_CODE_INET_NTOP_ERROR          = 8;

#endif // XSERVER_ENV_SETTINGS_HPP