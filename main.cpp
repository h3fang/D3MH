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

    qDebug("sizeof ObjectManager %#x", sizeof(D3::ObjectManager));
    qDebug("sizeof BasicAllocator %#x", sizeof(D3::BasicAllocator<void>));
    qDebug("sizeof Container %#x", sizeof(D3::Container<void>));
    qDebug("sizeof ExpandableContainer %#x", sizeof(D3::ExpandableContainer<void>));
    qDebug("sizeof Scene %#x", sizeof(D3::Scene));
    qDebug("sizeof LocalData %#x", sizeof(D3::LocalData));

    return a.exec();
}
