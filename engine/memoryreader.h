#ifndef MEMORYREADER_H
#define MEMORYREADER_H

#include <windows.h>

class MemoryReader
{
public:
    MemoryReader(const wchar_t *name);
    ~MemoryReader();

    bool read(void *dest, void *src, unsigned int size);

private:
    HANDLE process;
};

#endif // MEMORYREADER_H
