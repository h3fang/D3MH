#ifndef ENGINE_H
#define ENGINE_H

#include "structs.h"

class MemoryReader;

class Engine
{
public:
    D3::LocalData localData;
    D3::ObjectManager objectManager;
    int ApplicationLoopCount;

public:
    ~Engine();
    static Engine *getInstance();
    static Engine *pInstance;

    void update();

private:
    Engine();
    Engine(Engine&) = delete;
    Engine& operator=(const Engine &) = delete;

    MemoryReader *memoryReader;
};

#endif // ENGINE_H
