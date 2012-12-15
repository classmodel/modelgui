//#include <QtGui/QApplication> // No longer working as of Qt5
#include <QtWidgets/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWindow main;
  main.show();
  return a.exec();
}
