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
    navMesh->update();
}

Engine::Engine():
    memoryReader(MemoryReader::instance()),
    navMesh(new D3::NavMesh)
{
}

