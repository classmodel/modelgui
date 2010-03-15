#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "modelrun.h"

namespace Ui
{
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = 0);
  ~MainWindow();
  QMap<int, modelrun> *modelrunlist;           // List containing objects with model output
  void updateRunList();

public slots:
  void wind_switch(int);
  void newrun();

private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
