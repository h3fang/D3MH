#include "engine.h"

#include "process/memoryreader.h"

Engine::~Engine()
{
    delete navMesh;
}

Engine *Engine::getInstance()
{
    static Engine instance;
    return &instance;
}

void Engine::update()
{
    memoryReader->read(&localData, (void *)Addr_LocalData, sizeof(D3::LocalData));
    memoryReader->read(&ApplicationLoopCount, (void *)Addr_ApplicationLoopCount, sizeof(int));

    if (localData.x04_IsNotInGame == 0) {
        navMesh->update();
    }
}

Engine::Engine():
    navMesh(new D3::NavMesh),
    memoryReader(MemoryReader::instance())
{
}

