#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include "types.h"
#include "scene.h"
#include "actor.h"

namespace D3 {

struct ObjectManager{ // sizeof = 0x9F0
    UCHAR       unknown_000[0x38];
    ULONG       x038_Counter_CurrentFrame;
    UCHAR       unknown_03c[0x938-0x3c];
    ExpandableContainer<Actor>* x938_RActors;
    UCHAR       unknown_93c[0x978-0x93c];
    Container<Scene>*  x978_Scenes;
    UCHAR       unknown_97C[0x9F0 - 0x97c];
}/*__attribute__ ((packed))*/;

}

#endif // OBJECTMANAGER_H

