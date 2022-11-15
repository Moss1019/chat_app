#include "addressserver.hpp"

#include <iostream>

AddressServer::AddressServer()
{
    
}

AddressServer::~AddressServer()
{
    stop();
    if(m_worker != nullptr)
    {
        m_worker->join();
        delete m_worker;
        m_worker = nullptr;
    }
    for(auto i = m_clients.begin(); i != m_clients.end(); ++i)
    {
        delete *i;
    }
    if(m_sock != nullptr)
    {
        m_sock->disconnect();
        delete m_sock;
        m_sock = nullptr;
    }
}

bool AddressServer::inError() const
{
    return m_sock->inError();
}

const std::string &AddressServer::error() const
{
    return m_sock->error();
}

const std::vector<AddressInfo> &AddressServer::addresses() const
{
    return m_addresses;
}

void AddressServer::start(const SocketAddress &local)
{
    m_sock = new TcpSocket;
    m_isRunning = true;
    m_sock->bindAndListen(local);
    auto work = [this, &local]() 
    {
        while(m_isRunning)
        {
            socktype newConnection = m_sock->acceptConnection();
            if(newConnection != TcpSocket::Status::closed)
            {
                AddressClient *newClient = new AddressClient(newConnection, this);
                newClient->start();
                m_clients.push_back(newClient);
            }
            if(m_isRunning)
            {
                for(auto i = m_clients.begin(); i != m_clients.end(); ++i)
                {
                    if(!(*i)->isConnected())
                    {
                        delete *i;
                        m_clients.erase(i);
                    }
                }
            }
        }
    };
    m_worker = new std::thread(work);
}

void AddressServer::stop()
{
    m_isRunning = false;
    m_sock->disconnect();
}

void AddressServer::addAddress(const std::string &ipAddress, short port)
{
    m_addresses.push_back(AddressInfo{});
    m_addresses.back().ipAddress = ipAddress;
    m_addresses.back().port = port;
}

void AddressServer::removeAddress(const std::string &ipAddress, short port)
{
    auto i = m_addresses.begin();
    while(i != m_addresses.end() && ((*i).ipAddress != ipAddress && (*i).port != port))
    {
        ++i;
    }
    if(i != m_addresses.end())
    {
        m_addresses.erase(i);
    }
}
