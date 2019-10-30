#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    setlocale(LC_NUMERIC,"C");

    //Для сглаживания//
    QSurfaceFormat fmt;
    fmt.setSamples(16);
    QSurfaceFormat::setDefaultFormat(fmt);
    /**/

    MyWidget w;
    w.show();
    return a.exec();
}
