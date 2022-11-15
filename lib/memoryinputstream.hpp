#pragma once

class MemoryInputStream
{
private:
    char *m_buffer{nullptr};

    unsigned m_size{0};

    unsigned m_offset{0};

public:
    MemoryInputStream();

    MemoryInputStream(const char *buffer, unsigned size);

    MemoryInputStream(const MemoryInputStream &other) = delete;

    MemoryInputStream operator=(const MemoryInputStream &other) = delete;

    ~MemoryInputStream();

    void setBuffer(const char *buffer, unsigned size);

    void readInt(int *dest);

    void readUnsigned(unsigned *dest);

    void readShort(short *dest);

    void readData(char *dest, unsigned size);
};