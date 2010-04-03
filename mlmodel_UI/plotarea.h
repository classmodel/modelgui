#ifndef PLOTAREA_H
#define PLOTAREA_H

#include "mainwindow.h"

#include <QWidget>
#include "QPaintEvent"
#include "QPainter"
#include <iostream>

class modelrun;

class plotarea : public QWidget
{
  Q_OBJECT

public:
  plotarea(QMap<int, modelrun> *, QList<int> *, QWidget *parent = 0);
  QString plotvar;

public slots:

protected:
  void paintEvent(QPaintEvent *event);
  QList<int> *selectedruns;
  QMap<int, modelrun> *runlist;

private:

};

#endif // PLOTAREA_H
