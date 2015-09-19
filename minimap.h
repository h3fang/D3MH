#ifndef MINIMAP_H
#define MINIMAP_H

#include <QWidget>

namespace Ui {
class Minimap;
}

class Engine;

class Minimap : public QWidget
{
    Q_OBJECT

public:
    explicit Minimap(QWidget *parent = 0);
    ~Minimap();

    void paintEvent(QPaintEvent *event);

private slots:
    void updateD3Data();

private:
    Ui::Minimap *ui;
    Engine *engine;
};

#endif // MINIMAP_H
