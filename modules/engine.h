#ifndef D3MH_ENGINE_H
#define D3MH_ENGINE_H

#include <vector>

#include <utils/precisetimer.h>

#include "datatypes/structs.h"
#include "helpers.h"
#include "navmesh.h"

namespace D3 {

// NOTE:offset
// 2.4.3.43048
enum Addr
{
    Addr_SnoGroupByCode = 0x01C12C90,
    Addr_SnoGroups = 0x01C12C8C,
    Addr_ObjectManager = 0x01C12E98,
    Addr_ObjectManagerPristine = 0x01C12E9C,
    Addr_LocalData = 0x01C14148,
    Addr_LevelArea = 0x01B9D68C,
    Addr_LevelAreaName = 0x01B9D6BC,
    Addr_MapActId = 0x01B9DA00,
    Addr_ContainerManager = 0x01CD5A7C,
    Addr_ApplicationLoopCount = 0x01C12F14,
    Addr_AttributeDescriptors = 0x01C65660,
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
