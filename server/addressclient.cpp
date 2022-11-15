#include "addressclient.hpp"

#include <iostream>

#include "addressserver.hpp"

#include "memoryoutputstream.hpp"

AddressClient::AddressClient(socktype sock, AddressServer *parent)
    :m_parent{parent}
{
    m_sock = new TcpSocket(sock);
    m_isConnected = true;
}

AddressClient::~AddressClient()
{
    stop();
    if(m_worker != nullptr)
    {
        m_worker->join();
        delete m_worker;
        m_worker = nullptr;
    }
    if(m_sock != nullptr)
    {
        delete m_sock;
        m_sock = nullptr;
    }
}

bool AddressClient::isConnected() const
{
    return m_isConnected;
}

void AddressClient::start()
{
    m_isRunning = true;
    m_worker = new std::thread([this]()
    {
        while(m_isRunning && m_isConnected)
        {
            MemoryOutputStream out;
            int read = m_sock->recvData(out);
            if(read == TcpSocket::Status::closed)
            {
                m_isConnected = false;
            }
            else if(read == TcpSocket::Status::op_unsuccessful)
            {
                m_isConnected = false;
            }
            else
            {
                NetPacket packet;
                packet.read(out);
                switch(packet.type())
                {
                    case 1000: // connect
                    {
                        MemoryInputStream in(packet.data(), packet.size());
                        short port;
                        in.readShort(&port);
                        unsigned addressLength = 0;
                        in.readUnsigned(&addressLength);
                        char *addressBuffer = new char[addressLength];
                        in.readData(addressBuffer, addressLength);
                        std::string address(addressBuffer, addressLength);
                        delete[] addressBuffer;
                        m_parent->addAddress(address, port);
                        m_sock->sendData(out);
                        break;   
                    }
                    case 1001: // get all addresses
                    {
                        out.clear();
                        out.writeUnsigned(m_parent->addresses().size());
                        for(const auto address : m_parent->addresses())
                        {
                            out.writeShort(address.port);
                            out.writeUnsigned(address.ipAddress.length());
                            out.writeData(address.ipAddress.c_str(), address.ipAddress.length());
                        }
                        NetPacket res(packet.type(), out.buffer(), out.size());
                        out.clear();
                        res.write(out);
                        m_sock->sendData(out);
                        break;
                    }
                    case 1010:
                    {
                        MemoryInputStream in(packet.data(), packet.size());
                        short port;
                        in.readShort(&port);
                        unsigned addressLength = 0;
                        in.readUnsigned(&addressLength);
                        char *addressBuffer = new char[addressLength];
                        in.readData(addressBuffer, addressLength);
                        std::string address(addressBuffer, addressLength);
                        delete[] addressBuffer;
                        m_parent->removeAddress(address, port);
                        break;
                    }
                }
            }
        }
    });
}

void AddressClient::stop()
{
    m_isRunning = false;
    m_sock->disconnect();
}
