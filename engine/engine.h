#ifndef D3MH_ENGINE_H
#define D3MH_ENGINE_H

#include <vector>

#include "structs.h"

class MemoryReader;

enum Addr
{
//    Addr_SnoGroupInitializers = 0x01C36944 - 4,
    Addr_SnoGroupByCode = 0x01EA73B0,
//    Addr_SnoGroups = 0x01DCD164,
//    Addr_SnoGroupSearch = 0x01E2021C,
//    Addr_SnoFilesAsync = 0x01E20220,
    Addr_ObjectManager = 0x01EA60D4,
    Addr_ObjectManagerPristine = 0x01EA60D8,
    Addr_MessageDescriptor = 0x01F6842C,
    Addr_MapActId = 0x01E30EE0,
    Addr_LocalData = 0x01EA7378,
    Addr_LevelArea = 0x01E30B40,
//    Addr_LevelAreaName = 0x01D277A8,
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
    D3::LocalData localData;
    unsigned int ApplicationLoopCount;
    D3::NavMesh *navMesh;

public:
    ~Engine();
    static Engine *getInstance();
    void update();

private:
    Engine();
    Engine(Engine&) = delete;
    Engine& operator=(const Engine &) = delete;

    MemoryReader *memoryReader;
};

#endif // D3MH_ENGINE_H
