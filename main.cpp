#include "mainwindow.h"
#include <QApplication>
#include "./renderstate.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //RenderState *s = new RenderState();
    //s->show();

    return a.exec();
}
