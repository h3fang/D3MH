#ifndef ACTORCOMMONDATA_H
#define ACTORCOMMONDATA_H

#include "types.h"

namespace D3 {

struct ActorCommonData{ // sizeof = 0x2F8
    uint x000_Id;
    char x004_Name[0x80];
    int x084_ChestRelated;
    uint x088_AnnId;
    uint x08C_ActorId;
    int x090_ActorSnoId;
    float x094_StructStart_Min8Bytes;
    float x098_MonsterSnoId;
    int x09C;
    int x0A0;
    int x0A4_GameTick;
    int x0A8_GameTick;
    int x0AC_GameTick;
    int x0B0_GameBalanceType;
    int x0B4_GameBalanceId;
    int x0B8_MonsterQuality;
    int x0BC_IsItem_StructStart_Min100Bytes;
    int x0C0_StructStart_Min80bytes;
    int _x0C4;
    float x0C8_Direction;
    float x0CC_Direction;
    float x0D0_WorldPosX;
    float x0D4_WorldPosY;
    float x0D8_WorldPosZ;
    float x0DC_Radius;
    float _x0E0[7];
    float x0FC_Height;
    float x100;
    int _x104;
    int x108_WorldId;
    int x10C_SceneId;
    uchar x110[0x180-0x110];
    int x180_GizmoType;
    int x184_ActorType;
    uchar x188[0x2F8-0x188];
};

} // namespace D3

#endif // ACTORCOMMONDATA_H
