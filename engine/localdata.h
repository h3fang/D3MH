#ifndef D3MH_LOCALDATA_H
#define D3MH_LOCALDATA_H

#include "types.h"

namespace D3 {

struct LocalData{ // sizeof = 0x38
    int x00_IsActorCreated;
    int x04_IsNotInGame;
    uint x08_SceneSnoId;
    uint x0C_WorldSnoId;
    uint x10_ActorSnoId;
    int x14_ActId;
    int x18_WorldSeed;
    int x1C_QuestId;
    int x20_QuestStep;
    float x24_WorldPosX;
    float x28_WorldPosY;
    float x2C_WorldPosZ;
    int x30_PlayerDataSlotsInUse;
    int x34_IsPlayerDataCreated;
};

}
#endif // D3MH_LOCALDATA_H

