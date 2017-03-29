#include "helpers.h"

namespace D3 {

bool isGem(const ActorCommonData& acd) {
    //string ItemName = acd.x004_Name // Not needed but for easier debug
    switch (acd.x090_ActorSnoId) {
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

bool isMaterial(const ActorCommonData& acd) {
    //string ItemName = acd.x004_Name // Not needed but for easier debug
    switch (acd.x090_ActorSnoId) {
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

bool isMonster(const ActorCommonData &acd) {
    return acd.x17C_ActorType == int(D3::ActorType::Monster) &&
            acd.x090_ActorSnoId != D3::INVALID_ID &&
            acd.x180_Hitpoints > 0.9 &&
            (acd.x190_Flags_Is_Trail_Proxy_Etc & 1) == 0 &&
            acd.x188_TeamId == 10;
}

bool isTreasureGoblin(const ActorCommonData& acd) {
    switch (acd.x090_ActorSnoId) {
    // TODO:update goblin IDs
    case 0x00001760: // treasureGoblin_A
    case 0x00001761: // treasureGoblin_B
    case 0x00001763: // treasureGoblin_C
    case 0x00001764: // treasureGoblin_D
    case 0x00063C4F: // treasureGoblin_E
    case 0x00063D9D: // treasureGoblin_F
    case 0x0005F9A9: // treasureGoblin_G
    case 0x00064E69: // treasureGoblin_H
    case 0x00068A77: // treasureGoblin_I
    case 0x00068C69: // treasureGoblin_J
    case 0x0006E1B1: // treasureGoblin_K
        return true;

    default:
        return false;
    }
}

bool isShrine(const ActorCommonData& acd) {
    switch (acd.x090_ActorSnoId) {
    // TODO:update shrine IDs
    case 0x00050BC7:
    case 0x00050BCA:
    case 0x00050BC8:
    case 0x00050BC9:
    case 0x00050BCB:
    case 0x0002AFCC:
    case 0x0002AFCA:
    case 0x0002AFCB:
    case 0x0002AFCD:
    case 0x0003F8FA:
    case 0x0003F8FB:
    case 0x00041C25:
    case 0x00026408:
    case 0x0006153E:
    case 0x000210D8:
        return true;

    default:
        return false;
    }
}

bool isPoolOfReflection(const ActorCommonData &acd) {
    switch (acd.x090_ActorSnoId) {
    case 0x0005B2D7:
    case 0x0005B339:
    case 0x0005B2D9:
        return true;

    default:
        return false;
    }
}

Point toScreenCoordinate(float x, float y, float z, LocalData* ld, float width, float height) {
    float xd = x - ld->x24_WorldPosX;
    float yd = y - ld->x28_WorldPosY;
    float zd = z - ld->x2C_WorldPosZ;

    float w = -0.515f * xd - 0.514f * yd - 0.686f * zd + 97.985f;
    if (w < 1.0f) w = 1.0f;
    float rX = (-1.682f * xd + 1.683f * yd + 0.007045f) / w;
    float rY = (-1.540f * xd - 1.539f * yd + 2.307f * zd + 6.161f) / w;

    rX /= width / height * 600.0f / 800.0f;

    Point r;

    r.x = (rX + 1.0f) / 2.0f * width;
    r.y = (1.0f - rY) / 2.0f * height;
    return r;
}

}
