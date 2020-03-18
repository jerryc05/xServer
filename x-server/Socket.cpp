#include "Socket.hpp"

#ifdef unix
#include <sys/socket.h>
#endif

Socket::Socket(const char *ip_addr, int port_num) {}
