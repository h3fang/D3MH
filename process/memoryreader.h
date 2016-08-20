#ifndef UTILITIES_MEMORYREADER_H
#define UTILITIES_MEMORYREADER_H

#include <windows.h>

class MemoryReader
{
public:
    ~MemoryReader();

    static MemoryReader* instance();

    bool read(void *buffer, void *src, unsigned int size);

    DWORD getProcessId() const;
    bool checkHandle();

private:
    MemoryReader(const wchar_t *name, int size);
    MemoryReader(MemoryReader&) = delete;
    MemoryReader& operator=(const MemoryReader &) = delete;

private:
    HANDLE process;
    DWORD process_id;
};

#endif // UTILITIES_MEMORYREADER_H
