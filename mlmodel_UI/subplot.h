#ifndef SUBPLOT_H
#define SUBPLOT_H

#include <QtGui/QWidget>
#include "mainwindow.h"
#include <QWidget>
#include "QPaintEvent"
#include "QPainter"
#include <iostream>
#include <cmath>

// ++++++++++++++++++++++++++
// Actual plot area
// ++++++++++++++++++++++++++

class plotarea : public QWidget
{
  Q_OBJECT

public:
  plotarea(QMap<int, modelrun> *, QList<int> *, QWidget *parent = 0);
  double transfx(double xreal, double xscale, double xmin);    // Function real-coordinate to Widget-coordinate
  double transfy(double yreal, double yscale, double ymin);    // Function real-coordinate to Widget-coordinate
  QString plotvar;

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

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

namespace Ui
{
    class subplot;
}

class subplot : public QWidget
{
  Q_OBJECT

public:
  Ui::subplot *ui;
  plotarea *plotar;
  subplot(QMap<int, modelrun> *, QList<int> *, QWidget *parent = 0);
  QString plotvar;

public slots:

protected:
  QList<int> *selectedruns;
  QMap<int, modelrun> *runlist;

private:
};

#endif // SUBPLOT_H
