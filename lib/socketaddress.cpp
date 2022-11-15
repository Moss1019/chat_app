#include "socketaddress.hpp"

SocketAddress::SocketAddress()
{

}

SocketAddress::SocketAddress(const std::string &ipAddress, short port)
    :m_ipAddress{ipAddress}
{
    sockaddr_in in;
    std::memset(&in, 0, sizeof(sockaddr_in));
#ifdef _WIN32

#else
    inet_pton(AF_INET, ipAddress.c_str(), &in.sin_addr);
#endif
    in.sin_family = AF_INET;
    in.sin_port = htons(port);
    m_addr = *reinterpret_cast<sockaddr *>(&in);
}

short SocketAddress::port() const
{
    sockaddr_in addrIn{};
    std::memcpy(&addrIn, &m_addr, sizeof(sockaddr));
    return ntohs(addrIn.sin_port);
}

const std::string &SocketAddress::ipAddress() const
{
    return m_ipAddress;
}

const sockaddr *SocketAddress::constAddr() const
{
    return &m_addr;
}

sockaddr *SocketAddress::addr()
{
    return &m_addr;
}

size_t SocketAddress::addrLength() const
{
    return sizeof(sockaddr);
}

void SocketAddress::setAddress(sockaddr &addr)
{
    std::memcpy(&m_addr, &addr, sizeof(sockaddr));
}
