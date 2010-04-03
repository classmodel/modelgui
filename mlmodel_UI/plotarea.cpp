#include "plotarea.h"

plotarea::plotarea(QMap<int, modelrun> *runs, QList<int> *selected, QWidget *parent) : QWidget(parent)
{
  selectedruns = selected;       // Needed?
  runlist = runs;
  plotvar = "h";
  setBackgroundRole(QPalette::Base);
  setAutoFillBackground(true);
}

void plotarea::paintEvent(QPaintEvent * /* event */)
{
  if (selectedruns->count() > 0)
  {
    QPainter paint(this);
    QPen pen(Qt::black, 1, Qt::SolidLine);
    paint.setPen(pen);
    paint.drawLine(0,0,1000,1000);
  }
}
