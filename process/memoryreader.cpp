#include "memoryreader.h"

#include <stdio.h>
#include "helper.h"

MemoryReader::MemoryReader(const wchar_t *name, int size):
    process(NULL),
    process_id(0)
{
    fprintf(stderr, "AdjustDebugPrivilege() %s\n", AdjustDebugPrivilege() ? "Succeeded" : "Failed");

    if (!name) {
        return;
    }

    auto p_decoded = decode_string(name, size);

    process_id = GetProcessIdByName(p_decoded.data());

    if (process_id == 0) {
        return;
    }

    process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);
}

DWORD MemoryReader::getProcessId() const
{
    return process_id;
}

bool MemoryReader::checkHandle()
{
    if (process && WaitForSingleObject(process, 0) == WAIT_TIMEOUT) {
        return true;
    }
    else {
        return false;
    }
}

MemoryReader::~MemoryReader()
{
    if(process){
        CloseHandle(process);
        process = NULL;
    }
}

MemoryReader *MemoryReader::instance()
{
    static wchar_t n[] = {0xfe5f, 0xfe72, 0xfe7a, 0xfe79, 0xfe77, 0xfe74, 0xfe3b, 0xfe52, 0xfe52, 0xfe52, 0xfe35, 0xfe7e, 0xfe63, 0xfe7e, 0xfe1b};
    static MemoryReader instance(n, sizeof(n)/sizeof(wchar_t));
    return &instance;
}

bool MemoryReader::read(void *buffer, void *src, unsigned int size)
{
    if(!process || size == 0){
        return false;
    }

    unsigned long bytesRead = 0;

    if(!ReadProcessMemory(process, src, buffer, size, &bytesRead) || bytesRead != size){
        fprintf(stderr, "Error reading process memory, addr [%#lx]\n", (DWORD)src);
        return false;
    }

    return true;
}

