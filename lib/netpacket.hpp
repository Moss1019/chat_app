#pragma once

#include "memoryoutputstream.hpp"

class NetPacket
{
private:
    int m_type;

    unsigned m_size{0};

    char *m_data{nullptr};

public:
    NetPacket();

    NetPacket(int type, const char *data, unsigned size);

    NetPacket(const NetPacket &other) = delete;

    NetPacket operator=(const NetPacket &other) = delete;

    ~NetPacket();

    int type() const;

    unsigned size() const;

    const char *data() const;

    void read(const MemoryOutputStream &stream);

    void write(MemoryOutputStream &stream);
};
