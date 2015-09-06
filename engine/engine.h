#ifndef ENGINE_H
#define ENGINE_H

class Engine
{
public:
    static Engine* createEngine();
    static Engine *pInstance;

private:
    Engine();
    Engine(Engine&) = delete;
    Engine& operator=(const Engine &) = delete;
};

#endif // ENGINE_H
