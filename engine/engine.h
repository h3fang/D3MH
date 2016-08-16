#ifndef D3MH_ENGINE_H
#define D3MH_ENGINE_H

#include <vector>

#include <utils/precisetimer.h>

#include "structs.h"
#include "process/memoryreader.h"

namespace D3 {

// NOTE:offset
enum Addr
{
//    Addr_SnoGroupInitializers = 0x01C36944 - 4,
    Addr_SnoGroupByCode = 0x01EA73B0,
    Addr_SnoGroups = 0x01EA3FE4,
//    Addr_SnoGroupSearch = 0x01E2021C,
//    Addr_SnoFilesAsync = 0x01E20220,
    Addr_ObjectManager = 0x01EA60D4,
    Addr_ObjectManagerPristine = 0x01EA60D8,
    Addr_MessageDescriptor = 0x01F6842C,
    Addr_MapActId = 0x01E30EE0,
    Addr_LocalData = 0x01EA7378,
    Addr_LevelArea = 0x01E30B40,
    Addr_LevelAreaName = 0x01E30B70,
//    Addr_GameplayPreferences = 0x01BA1F94,
    Addr_ContainerManager = 0x01F67720,
//    Addr_BuffManager = 0x01DB4990,
    Addr_ApplicationLoopCount = 0x01EA6148,
    Addr_AttributeDescriptors = 0x01EBF028,
//    Addr_VideoPreferences = 0x01BA1A50,
//    Addr_ChatPreferences = 0x01BA2024,
//    Addr_SoundPreferences = 0x01BA1AE4,
//    Addr_SocialPreferences = 0x01BA1FF4,
//    Addr_UIHandlers = 0x01B684D0,
//    Addr_UIReferences = 0x01BBB8F8,
//    Addr_SnoIdToEntityId = 0x00000000,
//    Addr_TrickleManager = 0x01D8BF88,
//    Addr_PtrSnoFiles = 0x01DD1610,
};

class Engine
{
public:
    LocalData localData;
    unsigned int ApplicationLoopCount;
    NavMesh *navMesh;
    std::vector<ActorCommonData> acds;

public:
    Engine();
    ~Engine();

    void update();

    bool isInGame();
    void update_acds();

private:
    Engine(Engine&) = delete;
    Engine& operator=(const Engine &) = delete;

    PreciseTimer nav_mesh_timer;
};

bool isTreasureGoblin(const ActorCommonData& acd);

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
        fprintf(stderr, "NavMesh::getSerializedRecords() ptr.file_offset %d , ptr.size %d\n", ptr.file_offset, ptr.size);
        return false;
    }

    out.resize(ptr.size/sizeof(T)+1);

    //Read records
    if (!MemoryReader::instance()->read(out.data(), (void*)dwDataStart, ptr.size)) {
        fprintf(stderr, "Failed to read memory in NavMesh::getSerializedRecords() ptr.file_offset %d , ptr.size %d\n", ptr.file_offset, ptr.size);
        out.clear();
        return false;
    }

    return true;
}

}

#endif // D3MH_ENGINE_H
