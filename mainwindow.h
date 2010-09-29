#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QModelIndex>
#include "modelrun.h"
#include "landsoil.h"

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
  QList<plotwindow*> plotwindowList;
  //void updateRunList();
  void createrun();
  void showGraph(QMap<int, modelrun> *, QList<int> *);
  plotwindow *graph;
  modelinput defaultinput;                     // Store default model input settings
  modelinput formvalues;
  Reaction defaultreactions[27];
  void loadfieldslots();
  int numgraphs;

public slots:
  void newrun();
  void clonerun();
  void deleteRun();
  void runTreeChanged();                       // Disable input field when selection runs > 1
  void runTreePressed(QModelIndex);
  void updateRunName(QString);
  void startrun();
  void canceledit();
  void startGraph();
  void saveRuns();
  void loadRuns();
  void exportRuns();
  void updateSurfacetype(int);
  void updateSoiltype(int);
  void tabChanged(int);
  void speciesselectionchanged();
  void setNoReactions();
  void setSimpleReactions();
  void setComplexReactions();
  void graphClosed();
  void showAbout();

  // Switches
  void switch_wind(int);
  void switch_sl(int);
  void switch_ls(int);
  void switch_sea(int);
  void switch_rad(int);
  void switch_ml(int);
  void switch_wtheta(int);
  void switch_wq(int);
  void switch_soil_advanced(int);
  void switch_surface_advanced(int);
  void switch_chem(int);
  void switch_chem_constant(int);
  void switch_photolysis(int);

signals:
  void rundeleted(int);
  void runadded(int);

private:
  Ui::MainWindow *ui;
  void readdefaultinput();
  void updateSelectedRuns();
  void storeFormData();
  void loadFormData();
  QString bool2string(const bool);
  void updateStatusBar();
  bool CheckState2bool(Qt::CheckState);
  Qt::CheckState Bool2CheckState(bool);
  int Bool2Int(bool);
  void setReactions(int);

  void initLandSoil();
  void setLandSoil();
  void closeEvent(QCloseEvent *event);
  void blockInput(bool);

  surfacetype surfacetypes[3];
  soiltype    soiltypes[1];

  int activerun;
  int activetab;

  int activespecies;
};

#endif // MAINWINDOW_H
