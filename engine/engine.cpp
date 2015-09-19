#include "engine.h"

#include "process/memoryreader.h"

Engine::~Engine()
{
}

Engine *Engine::getInstance()
{
    static Engine instance;
    return &instance;
}

D3::SnoGroupManager *Engine::getSnoGroupManagerById(D3::SnoGroupId id)
{
    D3::SnoGroupManager *sgmAddr = nullptr;

    if ((unsigned int)id > 70) {return sgmAddr;}

    memoryReader->read(&sgmAddr, (void*)(Addr_SnoGroupByCode+id*4), sizeof(sgmAddr));
    return sgmAddr;
}

void Engine::update()
{
    memoryReader->read(&localData, (void *)Addr_LocalData, sizeof(D3::LocalData));
    memoryReader->read(&ApplicationLoopCount, (void *)Addr_ApplicationLoopCount, sizeof(int));
}

Engine::Engine():
    memoryReader(MemoryReader::instance())
{
}

