#include "engine.h"

#include <fstream>

#include "process/pointer.h"

namespace D3 {

Engine::Engine():
    ApplicationLoopCount(0),
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

    if (!MemoryReader::instance()->read(&ApplicationLoopCount, (void *)Addr_ApplicationLoopCount, sizeof(int))) { return false; }

    LocalData ld;
    if (!MemoryReader::instance()->read(&ld, (void *)Addr_LocalData, sizeof(LocalData))) { return false; }

    if (ld.x00_IsActorCreated > 1 || ld.x00_IsActorCreated < 0 || ld.x04_IsNotInGame > 1 || ld.x04_IsNotInGame < 0) {
        return false;
    }
    else {
        localData = ld;
    }

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
    switch (acd.x090_ActorSnoId) {
    // TODO:update goblin IDs
    case 0x00001760: // treasureGoblin_A
    case 0x00001761: // treasureGoblin_B
    case 0x00001763: // treasureGoblin_C
    case 0x00001764: // treasureGoblin_D
    case 0x00063C4F: // treasureGoblin_E
    case 0x00063D9D: // treasureGoblin_F
    case 0x0005F9A9: // treasureGoblin_G
    case 0x00064E69: // treasureGoblin_H
    case 0x00068A77: // treasureGoblin_I
    case 0x00068C69: // treasureGoblin_J
    case 0x0006E1B1: // treasureGoblin_K
        return true;

    default:
        return false;
    }
}

bool isShrine(const ActorCommonData& acd)
{
    switch (acd.x090_ActorSnoId) {
    // TODO:update shrine IDs
    case 0x00050BC7:
    case 0x00050BCA:
    case 0x00050BC8:
    case 0x00050BC9:
    case 0x00050BCB:
    case 0x0002AFCC:
    case 0x0002AFCA:
    case 0x0002AFCB:
    case 0x0002AFCD:
    case 0x0003F8FA:
    case 0x0003F8FB:
    case 0x00041C25:
    case 0x00026408:
    case 0x0006153E:
    case 0x000210D8:
        return true;

    default:
        return false;
    }
}

}

