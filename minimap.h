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

private slots:
    void updateD3Data();

private:
    void drawInfo(QPainter &p);
    void drawMinimap(QPainter &p);

    void repositionWindow();
    QRect getD3ClientRect();

private:
    Engine *engine;

    HWND d3Window;

    bool draw_minimap;
};

#endif // MINIMAP_H
