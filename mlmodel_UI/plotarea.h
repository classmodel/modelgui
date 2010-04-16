#ifndef PLOTAREA_H
#define PLOTAREA_H

#include <QtGui/QWidget>
#include "mainwindow.h"
#include <QWidget>
#include "QPaintEvent"
#include "QPainter"
#include <iostream>
#include <cmath>

//class modelrun;

namespace Ui
{
    class plotarea;
}

class plotarea : public QWidget
{
  Q_OBJECT

public:
  Ui::plotarea *ui;
  plotarea(QMap<int, modelrun> *, QList<int> *, QWidget *parent = 0);
  QString plotvar;
  double transfx(double xreal, double xscale, double xmin);    // Function real-coordinate to Widget-coordinate
  double transfy(double yreal, double yscale, double ymin);    // Function real-coordinate to Widget-coordinate

public slots:

protected:
  void paintEvent(QPaintEvent *event);
  QList<int> *selectedruns;
  QMap<int, modelrun> *runlist;

private:
  int plotmargin;
  int topmargin;
  int bottommargin;
  int leftmargin;
  int rightmargin;
  double nicenumber(double, bool);
};

#endif // PLOTAREA_H
