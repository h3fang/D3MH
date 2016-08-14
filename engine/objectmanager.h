#ifndef D3MH_OBJECTMANAGER_H
#define D3MH_OBJECTMANAGER_H

#include "types.h"
#include "scene.h"
#include "actor.h"

namespace D3 {

struct ObjectManager{ // sizeof = 0xA10
    uchar       unknown_000[0x38];
    int         x038_Counter_CurrentFrame;
    uchar       unknown_03c[0x958-0x3c];
    ExpandableContainer<Actor>* x958_RActors;
    uchar       unknown_95c[0x998-0x95c];
    Container<Scene>*  x998_Scenes;
    uchar       unknown_99C[0xA10 - 0x99c];
};

}

#endif // D3MH_OBJECTMANAGER_H

