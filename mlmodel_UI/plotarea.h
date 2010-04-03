#ifndef PLOTAREA_H
#define PLOTAREA_H

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

public slots:

protected:
  void paintEvent(QPaintEvent *event);

private:

};

#endif // PLOTAREA_H
