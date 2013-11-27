#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

//#include <QtGui/QWidget> // No longer working as of Qt5
//#include <QtGui/QTabWidget> // No longer working as of Qt5
#include <QtWidgets/QWidget>
#include <QtWidgets/QTabWidget>
#include "mainwindow.h"
#include "subplot.h"
#include "QPaintEvent"
#include "QPainter"
//#include "QTreeWidget" // No longer working as of Qt5
#include <QtWidgets/QTreeWidget>

class modelrun;
class subplot;

namespace Ui
{
  class plotwindow;
}

class plotwindow : public QMainWindow
{
  Q_OBJECT

public:
  plotwindow(QMap<int,modelrun> *, QList<int> *, QMainWindow *);
  ~plotwindow();
  plotarea *plotar;

  QList<int> *selectedruns;
  QList<int> *initialselected;
  QMap<int, modelrun> *runlist;
  QString plotvarx, plotvary;

public slots:
  void addrun(int num);
  void deleterun(int num);
  void updateselectedruns();
  void changeplotvar();
  void changeaxis();
  void zoomebymouse();
  void cursormoved();
  void switchtobasicplotting();
  void switchtoadvancedplotting();
  void viewmenutriggered();
  //void togglebasicsettings(bool);
  void toggleadvancedsettings(bool);
  void toggleaxissettings(bool);
  void togglemodelruns(bool);
  void selectadvanceddata(QTreeWidgetItem*,int);
  void changeplottype();

signals:
  void graphclosed(plotwindow* plot);

protected:
  void mouseMoveEvent(QMouseEvent *);

private:
  bool MacOS;
  Ui::plotwindow *ui;
  void closeEvent(QCloseEvent *event);
  QStringList outputnames;
  void updateplotdata();
  void getdata(outputvar*, modelrun, QString);
  void setplotvar(const QString, QString*);
};

#endif // PLOTWINDOW_H
