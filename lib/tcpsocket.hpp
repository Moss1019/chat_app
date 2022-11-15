#pragma once

#include <string>

#include "types.hpp"
#include "socketaddress.hpp"
#include "memoryinputstream.hpp"
#include "memoryoutputstream.hpp"

class TcpSocket
{
private:
    socktype m_sock{0};

    bool m_inError{false};

    bool m_isServer{false};

    bool m_isConnected{false};

    std::string m_error{""};
public:
    enum Status : int
    {
        closed = -1,
        op_unsuccessful = -2
    };

    TcpSocket();

    TcpSocket(socktype sock);

    TcpSocket(const TcpSocket &other) = delete;

    TcpSocket operator=(const TcpSocket &other) = delete;

    ~TcpSocket();

    bool inError() const;

    bool isConnected() const;

    const std::string &error() const;

    socktype acceptConnection() const;

    void setBlocking(bool isBlocking);

    void disconnect();

    void bindAndListen(const SocketAddress &local);

    void connectTo(const SocketAddress &remoteHost);

    int sendData(const MemoryOutputStream &stream);

    int recvData(MemoryOutputStream &stream);
};