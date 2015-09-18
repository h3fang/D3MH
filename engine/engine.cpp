#include "engine.h"

#include "memoryreader.h"

Engine *Engine::pInstance = nullptr;

Engine::~Engine()
{
    delete memoryReader;
}

Engine *Engine::getInstance()
{
    if(pInstance == nullptr)
    {
        pInstance = new Engine();
    }
    return pInstance;
}

void Engine::update()
{
//    memoryReader->read(&objectManager, (void *)0x01D895DC, sizeof(D3::ObjectManager));
    memoryReader->read(&localData, (void *)0x01D8A880, sizeof(D3::LocalData));
    memoryReader->read(&ApplicationLoopCount, (void *)0x01D89650, sizeof(int));
}

Engine::Engine():
    memoryReader(new MemoryReader(L"Diablo III.exe"))
{
}

