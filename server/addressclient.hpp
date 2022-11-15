#pragma once

#include <thread>

#include "types.hpp"
#include "tcpsocket.hpp"
#include "netpacket.hpp"
#include "addressinfo.hpp"

class AddressServer;

class AddressClient
{
private:
    TcpSocket *m_sock{nullptr};

    AddressServer *m_parent{nullptr};

    std::thread *m_worker{nullptr};

    bool m_isRunning{false};

    bool m_isConnected{false};

public:
    AddressClient(socktype sock, AddressServer *parent);

    AddressClient(const AddressClient &other) = delete;

    AddressClient operator=(const AddressClient &other) = delete;

    ~AddressClient();

    bool isConnected() const;

    void start();

    void stop();
};
