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
#include <QPen>
#include <QPointF>

class plotarea : public QWidget
{
  Q_OBJECT

public:
  plotarea(QMap<int, modelrun> *, QList<int> *, QWidget *parent = 0);
  QString plotvar;
  QList<QColor> colors;
  QList<int> assignedcolors;
  int saveImageMode;
  QString saveImageName;
  std::string xlabel, ylabel;
  int plotinterval;
  double graphminx, graphmaxx, graphminy, graphmaxy;
  double xmin, xmax, ymin, ymax;
  bool autoaxis;

  // Function to translate real coordinates (x,y) to widget coordinates
  double transfx(double xreal, double xscale, double xmin);
  double transfy(double yreal, double yscale, double ymin);

signals:
  void axischanged();

public slots:
  void saveImage();

protected:
  void paintEvent(QPaintEvent *event);
  QList<int> *selectedruns;
  QMap<int, modelrun> *runlist;
  double PNGscale;

private:
  int defaulttopmargin, defaultbottommargin, defaultleftmargin, defaultrightmargin;
  int topmargin, bottommargin, leftmargin, rightmargin;
  double nicenumber(double, bool);
  int plotwidget_width, plotwidget_height;
};
#endif // SUBPLOT_H
