#pragma once

#ifdef _WIN32
typedef SOCKET socktype;
typedef socklen_t sockaddrlen;
#else
#include <sys/socket.h>

typedef int socktype;
typedef socklen_t sockaddrlen;
#endif
