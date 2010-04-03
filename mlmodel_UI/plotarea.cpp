#include "plotarea.h"

plotarea::plotarea(QMap<int, modelrun> *givenrun, QList<int> *selectedr, QWidget *parent) : QWidget(parent)
{
  setBackgroundRole(QPalette::Base);
  setAutoFillBackground(true);
}

void plotarea::paintEvent(QPaintEvent * /* event */)
{
  QPainter paint(this);
  QPen pen(Qt::black, 1, Qt::SolidLine);
  paint.setPen(pen);
  paint.drawLine(0,0,150,150);
  std::cout << "PAINTEVENT" << std::endl;
}
