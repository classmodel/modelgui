#ifndef PLOTAREA_H
#define PLOTAREA_H

#include "mainwindow.h"

#include <QWidget>
#include "QPaintEvent"
#include "QPainter"
#include <iostream>
#include <cmath>

class modelrun;

class plotarea : public QWidget
{
  Q_OBJECT

public:
  plotarea(QMap<int, modelrun> *, QList<int> *, QTabWidget *parent = 0);
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
