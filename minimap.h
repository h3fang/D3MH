#ifndef MINIMAP_H
#define MINIMAP_H

#include <QWidget>

namespace Ui {
class Minimap;
}

class Minimap : public QWidget
{
    Q_OBJECT

public:
    explicit Minimap(QWidget *parent = 0);
    ~Minimap();

    void paintEvent(QPaintEvent *event);

private:
    Ui::Minimap *ui;
};

#endif // MINIMAP_H
