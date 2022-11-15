#include "chatclient.hpp"

#include "memoryinputstream.hpp"

ChatClient::ChatClient(const SocketAddress &addr, void (*onReceive)(const SocketAddress &remoteAddress, const std::string &msg))
    :m_onReceive{onReceive}
{
    m_sock = new UdpSocket(addr);
}

ChatClient::~ChatClient()
{
    if(m_sock != nullptr)
    {
        m_sock->disconnect();
    }
    if(m_worker != nullptr)
    {
        m_worker->join();
    }
    if(m_sock != nullptr)
    {
        delete m_sock;
        m_sock = nullptr;
    }
    if(m_worker != nullptr)
    {
        delete m_worker;
        m_worker = nullptr;
    }
}

bool ChatClient::inError() const
{
    return m_sock->inError();
}

const std::string &ChatClient::error() const
{
    return m_sock->error();
}

void ChatClient::sendMsg(const std::string &msg, const SocketAddress &destination)
{
    MemoryOutputStream out;
    out.writeUnsigned(msg.length());
    out.writeData(msg.c_str(), msg.length());
    m_sock->sendData(destination, out);
}

void ChatClient::start() 
{
    m_isRunning = true;
    m_worker = new std::thread([this]() {
        while(m_isRunning)
        {
            SocketAddress remoteAddr;
            MemoryOutputStream stream;
            m_sock->receiveData(&remoteAddr, stream);
            MemoryInputStream in(stream.buffer(), stream.size());
            unsigned length = 0;
            in.readUnsigned(&length);
            char *msgBuffer = new char[length];
            in.readData(msgBuffer, length);
            std::string msg(msgBuffer, length);
            delete[] msgBuffer;
            m_onReceive(remoteAddr, msg);
        }
    });
}

void ChatClient::stop()
{
    m_isRunning = false;
    m_sock->disconnect();
}