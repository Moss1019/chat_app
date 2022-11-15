#include "tcpsocket.hpp"

#include <iostream>

#ifdef _WIN32

#else
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#endif

TcpSocket::TcpSocket()
{
    m_sock = socket(AF_INET, SOCK_STREAM, 0);
    m_isConnected = true;
}

TcpSocket::TcpSocket(socktype sock)
{
    m_sock = sock;
    m_isConnected = true;
}

TcpSocket::~TcpSocket()
{
    disconnect();
}

bool TcpSocket::inError() const
{
    return m_inError;
}

bool TcpSocket::isConnected() const
{
    return m_isConnected;
}

const std::string &TcpSocket::error() const
{
    return m_error;
}

void TcpSocket::disconnect()
{
    if(m_sock > 0)
    {
        m_isConnected = false;
        #ifdef _WIN32

        #else
        shutdown(m_sock, SHUT_RDWR);
        close(m_sock);
        #endif
    }
}

void TcpSocket::bindAndListen(const SocketAddress &local)
{
    int hres = bind(m_sock, local.constAddr(), local.addrLength());
    if(hres < 0)
    {
        m_inError = true;
        m_error = "Failed to bind to underlying address";
        return;
    }
    hres = listen(m_sock, 2);
    if(hres < 0)
    {
        m_inError = true;
        m_error = "Failed to set listening state";
    }
    m_isServer = true;
}

socktype TcpSocket::acceptConnection() const
{
    sockaddr addr;
    sockaddrlen length;
    return accept(m_sock, &addr, &length);
}

void TcpSocket::connectTo(const SocketAddress &remoteHost)
{
    if(m_isServer)
    {
        m_inError = true;
        m_error = "Socket is being used as server socket";
        return;
    }
    int hres = connect(m_sock, remoteHost.constAddr(), remoteHost.addrLength());
    if(hres < 0)
    {
        m_inError = true;
        m_error = "Failed to connect to remote host";
    }
}

int TcpSocket::sendData(const MemoryOutputStream &stream)
{
    auto sent = send(m_sock, stream.buffer(), stream.size(), 0);
    if(sent == -1)
    {
        return static_cast<int>(Status::op_unsuccessful);
    }
    return sent;
}

int TcpSocket::recvData(MemoryOutputStream &stream)
{
    const auto bufferSize = 1024;
    auto totalRead = 0;
    auto read = 0;
    char buffer[bufferSize];
    do 
    {
        read = recv(m_sock, buffer, bufferSize, 0);
        if(read == -1)
        {
            return static_cast<int>(Status::op_unsuccessful);
        }
        if(read == 0)
        {
            m_inError = true;
            m_error = "Socket closed";
            return static_cast<int>(Status::closed);
        }
        totalRead += read;
        stream.writeData(buffer, read);
    } while(read == bufferSize);
    return totalRead;
}

void TcpSocket::setBlocking(bool isBlocking)
{
    #ifdef _WIN32

    #else
    auto flags = fcntl(m_sock, F_GETFL);
    flags = isBlocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
    fcntl(m_sock, F_SETFL, flags);
    #endif
}