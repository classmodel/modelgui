#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include <QtGui/QWidget>
#include <QtGui/QTabWidget>
#include "mainwindow.h"
#include "plotarea.h"
#include "QPaintEvent"
#include "QPainter"

class modelrun;

namespace Ui
{
  class plotwindow;
}

class plotwindow : public QTabWidget
{
  Q_OBJECT

public:
  plotwindow(QMap<int,modelrun> *, QMainWindow *);
  ~plotwindow();
  QList<int> *selectedruns;
  QMap<int, modelrun> *runlist;

public slots:
  void deleterun(int num);
  void updateselectedruns();
  void changeplotvar();

protected:

private:
  Ui::plotwindow *ui;
  plotarea *plot;
  QStringList outputnames;
};

#endif // PLOTWINDOW_H
