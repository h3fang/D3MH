#include "minimap.h"
#include <QApplication>
#include <stdio.h>

#include "datatypes/structs.h"
#include "utils/helpers.h"

int main(int argc, char *argv[])
{
    setbuf(stderr, NULL);

    if (!AdjustDebugPrivilege()) {
        fprintf(stderr, "Failed to AdjustDebugPrivilege()\n");
        return 1;
    }

    if (!terminateBN()) {
        fprintf(stderr, "Failed to terminateBN()\n");
        return 2;
    }

    timeBeginPeriod(1);

    QApplication a(argc, argv);

    Minimap w;
    w.show();

    qDebug("sizeof ObjectManager %#x %#x", sizeof(D3::ObjectManager), offsetof(D3::ObjectManager, x9C8_Scenes));
    qDebug("sizeof ActorCommonData %#x", sizeof(D3::ActorCommonData));
    qDebug("sizeof Container %#x", sizeof(D3::Container<void>));
    qDebug("sizeof ExpandableContainer %#x", sizeof(D3::ExpandableContainer<void>));
    qDebug("sizeof Scene %#x", sizeof(D3::Scene));
    qDebug("sizeof LocalData %#x", sizeof(D3::LocalData));
    qDebug("sizeof AssetScene %#x %#x", sizeof(D3::SceneSno), offsetof(D3::SceneSno, NavZone));
    qDebug("sizeof NavZoneDef %#x %#x", sizeof(D3::NavZoneDef), offsetof(D3::NavZoneDef, NavCells));
    qDebug("sizeof SnoDefinition %#x", sizeof(D3::SnoDefinition));
    qDebug("sizeof LevelArea %#x %#x", sizeof(D3::LevelArea), offsetof(D3::LevelArea, x044_SnoId));

    auto r = a.exec();

    timeEndPeriod(1);

    return r;
}
