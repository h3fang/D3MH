#include "helpers.h"

namespace D3 {

bool isTreasureGoblin(const ActorCommonData& acd)
{
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

bool isShrine(const ActorCommonData& acd)
{
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

bool isPoolOfReflection(const ActorCommonData &acd)
{
    switch (acd.x090_ActorSnoId) {
    case 0x0005B2D7:
    case 0x0005B339:
    case 0x0005B2D9:
        return true;

    default:
        return false;
    }
}

}
