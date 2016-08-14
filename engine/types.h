#ifndef D3MH_TYPES_H
#define D3MH_TYPES_H

#include <stdint.h>

namespace D3 {

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
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
    Vec3 min;
    Vec3 max;
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
class SinglyLinkedListNode
{
public:
    SinglyLinkedListNode* x00_Next;
    T x04_Element;
};

// int SizeOf = 8
template <class T>
class SinglyLinkedList
{
public:
    int x00_Count;
    SinglyLinkedListNode<T>* x04_First;
};

// SizeOf = 0x30; // 48
template <class T>
class AllocatorBlock
{
public:
    T* x00_PtrElements;
    T* x04_NextFreeElement;
    int x08_Limit;
    int x0C_ElementSize;
    int x10_ElementCount;
    int x14;
    int x18;
    int x1C_FreeCount;
    int x20;
    void* x24_FreeSpaceBitmap;
    int x28;
    int x2C_GoodFood;
};


// SizeOf = 0x1C
template <class T>
class Allocator
{
public:
    int x00_ElementSize;
    int x04_Limit;
    SinglyLinkedList<AllocatorBlock<T>> x08_Blocks;
    int x10_Flags;
    int x14_MemVT;
    int x18_GoodFood; // 0xFEEDFACE when disposed.
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

// sizeof = 0x124
template <class T>
class Container
{
public:
    char    x000_Name[256];
    int    x100_Capacity;
    int    x104_ItemSize;
    int    x108_MaxIndex;
    int    x10C_Count;
    short   x110_NextHash;
    short   x112_NextIndex;
    int     x114_Free;
    int     x118_NeedsToExpand;
    uint   x11C_PtrItems;
    int     x120_Allocation;
};

template <class T> // sizeof = 0x168
class ExpandableContainer
{
public:
    char    x000_Name[256];
    int    x100_Capacity;
    int    x104_ItemSize;
    int    x108_MaxIndex;
    int    x10C_Count;
    short   x110_NextHash;
    short   x112_NextIndex;
    int     x114_Free;
    int     x118_NeedsToExpand;
    uint   x11C_PtrItems;
    int     x120_Allocation;
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

// SizeOf = 0x38; // = 56
template <class T>
class Vector
{
public:
    T* x00_Data;
    int x04;
    int x08_Size;
    int x0C_Capacity;
    BasicAllocator<T> x10_Allocator;
    int _x2C;
    int x30; // Sometimes 1
    int _x34;
};

template<class TKey, class TValue>
class MapEntry
{
public:
    MapEntry* x00_Next;
    TKey x04_Key;
    TValue x08_Value;
};

// SizeOf = 0x70; // 112
template<class TKey, class TValue>
class Map
{
public:
    int x00_Mask;
    int x04_Count;
    Allocator<MapEntry<TKey,TValue>>* x08_PtrEntryAllocator;
    int _x0C;
    Vector<MapEntry<TKey,TValue>*> x10_Buckets;
    Allocator<MapEntry<TKey,TValue>> x48_EntryAllocator;
    int _x64;
    int _x68;
    int _x6C;

};

template<class T>
class LinkedListNode
{
public:
    T Value;
    LinkedListNode* Previous;
    LinkedListNode* Next;
};

// SizeOf = 0x10; // = 16
template<class T>
class LinkedList
{
public:
    LinkedListNode<T>* First;
    LinkedListNode<T>* Last;
    int Count;
    Allocator<LinkedListNode<T>>* NodeAllocator;
};


// SizeOf = 0x30; // 48
template<class T>
class ListPack
{
public:
    LinkedListNode<T>* First;
    LinkedListNode<T>* Last;
    int Count;
    Allocator<LinkedListNode<T>>* NodeAllocator;
    Allocator<LinkedListNode<T>> x10_NodeAllocator;
    int x2C_Boolean;
};

// SizeOf = 0x14; // 20
template<class T>
class ListB
{
public:
    LinkedListNode<T>* First;
    LinkedListNode<T>* Last;
    int Count;
    Allocator<LinkedListNode<T>>* NodeAllocator;
    int x10_Boolean;
};

}

#endif // D3MH_TYPES_H

