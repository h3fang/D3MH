#include "minimap.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Minimap w;
    w.show();

    return a.exec();
}
