#ifndef D3MH_SNO_H
#define D3MH_SNO_H

#include "snoenum.h"
#include "types.h"

namespace D3 {

//sizeof = 0x0C
class SNOHeader
{
public:
    uint x00_SnoId;         // 0x000
    int x04_LockCount;      // 0x004
    int x08_Flags;          // 0x008, 1 = DoNotPurge
};

//sizeof = 0x10
struct SNOFileHeader
{
    uint deadbeef;          // 0x000 0xDEADBEEF
    uint Reserved0;         // 0x004
    uint Reserved1;         // 0x008
    uint Reserved2;         // 0x010
};

//sizeof = 0x10
class DataPtr
{
public:
    uint file_offset;		// 0x000
    uint size;				// 0x004
    uint mem_offset;		// 0x008
    uint unused;			// 0x00C
};

//sizeof = 0x8
class DataPtr2
{
public:
    uint file_offset;		// 0x000
    uint size;				// 0x004
};

//sizeof = 0x8
class NavMeshSquare
{
public:
    float z;					// 0x000
    NavCellFlagsDW flags;		// 0x004
};

//sizeof = 0x6
class NavGridSquare
{
public:
    NavCellFlagsW Flags;		// 0x000
    ushort W1;					// 0x002
    ushort CellLookupIndex;		// 0x004
};

//sizeof = 0x20
class NavCell
{
public:
    Vec3 min;					// 0x000
    Vec3 max;					// 0x00C
    ushort flag;			// 0x018
    ushort NeighbourCount;		// 0x01A
    uint NeighborsIndex;		// 0x01C
};

//sizeof = 0x4
class NavCellLookup
{
public:
    ushort Flags;               // 0x000
    ushort WCell;				// 0x002
};

//sizeof = 0x28
class NavMeshDef
{
public:
    uint SquaresX;				// 0x000
    uint SquaresY;				// 0x004
    uint pad_008;				// 0x008
    uint SquareCount;			// 0x00C
    float pad_010;				// 0x010
    DataPtr2 Squares;			// 0x014
    uint pad_01C;				// 0x01C
    NavMeshSquare* array_ptr;	// 0x020
    uint pad_024;				// 0x024
};

//sizeof = 0x88
class NavZoneDef
{
public:
    int NavCellCount;			// 0x000
    uint pad_004[3];			// 0x004
    DataPtr2 NavCells;			// 0x010
    uint NeighbourCount;		// 0x018
    uint pad_01C[3];			// 0x01C
    DataPtr2 NavCellNeighbours; // 0x028
    float pad_030[2];			// 0x030
    uint pad_038;				// 0x038
    Vec2 V0;					// 0x03C
    uint pad_044[3];			// 0x044
    DataPtr2 GridSquares;		// 0x050
    uint pad_058[4];			// 0x058
    DataPtr2 CellLookups;		// 0x068
    uint BorderDataCount;		// 0x070
    uint pad_074[3];			// 0x074
    DataPtr2 BorderData;		// 0x080
};

//sizeof = 0x210
class SceneSno
{
public:
    SNOHeader header;			// 0x000
    int x00C;                   // 0x00C
    AABB AABBBounds;			// 0x010
    AABB AABBMarketSetBounds;	// 0x028
    NavMeshDef navmesh;			// 0x040
    DataPtr2 Exclusions;		// 0x068
    uint pad_070[14];			// 0x070
    DataPtr2 Inclusions;		// 0x0A8
    uint pad_0B0[14];			// 0x0B0
    DataPtr2 MarkerSets;		// 0x0E8
    uint pad_0F0[14];			// 0x1F0
    char lootlink[64];			// 0x128
    DataPtr2 MapTrigEvent;		// 0x268
    uint pad_170[4];			// 0x270
    NavZoneDef NavZone;			// 0x180
    uint SNOAppearance;        // 0x208
    uint SNOPhysMesh;			// 0x20C
};

class SceneSnoFile
{
public:
    SNOFileHeader header;
    SceneSno sceneSno;
};

// sizeof = 0x10
class SnoDefinition
{
public:
    uint x00_Id;                 // 0x000
    char unk[3];                // 0x004
    char x07_SnoGroupId;        // 0x007
    int x08_Size;               // 0x008
    uint x0C_pSNOAddr;         // 0x00C
};

// SizeOf = 0x84;
class SnoGroupManager
{
public:
    int x00_VTable;
    int x04;
    int x08;
    int x0C;
    Container<SnoDefinition>* x10_Container;
    Container<void>* x14_Container;
    uint x18_Flags;
    char x1C_Name[32];
    int x3C_SnoGroupId;
    int x40;
    int x44;
    int x48;
    int x4C;
    int x50;
    int x54;
    int x58;
    int x5C;
    int x60;
    int x64_Limit;
    int x68_ItemSize;
    int x6C_MaxCacheSize;
    int x70_EstCacheSize;
    void* x74_Ptr_DataType;
    int x78_Counter;
    void* x7C;
    int x80_Neg1_InvalidSnoId; // -1 or a group specific Axe_Bad_Data ID
};

} // namespace D3

#endif // D3MH_SNO_H
