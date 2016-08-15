#include "minimap.h"

#include <QPainter>
#include <QTimer>
#include <QResizeEvent>
#include <QDir>
#include <QCoreApplication>

#include <math.h>

#include "process/pointer.h"
#include "process/helper.h"

float CANVAS_WIDTH = 1500.0f;
const float CANVAS_HEIGHT = 1200.0f;

Minimap::Minimap(QWidget *parent) :
    QWidget(parent, Qt::FramelessWindowHint | Qt::WindowTransparentForInput | Qt::WindowStaysOnTopHint),
    d3Window(NULL),
    draw_minimap(false),
    size_changed(false),
    hotkey_id(0)
{
    setAttribute(Qt::WA_TranslucentBackground);

    minimapTransform.translate(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2);
    minimapTransform.rotate(-45.0);
    minimapTransform.scale(-1.0, 1.0);

    QDir dir(QCoreApplication::applicationDirPath());
    dir.mkdir("cache");

    engine = D3::Engine::getInstance();
    registerHotKeys();

    showMaximized();

    QTimer *t = new QTimer(this); t->setTimerType(Qt::PreciseTimer);
    connect(t, SIGNAL(timeout()), this, SLOT(update()));
    t->start(50);

    t = new QTimer(this); t->setTimerType(Qt::PreciseTimer);
    connect(t, SIGNAL(timeout()), this, SLOT(repositionWindow()));
    t->start(500);
}

Minimap::~Minimap()
{
    if (d3Window) {
        CloseHandle(d3Window);
    }

    UnregisterHotKey((HWND)winId(), hotkey_id);
}

void Minimap::paintEvent(QPaintEvent *)
{
    if (!d3Window || d3Window != GetForegroundWindow()) {
        return;
    }

    if (engine) {
        engine->update();
    }

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

//    drawInfo(&p);
    drawMinimap(&p);
}

void Minimap::resizeEvent(QResizeEvent *e)
{
    auto s = this->size();

    CANVAS_WIDTH = ((float)s.width())/s.height()*CANVAS_HEIGHT;

    minimapTransform.reset();
    minimapTransform.scale(s.width()/CANVAS_WIDTH, s.height()/CANVAS_HEIGHT);
    minimapTransform.translate(CANVAS_WIDTH / 2.0, CANVAS_HEIGHT / 2.0);
    minimapTransform.rotate(-45.0);
    minimapTransform.scale(-1.0, 1.0);

    QWidget::resizeEvent(e);
}

bool Minimap::nativeEvent(const QByteArray &/*eventType*/, void *message, long */*result*/)
{
    MSG* m = (MSG*)message;
    if (m->message == WM_HOTKEY && HIWORD(m->lParam) == VK_TAB) {
        draw_minimap = !draw_minimap;
    }

    return false;
}

void Minimap::drawInfo(QPainter *p)
{
    p->setPen(QColor(0, 250, 0, 128));
    p->setFont(QFont("Arial", 16));

    // NOTE:offset
    p->drawText(QRectF(0, 0, 0.2*p->window().width(), 0.4*p->window().height()), Qt::AlignCenter,
               QString::asprintf("FrameCnt:%u\nAppLoopCnt:%u\nWorldSnoId:%d\nX:%.4f\nY:%.4f\nZ:%.4f\nSceneCnt:%u",
                                 Pointer<uint>()(D3::Addr_ObjectManager, offsetof(D3::ObjectManager,x038_Counter_CurrentFrame)),
                                 engine->ApplicationLoopCount,
                                 engine->localData.x0C_WorldSnoId,
                                 engine->localData.x24_WorldPosX,
                                 engine->localData.x28_WorldPosY,
                                 engine->localData.x2C_WorldPosZ,
                                 engine->navMesh->sceneData.size()));
}

void Minimap::drawMinimap(QPainter *p)
{
    if (!draw_minimap || !engine->isInGame()) {
        return;
    }

    p->save();

    p->setTransform(minimapTransform);

    p->save();
//    drawCoordinates(p);

    p->translate(-engine->localData.x24_WorldPosX, -engine->localData.x28_WorldPosY);

    QVector<QRectF> scene_cells, scene_grids;
    scene_cells.reserve(100*engine->navMesh->sceneData.size());

    for (const auto& pair : engine->navMesh->sceneData) {
        D3::SceneDataPtr s = pair.second;
        scene_grids.push_back(QRectF(s->min.x, s->min.y, s->max.x-s->min.x, s->max.y-s->min.y));

        auto ss = s->findSceneSnoData();
        if (!ss) {
            continue;
        }

        for (const D3::NavCell &c: ss->cells) {
            QRectF r_cell(c.min.x+s->min.x, c.min.y+s->min.y, c.max.x-c.min.x, c.max.y-c.min.y);
            scene_cells.push_back(r_cell);
        }
    }

//    p->setPen(QColor(0, 0, 255));
//    p->setBrush(QColor(0, 0, 255, 64));
//    p->drawRects(scene_grids);

    p->setPen(Qt::transparent);
    p->setBrush(QColor(0, 255, 0, 64));
    p->drawRects(scene_cells);

    p->setBrush(QColor(255, 0, 0, 196));
    for (const D3::ActorCommonData& acd : engine->acds) {
        if (acd.x184_ActorType == D3::ActorType_Monster) {
            p->drawEllipse(QPointF(acd.x0D0_WorldPosX, acd.x0D4_WorldPosY), 5, 5);
        }
    }

    p->restore();

    // self
    p->setBrush(QColor(255, 0, 0, 196));
    p->drawEllipse(QPoint(0, 0), 7, 7);

    p->restore();
}

void Minimap::drawCoordinates(QPainter *p)
{
    p->setPen(Qt::red);
    p->drawLine(0, 0, 150, 0);

    p->setPen(Qt::blue);
    p->drawLine(0, 0, 0, 150);
}

void Minimap::repositionWindow()
{
    auto r = getD3ClientRect();
    if (r.isNull() || r == this->geometry()) {
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
//        qDebug("Failed to find D3 window");
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

bool Minimap::registerHotKeys()
{
    hotkey_id = GlobalAddAtomA("DEMH");
    if (!RegisterHotKey((HWND)winId(), hotkey_id, 0, VK_TAB)) {
        qDebug("Failed to register hotkeys");
        return false;
    }

    return true;
}
