#include "minimap.h"
#include <QApplication>
#include <stdio.h>

#include "engine/structs.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Minimap w;
    w.show();

    setbuf(stderr, NULL);

    qDebug("sizeof ObjectManager %#x %#x", sizeof(D3::ObjectManager), offsetof(D3::ObjectManager, x998_Scenes));
    qDebug("sizeof BasicAllocator %#x", sizeof(D3::BasicAllocator<void>));
    qDebug("sizeof Container %#x", sizeof(D3::Container<void>));
    qDebug("sizeof ExpandableContainer %#x", sizeof(D3::ExpandableContainer<void>));
    qDebug("sizeof Scene %#x", sizeof(D3::Scene));
    qDebug("sizeof LocalData %#x", sizeof(D3::LocalData));
    qDebug("sizeof SnoScene %#x", sizeof(D3::SnoScene));
    qDebug("sizeof NavZoneDef %#x", sizeof(D3::NavZoneDef));
    qDebug("sizeof SnoDefinition %#x", sizeof(D3::SnoDefinition));

    return a.exec();
}
