#ifndef D3MH_ENGINE_H
#define D3MH_ENGINE_H

#include <vector>

#include <utils/precisetimer.h>

#include "datatypes/structs.h"
#include "helpers.h"
#include "navmesh.h"

namespace D3 {

// NOTE:offset
enum Addr
{
    Addr_SnoGroupByCode = 0x01C13C90,
    Addr_SnoGroups = 0x01C13C8C,
    Addr_ObjectManager = 0x01C13E98,
    Addr_ObjectManagerPristine = 0x01C13E9C,
    Addr_LocalData = 0x01C15148,
    Addr_LevelArea = 0x01B9E68C,
    Addr_LevelAreaName = 0x01B9E6BC,
    Addr_MapActId = 0x01B9EA00,
    Addr_ContainerManager = 0x01CD6A7C,
    Addr_ApplicationLoopCount = 0x01C13F14,
    Addr_AttributeDescriptors = 0x01C66660,
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

    bool update();

    bool isInGame();
    bool isObjectManagerOnNewFrame();
    void update_acds();

private:
    Engine(Engine&) = delete;
    Engine& operator=(const Engine &) = delete;

    PreciseTimer nav_mesh_timer;
    uint last_frame;
};

}

#endif // D3MH_ENGINE_H
