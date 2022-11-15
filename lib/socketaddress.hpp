#pragma once

#include <string>

#ifdef _WIN32

#else
#include <arpa/inet.h>
#endif

class SocketAddress
{
private:
    sockaddr m_addr{};

    std::string m_ipAddress{""};

public:
    SocketAddress();

    SocketAddress(const std::string &ipAddress, short port);

    SocketAddress(const SocketAddress &other) = delete;

    SocketAddress operator=(const SocketAddress &other) =  delete;

    short port() const;

    const std::string &ipAddress() const;

    const sockaddr *constAddr() const;

    sockaddr *addr();

    size_t addrLength() const;

    void setAddress(sockaddr &addr);
};