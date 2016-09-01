#ifndef MINIMAP_H
#define MINIMAP_H

#include <QWidget>

#include <windows.h>

#include "modules/engine.h"

class Minimap : public QWidget
{
    Q_OBJECT

public:
    explicit Minimap(QWidget *parent = 0);
    ~Minimap();

protected:
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *e) override;
    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;

private slots:
    void repositionWindow();

private:
    void drawInfo(QPainter *p);
    void drawMinimap(QPainter *p);
    void drawCoordinates(QPainter *p);
    QRect getD3ClientRect();

    bool registerHotKeys();

private:
    QTransform minimapTransform;
    D3::Engine *engine;
    HWND d3Window;
    bool draw_minimap;
    bool size_changed;
    int hotkey_id;
};

#endif // MINIMAP_H
