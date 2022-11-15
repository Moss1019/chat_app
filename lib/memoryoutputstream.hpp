#pragma once

class MemoryOutputStream
{
private:
    char *m_buffer{nullptr};

    unsigned m_size{0};

    unsigned m_capacity{32};

    void reallocate(unsigned newSize);

public:
    MemoryOutputStream();

    MemoryOutputStream(const MemoryOutputStream &other) = delete;

    MemoryOutputStream operator=(const MemoryOutputStream &other) = delete;

    ~MemoryOutputStream();

    unsigned size() const;

    const char *buffer() const;

    void writeInt(int data);

    void writeUnsigned(unsigned data);

    void writeShort(short data);

    void writeData(const char *data, unsigned size);

    void clear();
};