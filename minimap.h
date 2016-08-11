#ifndef MINIMAP_H
#define MINIMAP_H

#include <QWidget>

#include <windows.h>

class Engine;

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

private:
    void drawInfo(QPainter *p);
    void drawMinimap(QPainter *p);
    void drawCoordinates(QPainter *p);

    void repositionWindow();
    QRect getD3ClientRect();

    bool registerHotKeys();

private:
    QTransform minimapTransform;
    Engine *engine;
    HWND d3Window;
    bool draw_minimap;
    bool size_changed;
    int hotkey_id;
};

#endif // MINIMAP_H
