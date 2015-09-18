#include "minimap.h"
#include "ui_minimap.h"

#include <QPainter>

#include "engine/engine.h"

Minimap::Minimap(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Minimap)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents);

    setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint/*|Qt::Tool*/);

    Engine::getInstance();
}

Minimap::~Minimap()
{
    delete ui;
}

void Minimap::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(100,100,100, 127));

    painter.drawRect(0, 0, width(),  height());
}
