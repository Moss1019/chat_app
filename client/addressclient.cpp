#include "addressclient.hpp"

#include <iostream>

#include "netpacket.hpp"

AddressClient::AddressClient(const SocketAddress &local, const SocketAddress &server)
    :m_ipAddress{local.ipAddress()}, m_port{local.port()}
{
    m_sock = new TcpSocket();
    m_sock->connectTo(server);
    m_isConnected = true;
    MemoryOutputStream stream;
    stream.writeShort(m_port);
    stream.writeUnsigned(static_cast<unsigned>(m_ipAddress.length()));
    stream.writeData(m_ipAddress.c_str(), m_ipAddress.length());
    NetPacket packet(1000, stream.buffer(), stream.size());
    stream.clear();
    packet.write(stream);
    m_sock->sendData(stream);
    stream.clear();
    m_sock->recvData(stream);
}

AddressClient::~AddressClient()
{
    disconnect();
    if(m_sock != nullptr)
    {
        delete m_sock;
        m_sock = nullptr;
    }
}

bool AddressClient::inError() const
{   
    return m_sock->inError();
}

const std::string &AddressClient::error() const
{
    return m_sock->error();
}

void AddressClient::getAddresses(std::vector<AddressInfo> &addresses) const
{
    NetPacket packet(1001, nullptr, 0);
    MemoryOutputStream stream;
    packet.write(stream);
    int bytes = m_sock->sendData(stream);
    stream.clear();
    bytes = m_sock->recvData(stream);
    packet.read(stream);
    MemoryInputStream in(packet.data(), packet.size());
    addresses.clear();
    unsigned numAddresses = 0;
    in.readUnsigned(&numAddresses);
    for(auto i = 0; i < numAddresses; ++i)
    {
        short port = 0;
        in.readShort(&port);
        unsigned addressLength = 0;
        in.readUnsigned(&addressLength);
        char *addressBuffer = new char[addressLength];
        in.readData(addressBuffer, addressLength);
        std::string ipAddress(addressBuffer, addressLength);
        delete[] addressBuffer;
        if(!(m_port == port && m_ipAddress == ipAddress))
        {
            addresses.push_back(AddressInfo{});
            addresses.back().ipAddress = ipAddress;
            addresses.back().port = port;
        }
    }
}

void AddressClient::disconnect() 
{
    if(!m_isConnected)
    {
        return;
    }
    MemoryOutputStream stream;
    stream.writeShort(m_port);
    stream.writeUnsigned(static_cast<unsigned>(m_ipAddress.length()));
    stream.writeData(m_ipAddress.c_str(), m_ipAddress.length());
    NetPacket packet(1010, stream.buffer(), stream.size());
    stream.clear();
    packet.write(stream);
    m_sock->sendData(stream);
    stream.clear();
    m_sock->disconnect();
    m_isConnected = false;
}