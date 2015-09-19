#ifndef D3MH_SNOENUM_H
#define D3MH_SNOENUM_H

namespace D3 {

#define INVALID_VALUE			0xFFFFFFFF

enum ActorType
{
    ActorType_Invalid = 0,
    ActorType_Monster = 1,
    ActorType_Gizmo = 2,
    ActorType_ClientEffect = 3,
    ActorType_ServerProp = 4,
    ActorType_Enviroment = 5,
    ActorType_Critter = 6,
    ActorType_Player = 7,
    ActorType_Item = 8,
    ActorType_AxeSymbol = 9,
    ActorType_Projectile = 10,
    ActorType_CustomBrain = 11
};

enum MonsterRace
{
    MobRace_Unknown = INVALID_VALUE,
    MobRace_Fallen = 1,
    MobRace_GoatMen = 2,
    MobRace_Rogue = 3,
    MobRace_Skeleton = 4,
    MobRace_Zombie = 5,
    MobRace_SPider = 6,
    MobRace_Triune = 7,
    MobRace_WoodWraith = 8,
    MobRace_Human = 9,
    MobRace_Animal = 10
};

enum MonsterType
{
    MobType_Unknown = INVALID_VALUE,
    MobType_Undead = 0,
    MobType_Demon = 1,
    MobType_Beast = 2,
    MobType_Human = 3,
    MobType_Breakable = 4,
    MobType_Scenery = 5,
    MobType_Ally = 6,
    MobType_Team = 7,
    MobType_Helper = 8
};

enum ItemLocation : unsigned int
{
    ItemLoc_UnknownLoc = 0xFFFFFFFF, //  meaning NPC or item on the ground.
    ItemLoc_Backpack = 0,
    ItemLoc_Head = 1,
    ItemLoc_Torso = 2,
    ItemLoc_RightHand = 3,
    ItemLoc_LeftHand = 4,
    ItemLoc_Hands = 5,
    ItemLoc_Waist = 6,
    ItemLoc_Feet = 7,
    ItemLoc_Shoulders = 8,
    ItemLoc_Legs = 9,
    ItemLoc_Bracers = 10,
    ItemLoc_LeftFinger = 11,
    ItemLoc_RightFinger = 12,
    ItemLoc_Neck = 13,
    ItemLoc_Talisman = 14,
    ItemLoc_Stash = 15,
    ItemLoc_Gold = 16,
    ItemLoc_Merchant = 18,
    ItemLoc_InSocket = 20,
    ItemLoc_PetRightHand = 23,
    ItemLoc_PetLeftHand = 24,
    ItemLoc_PetSpecial = 25,
    ItemLoc_PetLeftFinger = 28,
    ItemLoc_PetRightFinger = 27,
    ItemLoc_PetNeck = 26
};

enum NavCellFlagsDW : unsigned int
{
    NavCellFlagDW_AllowWalk = 0x1,
    NavCellFlagDW_AllowFlier = 0x2,
    NavCellFlagDW_AllowSpider = 0x4,
    NavCellFlagDW_LevelAreaBit0 = 0x8,
    NavCellFlagDW_LevelAreaBit1 = 0x10,
    NavCellFlagDW_NoNavMeshIntersected = 0x20,
    NavCellFlagDW_NoSpawn = 0x40,
    NavCellFlagDW_Special0 = 0x80,
    NavCellFlagDW_Special1 = 0x100,
    NavCellFlagDW_SymbolNotFound = 0x200,
    NavCellFlagDW_AllowProjectile = 0x400,
    NavCellFlagDW_AllowGhost = 0x800,
    NavCellFlagDW_RoundedCorner0 = 0x1000,
    NavCellFlagDW_RoundedCorner1 = 0x2000,
    NavCellFlagDW_RoundedCorner2 = 0x4000,
    NavCellFlagDW_RoundedCorner3 = 0x8000
};

enum NavCellFlagsW : unsigned short
{
    NavCellFlagW_AllowWalk = 0x1,
    NavCellFlagW_AllowFlier = 0x2,
    NavCellFlagW_AllowSpider = 0x4,
    NavCellFlagW_LevelAreaBit0 = 0x8,
    NavCellFlagW_LevelAreaBit1 = 0x10,
    NavCellFlagW_NoNavMeshIntersected = 0x20,
    NavCellFlagW_NoSpawn = 0x40,
    NavCellFlagW_Special0 = 0x80,
    NavCellFlagW_Special1 = 0x100,
    NavCellFlagW_SymbolNotFound = 0x200,
    NavCellFlagW_AllowProjectile = 0x400,
    NavCellFlagW_AllowGhost = 0x800,
    NavCellFlagW_RoundedCorner0 = 0x1000,
    NavCellFlagW_RoundedCorner1 = 0x2000,
    NavCellFlagW_RoundedCorner2 = 0x4000,
    NavCellFlagW_RoundedCorner3 = 0x8000
};

enum SNOGroup : int
{
    SNOGroup_Actor		= 0x0113,
    SNOGroup_Monster	= 0x0544,
    SNOGroup_Scene		= 0x9E,
};

}

#endif // D3MH_SNOENUM_H

