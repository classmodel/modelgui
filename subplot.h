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
#include <QRubberBand>

class plotarea : public QWidget
{
  Q_OBJECT

public:
  plotarea(QMap<int, modelrun> *, QList<int> *, QWidget *parent = 0);
  QMap<int, outputvar> xdatalist, ydatalist;
  QList<QColor> colors;
  QList<int> assignedcolors;
  QString saveImageName;
  QString xlabel, ylabel;
  QList<int> *selectedruns;

  int saveImageMode;
  int plotinterval;
  int lines;
  int legend_width, legend_height;
  int legend_x, legend_y;
  int legend_x_offset, legend_y_offset;

  // Mouse functions:
  int x_press, x_release, y_press, y_release;
  double x_current, y_current;
  bool mousepressed, mousereleased;
  bool legendmoved, legendmoves;

  double graphminx, graphmaxx, graphminy, graphmaxy;
  double xmin, xmax, ymin, ymax;
  double xmin_auto, xmax_auto;
  double yscale, xscale;

  bool autoaxis;
  bool scatterplot;
  bool MacOS;

  // Function to translate real coordinates (x,y) to widget coordinates
  double transfx(double x, double xscale, double xmin, int mode);
  double transfy(double y, double yscale, double ymin, int mode);

signals:
  void axischanged();
  void zoombymouse();
  void cursormoved();

public slots:
  void saveImage();

protected:
  void paintEvent(QPaintEvent *event);
  QMap<int, modelrun> *runlist;
  double PNGscale, PNGfontscale;

  void mousePressEvent( QMouseEvent * );
  void mouseReleaseEvent( QMouseEvent * );
  void mouseMoveEvent(QMouseEvent *);

private:
  int defaulttopmargin, defaultbottommargin, defaultleftmargin, defaultrightmargin;
  int topmargin, bottommargin, leftmargin, rightmargin;
  double nicenumber(double, bool);
  int plotwidget_width, plotwidget_height;
  int profinterval;
  QPoint origin_rubberband;
  QRubberBand *rubberBand;
  bool drawrubberband;

  double hscale_png,vscale_png;

};
#endif // SUBPLOT_H
