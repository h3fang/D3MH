#ifndef D3MH_OBJECTMANAGER_H
#define D3MH_OBJECTMANAGER_H

#include "types.h"
#include "scene.h"
#include "actor.h"
#include "attributedescriptor.h"

namespace D3 {

// sizeof = 0xA40
struct ObjectManager {
    uchar       unknown_000[0x38];
    int         x038_Counter_CurrentFrame;
    uchar       unknown_03C[0x14C-0x3C];
    FastAttrib* x14C_Ptr_92Bytes_FastAttrib;
    uchar       unknown_150[0x988-0x150];
    //Storage x7C8_Storage;
    ExpandableContainer<Actor>* x988_Ptr_RActors;
    uchar       unknown_98C[0x9C8-0x98C];
    Container<Scene>*  x9C8_Scenes;
    uchar       unknown_9CC[0xA40 - 0x9CC];
};

}

#endif // D3MH_OBJECTMANAGER_H

