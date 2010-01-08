#include <QtGui/QApplication>
#include "plotwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PlotWindow w;
    w.show();
    return a.exec();
}
