#ifndef D3MH_OBJECTMANAGER_H
#define D3MH_OBJECTMANAGER_H

#include "types.h"
#include "scene.h"
#include "actor.h"

namespace D3 {

struct ObjectManager{ // sizeof = 0xA10
    UCHAR       unknown_000[0x38];
    DWORD       x038_Counter_CurrentFrame;
    UCHAR       unknown_03c[0x958-0x3c];
    ExpandableContainer<Actor>* x958_RActors;
    UCHAR       unknown_95c[0x998-0x95c];
    Container<Scene>*  x998_Scenes;
    UCHAR       unknown_99C[0xA10 - 0x99c];
}/*__attribute__ ((packed))*/;

}

#endif // D3MH_OBJECTMANAGER_H

