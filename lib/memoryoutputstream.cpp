#include "memoryoutputstream.hpp"

#include <cstring>
#include <algorithm>

void MemoryOutputStream::reallocate(unsigned newSize) 
{
    char *newBuffer = new char[newSize];
    if(m_buffer != nullptr)
    {
        std::memcpy(newBuffer, m_buffer, m_size);
        delete[] m_buffer;
    }
    m_buffer = newBuffer;
    m_capacity = newSize;
}

MemoryOutputStream::MemoryOutputStream()
{
    reallocate(m_capacity);
}

MemoryOutputStream::~MemoryOutputStream()
{
    if(m_buffer != nullptr)
    {
        delete[] m_buffer;
        m_buffer = nullptr;
    }
}

unsigned MemoryOutputStream::size() const 
{
    return m_size;
}

const char *MemoryOutputStream::buffer() const 
{
    return m_buffer;
}

void MemoryOutputStream::writeInt(int data)
{
    unsigned newSize = m_size + sizeof(int);
    if(newSize > m_capacity) 
    {
        reallocate(std::max(m_capacity * 2, newSize));
    }
    std::memcpy(m_buffer + m_size, &data, sizeof(int));
    m_size = newSize;
}

void MemoryOutputStream::writeUnsigned(unsigned data)
{
    unsigned newSize = m_size + sizeof(unsigned);
    if(newSize > m_capacity) 
    {
        reallocate(std::max(m_capacity * 2, newSize));
    }
    std::memcpy(m_buffer + m_size, &data, sizeof(unsigned));
    m_size = newSize;
}

void MemoryOutputStream::writeShort(short data)
{
    unsigned newSize = m_size + sizeof(short);
    if(newSize > m_capacity) 
    {
        reallocate(std::max(m_capacity * 2, newSize));
    }
    std::memcpy(m_buffer + m_size, &data, sizeof(short));
    m_size = newSize;
}

void MemoryOutputStream::writeData(const char *data, unsigned size)
{
    unsigned newSize = m_size + size;
    if(newSize > m_capacity) 
    {
        reallocate(std::max(m_capacity * 2, newSize));
    }
    std::memcpy(m_buffer + m_size, data, size);
    m_size = newSize;
}

void MemoryOutputStream::clear()
{
    m_size = 0;
}
