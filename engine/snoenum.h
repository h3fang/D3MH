#ifndef D3MH_SNOENUM_H
#define D3MH_SNOENUM_H

namespace D3 {

#define INVALID_VALUE   0xFFFFFFFF

enum SnoGroupId : unsigned int
{
    SnoGroupId_Actor = 1,
    SnoGroupId_Adventure = 2,
    SnoGroupId_AmbientSound = 5,
    SnoGroupId_Anim = 6,
    SnoGroupId_Anim2D = 7,
    SnoGroupId_AnimSet = 8,
    SnoGroupId_Appearance = 9,
    SnoGroupId_Hero = 10,
    SnoGroupId_Cloth = 11,
    SnoGroupId_Conversation = 12,
    SnoGroupId_ConversationList = 13,
    SnoGroupId_EffectGroup = 14,
    SnoGroupId_Encounter = 15,
    SnoGroupId_Explosion = 17,
    SnoGroupId_FlagSet = 18,
    SnoGroupId_Font = 19,
    SnoGroupId_GameBalance = 20,
    SnoGroupId_Globals = 21,
    SnoGroupId_LevelArea = 22,
    SnoGroupId_Light = 23,
    SnoGroupId_MarkerSet = 24,
    SnoGroupId_Monster = 25,
    SnoGroupId_Observer = 26,
    SnoGroupId_Particle = 27,
    SnoGroupId_Physics = 28,
    SnoGroupId_Power = 29,
    SnoGroupId_Quest = 31,
    SnoGroupId_Rope = 32,
    SnoGroupId_Scene = 33,
    SnoGroupId_SceneGroup = 34,
    SnoGroupId_ShaderMap = 36,
    SnoGroupId_Shaders = 37,
    SnoGroupId_Shakes = 38,
    SnoGroupId_SkillKit = 39,
    SnoGroupId_Sound = 40,
    SnoGroupId_SoundBank = 41,
    SnoGroupId_StringList = 42,
    SnoGroupId_Surface = 43,
    SnoGroupId_Textures = 44,
    SnoGroupId_Trail = 45,
    SnoGroupId_UI = 46,
    SnoGroupId_Weather = 47,
    SnoGroupId_Worlds = 48,
    SnoGroupId_Recipe = 49,
    SnoGroupId_Condition = 51,
    SnoGroupId_TreasureClass = 52,
    SnoGroupId_Account = 53,
    SnoGroupId_TimedEvent = 55,
    SnoGroupId_Act = 56,
    SnoGroupId_Material = 57,
    SnoGroupId_QuestRange = 58,
    SnoGroupId_Lore = 59,
    SnoGroupId_Reverb = 60,
    SnoGroupId_PhysMesh = 61,
    SnoGroupId_Music = 62,
    SnoGroupId_Tutorial = 63,
    SnoGroupId_BossEncounter = 64,
    SnoGroupId_Accolade = 66,
    SnoGroupId_AnimTree = 67,
    SnoGroupId_Vibrations = 68,
};

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

}

#endif // D3MH_SNOENUM_H

