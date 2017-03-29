#ifndef ACTORCOMMONDATA_H
#define ACTORCOMMONDATA_H

#include "types.h"

namespace D3 {

struct ActorCommonData{ // sizeof = 0x2F0
    uint x000_Id;
    char x004_Name[0x80];
    int x084_ChestRelated;
    uint x088_AnnId;
    uint x08C_ActorId;
    uint x090_ActorSnoId;
    int x094_StructStart_Min8Bytes;
    uint x098_MonsterSnoId;
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
    uint x108_WorldId;
    uint x10C_SceneId;
    int x110_ItemSocketHostAcdId_StructStart_Min16Bytes;
    int x114_ItemLocation;
    int x118_ItemSlotX;
    int x11C_ItemSlotY;
    int x120_FastAttribGroupId;
    int x124_Id_FastAttribGroupId;
    uchar x128[0x178-0x128];
    int x178_GizmoType;
    int x17C_ActorType;
    float x180_Hitpoints;
    int x184_TeamIdOverride;
    int x188_TeamId;
    int _x18C;
    int x190_Flags_Is_Trail_Proxy_Etc;
    int x194_FollowPlayerIndex; // 7 = None
    int x198_Flags_1IsFollower_4HasHP;
    uchar x19C[0x2F0-0x19C];
};

} // namespace D3

#endif // ACTORCOMMONDATA_H
