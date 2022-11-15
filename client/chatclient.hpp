#pragma once

#include <thread>

#include "udpsocket.hpp"
#include "socketaddress.hpp"

class ChatClient
{
private:
    UdpSocket *m_sock{nullptr};

    std::thread *m_worker{nullptr};

    bool m_isRunning{false};

    void (*m_onReceive)(const SocketAddress &remoteAddress, const std::string &msg);

public:
    ChatClient(const SocketAddress &addr, void (*onReceive)(const SocketAddress &remoteAddress, const std::string &msg));

    ChatClient(const ChatClient &other) = delete;

    ChatClient operator=(const ChatClient &other) = delete;

    ~ChatClient();

    bool inError() const;

    const std::string &error() const;

    void sendMsg(const std::string &msg, const SocketAddress &destination);

    void start();

    void stop();
};
