#ifndef XSERVER_ENV_SETTINGS_HPP
#define XSERVER_ENV_SETTINGS_HPP

#ifdef unix
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>

using SockAddrFamily = sa_family_t;
using SockAddr = sockaddr;
using SockAddrIn = sockaddr_in;
using SockAddrIn6 = sockaddr_in6;
#endif

#include <cassert>
#include <cerrno>
#include <cstring>
#include <iostream>
using std::cout;

const int ERR_CODE_EPOLL_CREATE1_ERROR = 1;
const int ERR_CODE_CREATE_SOCKET_ERROR = 2;
const int ERR_CODE_INET_ATON_FAILED = 3;

#endif // XSERVER_ENV_SETTINGS_HPP