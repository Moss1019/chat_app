#include "memoryinputstream.hpp"

#include <cstring>

MemoryInputStream::MemoryInputStream()
{

}

MemoryInputStream::MemoryInputStream(const char *buffer, unsigned size)
{
    setBuffer(buffer, size);
}

MemoryInputStream::~MemoryInputStream()
{
    if(m_buffer != nullptr)
    {
        delete[] m_buffer;
        m_buffer = nullptr;
    }
}

void MemoryInputStream::setBuffer(const char *buffer, unsigned size)
{
    m_size = size;
    if(m_buffer != nullptr)
    {
        delete[] m_buffer;
    }
    m_buffer = new char[size];
    std::memcpy(m_buffer, buffer, size);
}

void MemoryInputStream::readInt(int *dest)
{
    std::memcpy(dest, m_buffer + m_offset, sizeof(int));
    m_offset += sizeof(int);
}

void MemoryInputStream::readUnsigned(unsigned *dest)
{
    std::memcpy(dest, m_buffer + m_offset, sizeof(unsigned));
    m_offset += sizeof(unsigned);
}

void MemoryInputStream::readShort(short *dest)
{
    std::memcpy(dest, m_buffer + m_offset, sizeof(short));
    m_offset += sizeof(short);
}

void MemoryInputStream::readData(char *dest, unsigned size)
{
    std::memcpy(dest, m_buffer + m_offset, size);
    m_offset += size;
}
