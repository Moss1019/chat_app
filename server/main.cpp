#include <iostream>

#include "types.hpp"
#include "udpsocket.hpp"
#include "netpacket.hpp"
#include "addressserver.hpp"

int main() 
{
    SocketAddress local("192.168.1.182", 8081);
    
    AddressServer server;
    server.start(local);

    bool isRunning = true;
    while(isRunning)
    {
        std::string inp;
        std::cin >> inp;
        if(inp == "-q")
        {
            isRunning = false;
        }
        else if (inp == "-l") 
        {
            for(const auto addr : server.addresses()) 
            {
                std::cout << addr.ipAddress << ":" << addr.port << "\n";
            }
        }
    }

    server.stop();
    return 0;
}