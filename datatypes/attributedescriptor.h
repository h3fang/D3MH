#ifndef ATTRIBUTEDESCRIPTOR_H
#define ATTRIBUTEDESCRIPTOR_H

#include "types.h"

namespace D3 {

enum class ParameterType : int {
    DamageType = 0, // SymbolTable @ 0x01802898
    PowerSnoId = 4,
    PowerSnoId2 = 11,
    ResourceType = 10, // SymbolTable @ 0x01802FB8
    TreasureClassSnoId = 14,
    ActorSnoId = 15,
    ConversationSnoId = 16,
    ActorType = 18, // SymbolTable @ 0x01802778
    InventoryLocation = 19, // SymbolTable @ 0x01803BF0
};

// SizeOf = 0x28 = 40
struct AttributeDescriptor {
    int x00_Id;
    int x04_DefaultValue;
    ParameterType x08_ParameterType;
    int x0C;
    int x10_DataType; // 0 = Float, 1 = Int32
    char* x14_Formula;
    char* x18_Formula;
    char* x1C_Name;
    void* x20_PtrDecoder;
    int x24;
};

struct AttributeValue {
    union {
        int Int32;
        float Single;
    } value;
};

// SizeOf = 0x9C8
struct FastAttribGroup {
    int x000_Id;
    int x004_Flags;
    int _x008;
    Map<int, AttributeValue>* x00C_PtrMap;
    Map<int, AttributeValue> x010_Map;
    int _x074[0x9C8-0x74];
};

// SizeOf = 0x5C = 92
struct FastAttrib {
    Allocator<MapEntry<int, void*>> x00_Allocator_10x12Bytes;
    Allocator<char> x1C_Allocator_10x12Bytes;
    Allocator<char> x38_Allocator_10x12Bytes;
    ExpandableContainer<FastAttribGroup>* x54_Groups;
    int x58;
};

}

#endif // ATTRIBUTEDESCRIPTOR_H
