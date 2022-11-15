#include <thread>
#include <vector>
#include <iostream>

#include "chatclient.hpp"
#include "addressinfo.hpp"
#include "addressclient.hpp"

int main(int argc, char **argv) 
{
    short port = 8080;
    if(argc > 1)
    {
        port = static_cast<short>(atoi(argv[1]));
    }
    short rmtPort = 8081;
    if(argc > 2) 
    {
        rmtPort = static_cast<short>(atoi(argv[2]));
    }

    SocketAddress server("192.168.1.182", rmtPort);
    SocketAddress local("192.168.1.37", port);
    AddressClient client(local, server);
    ChatClient chatClient(local, [](const SocketAddress &remote, const std::string &msg)
    {
        std::cout << msg << "\n";
    });
    chatClient.start();

    bool isRunning = true;
    while(isRunning)
    {
        std::cout << "-q quit\n-m send msg\n";
        std::string inp;
        std::cin >> inp;
        if(inp == "-q")
        {
            isRunning = false;
        }
        else if(inp == "-m")
        {
            std::vector<AddressInfo> addresses{};
            client.getAddresses(addresses);
            if(addresses.empty())
            {
                continue;
            }
            std::cout << addresses.size() << " send msg to\n";
            auto i = 0;
            for(const auto &addr : addresses)
            {
                std::cout << ++i << ". " << addr.ipAddress << ":" << addr.port << "\n";
            }
            std::cin >> i;
            --i;
            if(i >= addresses.size())
            {
                continue;
            }
            std::cout << "msg...\n";
            std::cin >> inp;
            SocketAddress remote(addresses[i].ipAddress, addresses[i].port);
            chatClient.sendMsg(inp, remote);
        }
    }

    client.disconnect();
    chatClient.stop();

    return 0;
}