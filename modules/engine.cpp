#include "engine.h"

#include <fstream>

#include "utils/pointer.h"
#include "snocache.h"

namespace D3 {

Engine::Engine():
    ApplicationLoopCount(0),
    last_frame(0)
{
//    SnoCache sc("..\\Base\\Scene\\");
//    sc.build();
    navMesh = new NavMesh(this);
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
        navMesh->clearAll();
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
    ExpandableContainer<ActorCommonData> c = Pointer<ExpandableContainer<ActorCommonData>>()(Addr_ObjectManager, 0x7C8 + 0x158, 0, 0);
    acds = enumerate_expandable_container(c);
}

}

