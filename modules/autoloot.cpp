#include "autoloot.h"

#include <algorithm>

#include "engine.h"

namespace D3 {

std::atomic<bool> is_looting;

AutoLoot::AutoLoot(Engine *e) :
    engine(e)
{

}

void AutoLoot::loot()
{
    auto items = getItemsToLoot();
    std::sort(items.begin(), items.end(), [](const Item& lhs, const Item& rhs) {
        return lhs.distance < rhs.distance;
    });

    is_looting = true;


    is_looting = false;
}

std::vector<Item> AutoLoot::getItemsToLoot()
{
    std::vector<Item> result;
    result.reserve(10);

    const auto x = engine->localData.x24_WorldPosX;
    const auto y = engine->localData.x28_WorldPosY;
    const auto z = engine->localData.x2C_WorldPosZ;

    float d = 0.0f;

    for (const ActorCommonData& acd : engine->acds) {
        if (acd.x090_ActorSnoId == INVALID_SNO_ID ||
                acd.x184_ActorType != ActorType_Item ||
                acd.x114_ItemLocation != ItemLoc_UnknownLoc ||
                (d = distance(x, y, z, acd.x0D0_WorldPosX, acd.x0D4_WorldPosY, acd.x0D8_WorldPosZ))> MAX_LOOT_RADIUS) {
            continue;
        }

        Item item;
        item.actor_sno_id = acd.x090_ActorSnoId;
        item.x = acd.x0D0_WorldPosX;
        item.y = acd.x0D4_WorldPosY;
        item.z = acd.x0D8_WorldPosZ;
        item.distance = d;

        result.push_back(item);
    }

    return result;
}

bool isGem(const ActorCommonData& acd)
{
    //string ItemName = acd.x004_Name // Not needed but for easier debug
    switch (acd.x090_ActorSnoId)
    {
        case 437414: // Amethyst_15 Marquise Amethyst
        case 437415: // Amethyst_16 Imperial Amethyst
        case 437459: // Ruby_15 Marquise Ruby
        case 437460: // Ruby_16 Imperial Ruby
        case 437481: // x1_Diamond_06 Marquise Diamond
        case 437482: // x1_Diamond_07 Imperial Diamond
        case 437448: // x1_Emerald_06 Marquise Emerald
        case 437449: // x1_Emerald_07 Imperial Emerald
        case 437469: // x1_Topaz_06 Marquise Topaz
        case 437470: // x1_Topaz_07 Imperial Topaz
            return true;

        default:
            return false;
    }
}

bool isMaterial(const ActorCommonData& acd)
{
    //string ItemName = acd.x004_Name // Not needed but for easier debug
    switch (acd.x090_ActorSnoId)
    {
        case 361988:    // Crafting_Legendary_05
        case 361989:    // Crafting_Looted_Reagent_05
        case 361986:    // Crafting_Rare_05
        case 361985:    // Crafting_Magic_05
        case 361984:    // Crafting_AssortedParts_05
        case 137958:    // CraftingMaterials_Flippy_Global
        case 365020:    // CraftingReagent_Legendary_Set_Borns_X1
        case 364281:    // CraftingReagent_Legendary_Set_Cains_X1
        case 364290:    // CraftingReagent_Legendary_Set_Demon_X1
        case 364305:    // CraftingReagent_Legendary_Set_Hallowed_X1
        case 364975:    // CraftingReagent_Legendary_Set_CaptainCrimsons_X1
        case 364725:    // DemonOrgan_Diablo_x1
        case 364723:    // DemonOrgan_Ghom_x1
        case 364724:    // DemonOrgan_SiegeBreaker_x1
        case 364722:    // DemonOrgan_SkeletonKing_x1
        case 366949:    // InfernalMachine_Diablo_x1
        case 366947:    // InfernalMachine_Ghom_x1
        case 366948:    // InfernalMachine_SiegeBreaker_x1
        case 366946:    // InfernalMachine_SkeletonKing_x1
        case 359504:    // HoradricRelic
        case 449044:    // Deathbreath (itemFlippy_deathsBreath_Flippy_Global)
            return true;
        default:
            return false;
    }
}

}
