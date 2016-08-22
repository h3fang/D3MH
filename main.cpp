#include "minimap.h"
#include <QApplication>
#include <stdio.h>

#include "engine/structs.h"
#include "process/helper.h"

int main(int argc, char *argv[])
{
    fprintf(stderr, "AdjustDebugPrivilege() %s\n", AdjustDebugPrivilege() ? "Succeeded" : "Failed");

    QApplication a(argc, argv);

    Minimap w;
    w.show();

    setbuf(stderr, NULL);

    qDebug("sizeof ObjectManager %#x %#x", sizeof(D3::ObjectManager), offsetof(D3::ObjectManager, x998_Scenes));
    qDebug("sizeof ActorCommonData %#x", sizeof(D3::ActorCommonData));
    qDebug("sizeof Container %#x", sizeof(D3::Container<void>));
    qDebug("sizeof ExpandableContainer %#x", sizeof(D3::ExpandableContainer<void>));
    qDebug("sizeof Scene %#x", sizeof(D3::Scene));
    qDebug("sizeof LocalData %#x", sizeof(D3::LocalData));
    qDebug("sizeof AssetScene %#x %#x", sizeof(D3::SceneSno), offsetof(D3::SceneSno, NavZone));
    qDebug("sizeof NavZoneDef %#x %#x", sizeof(D3::NavZoneDef), offsetof(D3::NavZoneDef, NavCells));
    qDebug("sizeof SnoDefinition %#x", sizeof(D3::SnoDefinition));
    qDebug("sizeof LevelArea %#x %#x", sizeof(D3::LevelArea), offsetof(D3::LevelArea, x044_SnoId));

    return a.exec();
}
