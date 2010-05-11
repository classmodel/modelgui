#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include <QtGui/QWidget>
#include <QtGui/QTabWidget>
#include "mainwindow.h"
#include "subplot.h"
#include "QPaintEvent"
#include "QPainter"

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
  QString plotvar;

public slots:
  void addrun(int num);
  void deleterun(int num);
  void updateselectedruns();
  void changeplotvar();
  void changeaxis();
  void switchtobasicplotting();
  void switchtoadvancedplotting();

protected:

private:
  Ui::plotwindow *ui;
  QStringList outputnames;
};

#endif // PLOTWINDOW_H
