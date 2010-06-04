#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "modelrun.h"
//#include "plotwindow.h"

class plotwindow;

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
  QList<int> *selectedruns;
  //void updateRunList();
  void updateForm();
  void createrun();
  void showGraph(QMap<int, modelrun> *, QList<int> *);
  plotwindow *graph;
  modelinput defaultinput;                     // Store default model input settings
  modelinput formvalues;
  void loadfieldslots();

public slots:
  void newrun();
  void clonerun();
  void deleteRun();
  void runTreeChanged();                       // Disable input field when selection runs > 1
  void updateInputdata();
  void updateRunName();
  void startrun();
  void canceledit();
  void startGraph();
  void exportRuns();
  // Switches
  void switch_wind(int);
  void switch_sl(int);
  void switch_ls(int);
  void switch_rad(int);
  void switch_ml(int);

  //void switches_changed();

signals:
  void rundeleted(int);
  void runadded(int);

private:
  Ui::MainWindow *ui;
  void readdefaultinput();
  void updateSelectedRuns();
};

#endif // MAINWINDOW_H
