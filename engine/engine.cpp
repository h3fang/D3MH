#include "engine.h"

Engine *Engine::pInstance = nullptr;

Engine *Engine::createEngine()
{
    if(pInstance == nullptr)
    {
        pInstance = new Engine();
    }
    return pInstance;
}

Engine::Engine()
{
}

