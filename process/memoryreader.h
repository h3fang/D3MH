#ifndef MEMORYREADER_H
#define MEMORYREADER_H

#include <windows.h>

class MemoryReader
{
public:
    ~MemoryReader();

    static MemoryReader *instance();

    bool read(void *buffer, void *src, unsigned int size);

private:
    MemoryReader(const wchar_t *name);
    MemoryReader(MemoryReader&) = delete;
    MemoryReader& operator=(const MemoryReader &) = delete;

private:
    HANDLE process;
    DWORD process_id;
};

#endif // MEMORYREADER_H
