#include "memoryreader.h"

#include <stdio.h>
#include "helper.h"

MemoryReader::MemoryReader(const wchar_t *name):
    process(NULL)
{
    fprintf(stderr, "AdjustDebugPrivilege() %s\n", AdjustDebugPrivilege() ? "Successful" : "Failed");

    if(name){
        process = GetProcessByName(name);
    }
}

MemoryReader::~MemoryReader()
{
    if(process){
        CloseHandle(process);
    }
}

bool MemoryReader::read(void *dest, void *src, unsigned int size)
{
    if(!process || size == 0){
        return false;
    }

    unsigned long bytesRead = 0;

    if(!ReadProcessMemory(process, src, dest, size, &bytesRead) || bytesRead != size){
        return false;
    }

    return true;
}

