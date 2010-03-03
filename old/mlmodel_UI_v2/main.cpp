#include <QtGui/QApplication>
#include "mlm_main.h"
#include "runwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mlm_main mainWin;
    mainWin.show();
    return a.exec();
}
