#include "autoloot.h"

#include <algorithm>

#include <QDebug>

#include <windows.h>

#include "engine.h"
#include "utils/keyboardmouse.h"
#include "utils/hook.h"

namespace D3 {

AutoLoot::AutoLoot(Engine *e) :
    engine(e),
    kbm(new KeyboardMouse()),
    hook(new Hook())
{
    screen_width = GetSystemMetrics(SM_CXSCREEN);
    screen_height = GetSystemMetrics(SM_CYSCREEN);
    qDebug("Screen resolution w x h: %d x %d", screen_width, screen_height);
}

AutoLoot::~AutoLoot()
{
    delete kbm;
    delete hook;
}

void AutoLoot::loot()
{
    auto items = getItemsToLoot();

    std::map<uint, int> loot_count;

    // block mouse input while looting
    block_mouse_events = true;

    while (!items.empty()) {
        std::sort(items.begin(), items.end(), [](const Item& lhs, const Item& rhs) {
            return lhs.distance < rhs.distance;
        });

        Point p(-1, -1);

        for (const Item& item:items) {
            // skip items that are handled 3 times
            if (loot_count[item.actor_id] >= 3) {
                continue;
            }

            p = toScreenCoordinate(item.x, item.y, item.z, &engine->localData, screen_width, screen_height);

            loot_count[item.actor_id] += 1;

            // continue if point is invalid
            if (p.x < 0 || p.x >screen_width || p.y < 0 || p.y >screen_height) {
                p.x = -1;
                p.y = -1;
                continue;
            }

            break;
        }

        // no valid item or tried more than 3 times for valid items
        if ( p.x < 0 && p.y < 0) {
            break;
        }

        kbm->mouseMove(p);
        kbm->mouseClick(MouseKey::LEFT_BUTTON);

        items = getItemsToLoot();
    }

    block_mouse_events = false;
}

std::vector<Item> AutoLoot::getItemsToLoot()
{
    std::vector<Item> result;
    result.reserve(10);

    const auto x = engine->localData.x24_WorldPosX;
    const auto y = engine->localData.x28_WorldPosY;
    const auto z = engine->localData.x2C_WorldPosZ;

    float d = 0.0f;

    for (const ActorCommonData& acd : engine->acds) {
        if (acd.x000_Id == INVALID_ID ||
                acd.x184_ActorType != int(ActorType::Item) ||
                acd.x114_ItemLocation != ItemLoc_UnknownLoc ||
                acd.x180_GizmoType != int(GizmoType::Item) ||
                (d = distance(x, y, z, acd.x0D0_WorldPosX, acd.x0D4_WorldPosY, acd.x0D8_WorldPosZ))> MAX_LOOT_RADIUS) {
            continue;
        }

        if (!isGem(acd) && !isMaterial(acd)) {
            //continue;
        }

        QString str = QString::fromUtf8(acd.x004_Name);

        if (str.contains("gold_", Qt::CaseInsensitive) ||
                str.contains("cube_", Qt::CaseInsensitive) ||
                str.contains("book_", Qt::CaseInsensitive) ||
                str.contains("forge_", Qt::CaseInsensitive)) {
            continue;
        }

        qDebug() << "Item Name:" << str
                 << ", Distance:" << d
                 << ", ActorId:" << acd.x08C_ActorId
                 << ", ActorSnoId:" << acd.x090_ActorSnoId;

        Item item;
        item.actor_id = acd.x000_Id;
        item.x = acd.x0D0_WorldPosX;
        item.y = acd.x0D4_WorldPosY;
        item.z = acd.x0D8_WorldPosZ;
        item.distance = d;

        result.push_back(item);
    }

    return result;
}

}
