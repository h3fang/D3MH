#ifndef D3MH_SNO_H
#define D3MH_SNO_H

#include "snoenum.h"
#include "types.h"

namespace D3 {

//sizeof = 0x10
class MemHeader
{
public:
    DWORD id;				// 0x000
    DWORD pad_004[3];		// 0x004
};

//sizeof = 0x20
class FileHeader
{
    DWORD deadbeef;			// 0x000
    DWORD snoGroup;			// 0x004
    DWORD pad_008[2];		// 0x008
    MemHeader mempart;		// 0x010
};

//sizeof = 0x10
class DataPtr
{
public:
    DWORD file_offset;		// 0x000
    DWORD size;				// 0x004
    DWORD mem_offset;		// 0x008
    DWORD unused;			// 0x00C
};

//sizeof = 0x8
class DataPtr2
{
public:
    DWORD file_offset;		// 0x000
    DWORD size;				// 0x004
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
    WORD W1;					// 0x002
    WORD CellLookupIndex;		// 0x004
};

//sizeof = 0x20
class NavCell
{
public:
    Vec3 min;					// 0x000
    Vec3 max;					// 0x00C
    NavCellFlagsW flag;			// 0x018
    WORD NeighbourCount;		// 0x01A
    DWORD NeighborsIndex;		// 0x01C
};

//sizeof = 0x4
class NavCellLookup
{
public:
    NavCellFlagsW Flags;		// 0x000
    WORD WCell;					// 0x002
};

//sizeof = 0x28
class NavMeshDef
{
public:
    DWORD SquaresX;				// 0x000
    DWORD SquaresY;				// 0x004
    DWORD pad_008;				// 0x008
    DWORD SquareCount;			// 0x00C
    float pad_010;				// 0x010
    DataPtr2 Squares;			// 0x014
    DWORD pad_01C;				// 0x01C
    NavMeshSquare* array_ptr;	// 0x020
    DWORD pad_024;				// 0x024
};

//sizeof = 0x88
class NavZoneDef
{
public:
    DWORD NavCellCount;			// 0x000
    DWORD pad_004[3];			// 0x004
    DataPtr2 NavCells;			// 0x010
    DWORD NeighbourCount;		// 0x018
    DWORD pad_01C[3];			// 0x01C
    DataPtr2 NavCellNeighbours; // 0x028
    float pad_030[2];			// 0x030
    DWORD pad_038;				// 0x038
    Vec2 V0;					// 0x03C
    DWORD pad_044[3];			// 0x044
    DataPtr2 GridSquares;		// 0x050
    DWORD pad_058[4];			// 0x058
    DataPtr2 CellLookups;		// 0x068
    DWORD BorderDataCount;		// 0x070
    DWORD pad_074[3];			// 0x074
    DataPtr2 BorderData;		// 0x080
};

//sizeof = 210
class SnoScene
{
public:
    MemHeader header;			// 0x000
    AABB AABBBounds;			// 0x010
    AABB AABBMarketSetBounds;	// 0x028
    NavMeshDef navmesh;			// 0x040
    DataPtr2 Exclusions;		// 0x068
    DWORD pad_070[14];			// 0x070
    DataPtr2 Inclusions;		// 0x0A8
    DWORD pad_0B0[14];			// 0x0B0
    DataPtr2 MarkerSets;		// 0x0E8
    DWORD pad_0F0[14];			// 0x1F0
    char lootlink[64];			// 0x128
    DataPtr2 MapTrigEvent;		// 0x268
    DWORD pad_170[4];			// 0x270
    NavZoneDef NavZone;			// 0x180
    DWORD SNOAppearance;        // 0x208
    DWORD SNOPhysMesh;			// 0x20C
};

// sizeof = 0x10
class SnoDefinition
{
public:
    DWORD x00_Id;                 // 0x000
    char unk[3];                // 0x004
    char x07_SnoGroupId;        // 0x007
    DWORD x08_Size;             // 0x008
    DWORD pSNOAddr;             // 0x00C
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
    DWORD x18_Flags;
    char x1C_Name[32];
    SnoGroupId x3C_SnoGroupId;
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
