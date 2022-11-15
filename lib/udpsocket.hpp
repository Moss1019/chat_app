#pragma once

#include <string>

#include "types.hpp"
#include "socketaddress.hpp"
#include "memoryoutputstream.hpp"

class UdpSocket
{
private:
    socktype m_sock{0};

    bool m_inError{false};

    std::string m_error{""};

public:
    enum Status : int
    {
        closed = -1,
        op_unsuccessful = -2
    };
    
    UdpSocket(const SocketAddress &address);

    UdpSocket(const UdpSocket &other) = delete;

    UdpSocket operator=(const UdpSocket &other) = delete;

    ~UdpSocket();

    bool inError() const;

    const std::string &error() const;

    int sendData(const SocketAddress &dest, const MemoryOutputStream &stream);

    int receiveData(SocketAddress *remote, MemoryOutputStream &stream);

    void disconnect();
};
