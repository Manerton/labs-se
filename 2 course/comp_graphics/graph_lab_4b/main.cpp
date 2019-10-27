#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Для сглаживания//
    QGLFormat fmt;
    fmt.setSampleBuffers(true);
    fmt.setSamples(16); //2, 4, 8, 16
    QGLFormat::setDefaultFormat(fmt);
    /**/

    MyWidget w;
    w.show();
    return a.exec();
}
