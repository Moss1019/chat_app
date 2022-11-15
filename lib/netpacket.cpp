#include "netpacket.hpp"

#include <cstring>

#include "memoryinputstream.hpp"

NetPacket::NetPacket()
{

}

NetPacket::NetPacket(int type, const char *data, unsigned size)
    :m_type{type}, m_size{size}
{
    if(size > 0 && data != nullptr)
    {
        m_data = new char[m_size];
        std::memcpy(m_data, data, m_size);
    }
}

NetPacket::~NetPacket()
{
    if(m_data != nullptr)
    {
        delete[] m_data;
    }
}

int NetPacket::type() const
{
    return m_type;
}

unsigned NetPacket::size() const
{
    return m_size;
}

const char *NetPacket::data() const
{
    return m_data;
}

void NetPacket::read(const MemoryOutputStream &stream)
{   
    MemoryInputStream in(stream.buffer(), stream.size());
    in.readInt(&m_type);
    in.readUnsigned(&m_size);
    if(m_size > 0)
    {
        m_data = new char[m_size];
        in.readData(m_data, m_size);
    }
}

void NetPacket::write(MemoryOutputStream &stream)
{
    stream.writeInt(m_type);
    stream.writeUnsigned(m_size);
    if(m_size > 0)
    {
        stream.writeData(m_data, m_size);
    }
}
