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
  //void updateRunList();
  void updateForm();
  void createrun();
  modelinput defaultinput;                     // Store default model input settings
  modelinput formvalues;

public slots:
  void wind_switch(int);
  void newrun();
  void clonerun();
  void deleteRun();
  void runTreeChanged();                       // Disable input field when selection runs > 1
  void updateInputdata();
  void updateRunName();

private:
  Ui::MainWindow *ui;
  void readdefaultinput();
};

#endif // MAINWINDOW_H
