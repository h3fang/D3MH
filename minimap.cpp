#include "minimap.h"

#include <QPainter>
#include <QTimer>
#include <QShortcut>

#include <math.h>

#include "engine/engine.h"
#include "process/pointer.h"
#include "process/helper.h"

Minimap::Minimap(QWidget *parent) :
    QWidget(parent, Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint/*|Qt::Tool*/),
    d3Window(NULL),
    draw_minimap(false)
{
    setPalette(Qt::transparent);

    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents);

    engine = Engine::getInstance();

    QTimer *t = new QTimer(this);
    connect(t, SIGNAL(timeout()), this, SLOT(updateD3Data()));
    t->start(200);

    t = new QTimer(this);
    connect(t, SIGNAL(timeout()), this, SLOT(update()));
    t->start(200);
}

Minimap::~Minimap()
{
}

void Minimap::paintEvent(QPaintEvent *)
{
    repositionWindow();

    if (!d3Window || d3Window != GetForegroundWindow()) {
        return;
    }

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // draw background
    //    p.setPen(Qt::NoPen);
    //    p.setBrush(QColor(250, 150, 150, 128));
    //    p.drawRect(0, 0, width(),  height());

    drawInfo(p);
    drawMinimap(p);
}

void Minimap::updateD3Data()
{
    if (!engine) return;

    engine->update();
}

void Minimap::drawInfo(QPainter &p)
{
    p.setPen(QColor(0, 250, 0, 128));
    p.setFont(QFont("Arial", 8));

    p.drawText(QRectF(0, 0, 0.15*width(), 0.3*height()), Qt::AlignCenter,
               QString::asprintf("FrameCnt:%d\nAppLoopCnt:%d\nWorldSnoId:%d\nX:%.4f\nY:%.4f\nZ:%.4f\nSceneCnt:%d\nSnoSceneCnt:%d\n",
                                 Pointer<int>()(Addr_ObjectManager, offsetof(D3::ObjectManager,x038_Counter_CurrentFrame)),
                                 engine->ApplicationLoopCount,
                                 engine->localData.x0C_WorldSnoId,
                                 engine->localData.x24_WorldPosX,
                                 engine->localData.x28_WorldPosY,
                                 engine->localData.x2C_WorldPosZ,
                                 engine->navMesh->sceneData.size(),
                                 engine->navMesh->sceneSnoData.size()));
}

void Minimap::drawMinimap(QPainter &p)
{
    D3::AABB bounds = engine->navMesh->bounds;

    //    qDebug("Minimap bounds: min{%f, %f, %f}, max{%f, %f, %f}",
    //           bounds.min.x, bounds.min.y, bounds.min.z, bounds.max.x, bounds.max.y, bounds.max.z);

    float mult = std::min((width() - 20 + 5) / (bounds.max.x - bounds.min.x), (height() - 40) / (bounds.max.y - bounds.min.y));



    for (auto &s : engine->navMesh->sceneData) {

    }
}

void Minimap::repositionWindow()
{
    auto r = getD3ClientRect();
    if (r.isEmpty() || r == this->geometry()) {
        return;
    }
    else {
        this->setGeometry(r);
    }
}

QRect Minimap::getD3ClientRect()
{
    if (!d3Window) {
        d3Window = FindMainWindow(MemoryReader::instance()->getProcessId());
    }

    if (!d3Window) {
        qDebug("Failed to find D3 window");
        return QRect();
    }

    RECT r;

    if (!GetClientRect(d3Window, &r)) {
        return QRect();
    }

    ClientToScreen(d3Window, reinterpret_cast<POINT*>(&r.left));
    ClientToScreen(d3Window, reinterpret_cast<POINT*>(&r.right));

    return QRect(QPoint(r.left, r.top), QPoint(r.right, r.bottom));
}
