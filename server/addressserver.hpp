#pragma once

#include <thread>
#include <string>
#include <vector>

#include "tcpsocket.hpp"
#include "addressinfo.hpp"
#include "addressclient.hpp"

class AddressServer 
{
private:
    SocketAddress m_localAddress{};

    TcpSocket *m_sock{nullptr};

    std::thread *m_worker{nullptr};

    std::thread *m_clientWorker{nullptr};

    bool m_isRunning{false};

    std::vector<AddressInfo> m_addresses{};

    std::vector<AddressClient *> m_clients{};

public:
    AddressServer();

    AddressServer(const AddressServer &other) = delete;

    AddressServer operator=(const AddressServer &other) = delete;

    ~AddressServer();

    bool inError() const;

    const std::string &error() const;

    const std::vector<AddressInfo> &addresses() const;

    void start(const SocketAddress &local);

    void stop();

    void addAddress(const std::string &ipAddress, short port);

    void removeAddress(const std::string &ipAddress, short port);
};