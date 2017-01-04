#ifndef D3MH_AUTOLOOT_H
#define D3MH_AUTOLOOT_H

#include <vector>

#include <datatypes/structs.h>

class KeyboardMouse;
class Hook;

namespace D3 {

const float MAX_LOOT_RADIUS = 40.0f;

class Engine;

struct Item {
    uint actor_id;
    float x;
    float y;
    float z;
    float distance;
};

class AutoLoot
{
public:
    AutoLoot(Engine* e);
    ~AutoLoot();

    void loot();

private:
    std::vector<Item> getItemsToLoot();

private:
    Engine* engine;
    KeyboardMouse* kbm;
    Hook* hook;

    int screen_height, screen_width;
};

}

#endif // D3MH_AUTOLOOT_H
