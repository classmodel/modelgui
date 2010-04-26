#ifndef SUBPLOT_H
#define SUBPLOT_H

#include <QtGui/QWidget>
#include "mainwindow.h"
#include <QWidget>
#include "QPaintEvent"
#include "QPainter"
#include "QPrinter"
#include "QGraphicsScene"
#include <iostream>
#include <cmath>
#include <QFileDialog>
#include <QPaintDevice>
#include <QFont>

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
  std::string xlabel, ylabel;
  double graphminx, graphmaxx, graphminy, graphmaxy;
  double xmin, xmax, ymin, ymax;
  bool autoaxis;

  int saveImageMode;
  QString saveImageName;

signals:
  void axischanged();

public slots:
  void saveImage();

protected:
  void paintEvent(QPaintEvent *event);
  QList<int> *selectedruns;
  QMap<int, modelrun> *runlist;

private:
  int plotmargin, topmargin, bottommargin, leftmargin, rightmargin;
  double nicenumber(double, bool);
  int plotwidget_width, plotwidget_height;
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
  void changeaxis();

protected:
  QList<int> *selectedruns;
  QMap<int, modelrun> *runlist;

private:
};

#endif // SUBPLOT_H
