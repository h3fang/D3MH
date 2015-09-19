#include "memoryreader.h"

#include <stdio.h>
#include "helper.h"

MemoryReader::MemoryReader(const wchar_t *name):
    process(NULL),
    process_id(0)
{
    fprintf(stderr, "AdjustDebugPrivilege() %s\n", AdjustDebugPrivilege() ? "Succeeded" : "Failed");

    if(name){
        process_id = GetProcessIdByName(name);

        if (process_id == 0) {
            return;
        }

        process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);
    }
}

MemoryReader::~MemoryReader()
{
    if(process){
        CloseHandle(process);
    }
}

MemoryReader *MemoryReader::instance()
{
    static MemoryReader instance(L"Diablo III.exe");
    return &instance;
}

bool MemoryReader::read(void *buffer, void *src, unsigned int size)
{
    if(!process || size == 0){
        return false;
    }

    unsigned long bytesRead = 0;

    if(!ReadProcessMemory(process, src, buffer, size, &bytesRead) || bytesRead != size){
        return false;
    }

    return true;
}

