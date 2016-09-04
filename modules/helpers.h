#ifndef D3MH_HELPERS_H
#define D3MH_HELPERS_H

#include <cstdio>
#include <cmath>
#include <vector>

#include <utils/precisetimer.h>

#include "datatypes/structs.h"
#include "utils/memoryreader.h"

namespace D3 {

template<class T>
T distance(T x1, T y1, T x2, T y2) {
    return std::sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

template<class T>
T distance(T x1, T y1, T z1, T x2, T y2, T z2) {
    return std::sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2));
}

bool isMonster(const ActorCommonData& acd);
bool isTreasureGoblin(const ActorCommonData& acd);
bool isShrine(const ActorCommonData& acd);
bool isPoolOfReflection(const ActorCommonData& acd);

template<class T>
std::vector<T> enumerate_container(const Container<T>& c)
{
    std::vector<T> r;

    if (short(c.x108_MaxIndex) < 0 || c.x11C_PtrItems == 0) {
        return r;
    }

    if (sizeof(T) != c.x104_ItemSize) {
        fprintf(stderr, "sizeof(T) and c.x104_ItemSize doesn't match in enumerate_container()\n");
        return r;
    }

    r.resize(short(c.x108_MaxIndex));

    if (!MemoryReader::instance()->read(r.data(), (void*)c.x11C_PtrItems, c.x104_ItemSize * short(c.x108_MaxIndex))) {
        fprintf(stderr, "Failed to read memory in enumerate_container()\n");
        r.clear();
    }

    return r;
}

template<class T>
std::vector<T> enumerate_expandable_container(const ExpandableContainer<T>& c)
{
    std::vector<T> r;

    if (short(c.x108_MaxIndex) < 0) {
        return r;
    }

    if (sizeof(T) != c.x104_ItemSize) {
        fprintf(stderr, "sizeof(T) %d and c.x104_ItemSize %d doesn't match in enumerate_expandable_container()\n",
               sizeof(T), c.x104_ItemSize);
        return r;
    }

    int blockSize = 1 << c.x164_Bits;
    int blockCount = short(c.x108_MaxIndex) / blockSize;

    if (short(c.x108_MaxIndex) % blockSize != 0) {
        blockCount += 1;
    }

    std::vector<uint> blockPointers(blockCount);

    if (!MemoryReader::instance()->read(blockPointers.data(), (void*)c.x120_Allocation, blockCount * sizeof(uint))) {
        fprintf(stderr, "Failed to read memory in enumerate_expandable_container()\n");
        return r;
    }

    r.resize(short(c.x108_MaxIndex));

    for (int i = 0; i <= short(c.x108_MaxIndex); i+=blockSize)
    {
        uint itemAddress = blockPointers[i / blockSize] + c.x104_ItemSize * (i % blockSize);

        int n = blockSize;
        if ((i + blockSize) > short(c.x108_MaxIndex)) {
            n = short(c.x108_MaxIndex) - i;
        }

        if (!MemoryReader::instance()->read(&(r[i]), (void*)itemAddress, c.x104_ItemSize * n)) {
            fprintf(stderr, "Failed to read memory in enumerate_expandable_container()\n");
            r.clear();
            return r;
        }
    }

    return r;
}

template<class T>
bool getSerializedData(std::vector<T> &out, DataPtr2 ptr, uint dwBase)
{
    //Real data start
    uint dwDataStart = dwBase + ptr.file_offset;

    if (ptr.size == 0) {
        fprintf(stderr, "getSerializedData() ptr.file_offset %d , ptr.size %d\n",
                ptr.file_offset, ptr.size);
        return false;
    }

    out.resize(ptr.size/sizeof(T)+1);

    //Read records
    if (!MemoryReader::instance()->read(out.data(), (void*)dwDataStart, ptr.size)) {
        fprintf(stderr, "Failed to read memory in getSerializedData() ptr.file_offset %d , ptr.size %d\n",
                ptr.file_offset, ptr.size);
        out.clear();
        return false;
    }

    return true;
}

template<class T>
bool getSerializedDataFromFile(std::vector<T> &out, DataPtr2 ptr, uint dwBase)
{
    //Real data start
    uint dwDataStart = dwBase + ptr.file_offset;

    if (ptr.size == 0) {
        fprintf(stderr, "getSerializedDataFromFile() ptr.file_offset %d , ptr.size %d\n",
                ptr.file_offset, ptr.size);
        return false;
    }

    out.resize(ptr.size/sizeof(T)+1);

    //Read records
    memcpy(out.data(), (void*)dwDataStart, ptr.size);

    return true;
}

}

#endif // D3MH_HELPERS_H
