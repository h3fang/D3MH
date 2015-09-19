#ifndef D3MH_TYPES_H
#define D3MH_TYPES_H

namespace D3 {

typedef char CHAR;
typedef unsigned char UCHAR;
typedef long LONG;
typedef unsigned long ULONG;
typedef unsigned short WORD;
typedef unsigned int DWORD;

struct Vec2
{
    float x;     // 0x000
    float y;     // 0x004
};

struct Vec3
{
    float x;     // 0x000
    float y;     // 0x004
    float z;     // 0x008
};

struct AABB
{
    Vec3 Base;
    Vec3 Offset;
};

struct Vec4
{
    float x;     // 0x000
    float y;     // 0x004
    float z;     // 0x008
    float w;     // 0x00C
};

struct Mat4
{
    float mm[16];     // 0x000
};

struct Time
{
    int x00;
    int x04;
    int x08;
};

template <class T>
class BasicAllocator // sizeof = 0x1C
{
public:
    T*      x00_Allocation;
    int     x04;
    int     x08_Size;
    int     x0C_Flags; // 1 => can/should free, 2 => call free()/realloc() instead of using Blizzard classes.
    void*   x10_MemoryVTable;
    int     _x14;
    int     x18_GoodFood;
};

template <class T> // sizeof = 0x124
class Container
{
public:
    CHAR    x000_Name[256];
    LONG    x100_Capacity;
    LONG    x104_ItemSize;
    LONG    x108_MaxIndex;
    LONG    x10C_Count;
    short   x110_NextHash;
    short   x112_NextIndex;
    int     x114_Free;
    int     x118_NeedsToExpand;
    T**     x11C_PtrItems;
    int     x120_Allocation;
};

template <class T> // sizeof = 0x168
class ExpandableContainer : public Container<T>
{
public:
    int     x124;
    int     x128;
    int     _x12C;
    BasicAllocator<T> x130_Allocator;
    int     _x14C;
    int     _x150;
    int     _x154;
    void*   x158_MemoryVTable;
    int     x15C_Limit;
    int     x160_MaxLimit_;
    int     x164_Bits;
};

}

#endif // D3MH_TYPES_H

