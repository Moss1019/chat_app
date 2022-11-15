cmake_minimum_required(VERSION 3.20)
project(chat_client)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)

include_directories(BEFORE ../lib)

add_library(chat_lib
    ../lib/udpsocket.cpp
    ../lib/tcpsocket.cpp
    ../lib/netpacket.cpp
    ../lib/socketaddress.cpp
    ../lib/memoryinputstream.cpp
    ../lib/memoryoutputstream.cpp
)
add_executable(chat_client 
    main.cpp
    chatclient.cpp
    addressclient.cpp
    ) 

target_link_libraries(chat_client PRIVATE pthread chat_lib)
