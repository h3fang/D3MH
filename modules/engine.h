#ifndef D3MH_ENGINE_H
#define D3MH_ENGINE_H

#include <vector>

#include <utils/precisetimer.h>

#include "datatypes/structs.h"
#include "helpers.h"
#include "navmesh.h"

namespace D3 {

// NOTE:offset
// 2.5.0.44247
enum Addr
{
    Addr_SnoGroupByCode = 0x02059EA0,
    Addr_SnoGroups = 0x02059FB8,
    Addr_ObjectManager = 0x0205C0F0,
    Addr_LocalData = 0x0205D3A0,
    Addr_LevelArea = 0x01FEABE0,
    Addr_LevelAreaName = 0x01FEABE8,
    Addr_MapActId = 0x01FEAED0,
    Addr_ContainerManager = 0x02124644,
    Addr_ApplicationLoopCount = 0x0205C16C,
    Addr_AttributeDescriptors = 0x020B3A60,
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
