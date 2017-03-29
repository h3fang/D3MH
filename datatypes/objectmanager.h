#ifndef D3MH_OBJECTMANAGER_H
#define D3MH_OBJECTMANAGER_H

#include "types.h"
#include "scene.h"
#include "actor.h"
#include "attributedescriptor.h"

namespace D3 {

// sizeof = 0xA30
struct ObjectManager {
    uchar       unknown_000[0x38];
    int         x038_Counter_CurrentFrame;
    uchar       unknown_03C[0x14C-0x3C];
    FastAttrib* x14C_Ptr_92Bytes_FastAttrib;
    uchar       unknown_150[0x978-0x150];
    ExpandableContainer<Actor>* x978_Ptr_RActors;
    uchar       unknown_97C[0x9B8-0x97C];
    Container<Scene>*  x9B8_Scenes;
    uchar       unknown_9CC[0xA30 - 0x9BC];
};

}

#endif // D3MH_OBJECTMANAGER_H

