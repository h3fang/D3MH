#include "minimap.h"
#include "ui_minimap.h"

#include <QPainter>
#include <QTimer>

#include "engine/engine.h"
#include "process/pointer.h"

Minimap::Minimap(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Minimap)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents);

    setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint/*|Qt::Tool*/);

    engine = Engine::getInstance();

    QTimer *t = new QTimer(this);
    connect(t, SIGNAL(timeout()), this, SLOT(update()));
    t->start(200);
}

Minimap::~Minimap()
{
    delete ui;
}

void Minimap::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // draw background
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(150, 150, 150, 32));

    p.drawRect(0, 0, width(),  height());

    // update d3 data
    updateD3Data();

    p.setPen(Qt::blue);
    p.setFont(QFont("Arial", 16));

    p.drawText(QRectF(0, 0, width(), height()), Qt::AlignCenter,
               QString::asprintf("FrameCnt:%d\nAppLoopCnt:%d\nWorldId:%d\nX:%.4f\nY:%.4f\nZ:%.4f",
                                 Pointer<int>()(Addr_ObjectManager, offsetof(D3::ObjectManager,x038_Counter_CurrentFrame)),
                                 engine->ApplicationLoopCount,
                                 engine->localData.x0C_WorldSnoId,
                                 engine->localData.x24_WorldPosX,
                                 engine->localData.x28_WorldPosY,
                                 engine->localData.x2C_WorldPosZ));
}

void Minimap::updateD3Data()
{
    if (!engine) return;

    engine->update();
}
