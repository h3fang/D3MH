#include "engine.h"

#include <fstream>

#include "process/pointer.h"

namespace D3 {

Engine::~Engine()
{
    memoryReader->closeHandle();
    delete navMesh;
}

Engine *Engine::getInstance()
{
    static Engine instance;
    return &instance;
}

void Engine::update()
{
    if (!memoryReader->checkHandle()) {
        return;
    }

    if (!memoryReader->read(&localData, (void *)Addr_LocalData, sizeof(LocalData))) { return; }
    if (!memoryReader->read(&ApplicationLoopCount, (void *)Addr_ApplicationLoopCount, sizeof(int))) { return; }

    update_acds();

    if (isInGame()) {
        if (nav_mesh_timer.start_or_elapsed() > 0.3) {
            navMesh->update();
            nav_mesh_timer.start();
        }
    }
    else {
        navMesh->clear();
    }
}

bool Engine::isInGame()
{
    return localData.x04_IsNotInGame != 0xCD && localData.x00_IsActorCreated == 1;
}

void Engine::update_acds()
{
    // NOTE:offset
    ExpandableContainer<ActorCommonData> c = Pointer<ExpandableContainer<ActorCommonData>>()(Addr_ObjectManager, 0x798 + 0x158, 0, 0);
    acds = enumerate_expandable_container(c);
}

Engine::Engine():
    navMesh(new D3::NavMesh),
    memoryReader(MemoryReader::instance())
{
}

bool isTreasureGoblin(const ActorCommonData& acd)
{
    switch (acd.x098_MonsterSnoId) {
    // TODO:update goblin IDs
    case 0x00001760: // treasureGoblin_A
    case 0x00001761: // treasureGoblin_B
    case 0x00001763: // treasureGoblin_C
    case 0x00001764: // treasureGoblin_D
        return true;

    default:
        return false;
    }
}

}

