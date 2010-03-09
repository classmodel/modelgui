#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "modeloutput.h"
#include "modelinput.h"

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
  QMap<int, modelinput> *modelinputlist;           // List containing objects with model input
  QMap<int, modeloutput> *modeloutputlist;           // List containing objects with model output
  void updateRunList();

public slots:
  void wind_switch(int);

private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
