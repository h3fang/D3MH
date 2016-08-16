#include "engine.h"

#include <fstream>

#include "process/pointer.h"

namespace D3 {

Engine::Engine():
    navMesh(new NavMesh(this)),
    last_frame(0)
{
}

Engine::~Engine()
{
    delete navMesh;
}

bool Engine::update()
{
    if (!MemoryReader::instance()->checkHandle()) {
        return false;
    }

    if (!MemoryReader::instance()->read(&localData, (void *)Addr_LocalData, sizeof(LocalData))) { return false; }
    if (!MemoryReader::instance()->read(&ApplicationLoopCount, (void *)Addr_ApplicationLoopCount, sizeof(int))) { return false; }

    if (isInGame()) {
        if (isObjectManagerOnNewFrame()) {
            update_acds();

            if (nav_mesh_timer.start_or_elapsed() > 0.3) {
                navMesh->update();
                nav_mesh_timer.start();
            }
        }
    }
    else {
        acds.clear();
        navMesh->clear();
    }

    return true;
}

bool Engine::isInGame()
{
    return localData.x04_IsNotInGame != 0xCD && localData.x00_IsActorCreated == 1;
}

bool Engine::isObjectManagerOnNewFrame()
{
    auto frame = Pointer<uint>()(D3::Addr_ObjectManager, offsetof(D3::ObjectManager,x038_Counter_CurrentFrame));

    if (frame > last_frame) {
        last_frame = frame;
        return true;
    }
    else if (frame < last_frame) {
        last_frame = frame;
        return false;
    }
    else {
        return false;
    }
}

void Engine::update_acds()
{
    // NOTE:offset
    ExpandableContainer<ActorCommonData> c = Pointer<ExpandableContainer<ActorCommonData>>()(Addr_ObjectManager, 0x798 + 0x158, 0, 0);
    acds = enumerate_expandable_container(c);
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

