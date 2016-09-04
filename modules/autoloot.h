#ifndef D3MH_AUTOLOOT_H
#define D3MH_AUTOLOOT_H

#include <vector>
#include <atomic>

#include <datatypes/structs.h>

namespace D3 {

const float MAX_LOOT_RADIUS = 40.0f;

class Engine;

extern std::atomic<bool> is_looting;

struct Item {
    uint actor_sno_id;
    float x;
    float y;
    float z;
    float distance;
};

class AutoLoot
{
public:
    AutoLoot(Engine* e);

    void loot();

private:
    std::vector<Item> getItemsToLoot();
    bool isGem(const ActorCommonData& acd);
    bool isMaterial(const ActorCommonData& acd);

private:
    bool is_looting;
    Engine* engine;
};

}

#endif // D3MH_AUTOLOOT_H
