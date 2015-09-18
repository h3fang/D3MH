#ifndef LOCALDATA
#define LOCALDATA

#include "types.h"

namespace D3 {

struct LocalData{ // sizeof = 0x38
    int x00_IsActorCreated;
    int x04_IsNotInGame;
    int x08_SceneSnoId;
    int x0C_WorldSnoId;
    int x10_ActorSnoId;
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
#endif // LOCALDATA

