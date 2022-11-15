#pragma once

#include <vector>
#include <string>

#include "tcpsocket.hpp"
#include "addressinfo.hpp"
#include "socketaddress.hpp"

class AddressClient
{
private:
    TcpSocket *m_sock{nullptr};

    std::string m_ipAddress{""};

    short m_port{0};

    bool m_isConnected{false};
public:
    AddressClient(const SocketAddress &local, const SocketAddress &server);

    AddressClient(const AddressClient &other) = delete;

    AddressClient operator=(const AddressClient &other) = delete;

    ~AddressClient();

    bool inError() const;

    const std::string &error() const;

    void getAddresses(std::vector<AddressInfo> &addresses) const;

    void disconnect();
};
