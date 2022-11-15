#include "udpsocket.hpp"

#ifdef _WIN32

#else
#include <unistd.h>
#include <sys/socket.h>
#endif

#include <iostream>

UdpSocket::UdpSocket(const SocketAddress &address)
{
    m_sock = socket(AF_INET, SOCK_DGRAM, 0);
    int hres = bind(m_sock, address.constAddr(), address.addrLength());
    if(hres < 0)
    {
        m_inError = true;
        m_error = "Failed to bind to socket";
    }
}

UdpSocket::~UdpSocket()
{
    disconnect();
}

bool UdpSocket::inError() const
{
    return m_inError;
}

const std::string &UdpSocket::error() const
{
    return m_error;
}

int UdpSocket::sendData(const SocketAddress &dest, const MemoryOutputStream &stream)
{
    if(m_inError) 
    {
        return static_cast<int>(Status::closed);;
    }
    auto sent = static_cast<int>(sendto(m_sock, stream.buffer(), stream.size(), 0, dest.constAddr(), dest.addrLength()));
    if(sent == -1)
    {
        return static_cast<int>(Status::op_unsuccessful);
    }
    return sent;
}

int UdpSocket::receiveData(SocketAddress *remote, MemoryOutputStream &stream)
{
    if(m_inError)
    {
        return static_cast<int>(Status::closed);
    }
    const int bufferSize = 1024;
    char buffer[bufferSize];
    sockaddr rmt;
    sockaddrlen length;
    auto read = static_cast<unsigned>(recvfrom(m_sock, buffer, bufferSize, 0, &rmt, &length));
    if(read < 0) 
    {
        m_inError = true;
        m_error = "Socket closed";
        return static_cast<int>(Status::op_unsuccessful);
    }
    remote->setAddress(rmt);
    stream.writeUnsigned(read);
    stream.writeData(buffer, read);
    return read;
}

void UdpSocket::disconnect()
{
    if(m_sock > 0)
    {
#ifdef _WIN32

#else
    shutdown(m_sock, SHUT_RDWR);
    close(m_sock);
#endif
    }
}
