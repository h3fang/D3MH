#ifndef D3MH_ENGINE_H
#define D3MH_ENGINE_H

#include <vector>

#include "structs.h"

class MemoryReader;

enum Addr
{
    Addr_SnoGroupInitializers = 0x01B71ABC - 4,
    Addr_SnoGroupByCode = 0x01DCD028,
    Addr_SnoGroups = 0x01DCD144,
    Addr_SnoGroupSearch = 0x01E201FC,
    Addr_SnoFilesAsync = 0x01E20200,
    Addr_ObjectManager = 0x01DCF22C,
    Addr_ObjectManagerPristine = 0x01DCF230,
    Addr_MessageDescriptor = 0x01E8384C,
    Addr_MapActId = 0x01BBB328,
    Addr_LocalData = 0x01DD04D0,
    Addr_LevelArea = 0x01D27758,
    Addr_LevelAreaName = 0x01D27788,
    Addr_GameplayPreferences = 0x01BA1F74,
    Addr_ContainerManager = 0x01E8454C,
    Addr_BuffManager = 0x01DB4970,
    Addr_ApplicationLoopCount = 0x01DCF2A0,
    Addr_AttributeDescriptors = 0x01B76AC8,
    Addr_VideoPreferences = 0x01BA1A30,
    Addr_ChatPreferences = 0x01BA2004,
    Addr_SoundPreferences = 0x01BA1AC4,
    Addr_SocialPreferences = 0x01BA1FD4,
    Addr_UIHandlers = 0x01B684D0,
    Addr_UIReferences = 0x01BBB8D8,
    Addr_SnoIdToEntityId = 0x01DE199C,
    Addr_TrickleManager = 0x01D8BF68,
    Addr_PtrSnoFiles = 0x01DD15F0,
};

class Engine
{
public:
    D3::LocalData localData;
    int ApplicationLoopCount;

public:
    ~Engine();
    static Engine *getInstance();
    void update();

private:
    Engine();
    Engine(Engine&) = delete;
    Engine& operator=(const Engine &) = delete;

    MemoryReader *memoryReader;
    D3::NavMesh *navMesh;
};

#endif // D3MH_ENGINE_H
