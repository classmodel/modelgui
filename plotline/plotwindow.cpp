#include "plotwindow.h"
#include "ui_plotwindow.h"
#include <qpainter.h>
#include <QPaintEvent>
#include <QRectF>
#include <QImage>
#include <math.h>
#include <iostream>
using namespace std;

PlotWindow::PlotWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::PlotWindow)
{
  ui->setupUi(this);
  setMinimumSize(250,250);
  setSizeIncrement(20,20);
  setWindowTitle(tr("mlmodel data plot"));
}

double PlotWindow::transfx(double xreal, double xscale, double xmin)
{
  double xwidget = ((xreal-xmin)*xscale) + plotmargin;
  return (xwidget);
}

double PlotWindow::transfy(double yreal, double yscale, double ymin)
{
  int pwidget_height = geometry().height();
  double ywidget = pwidget_height -plotmargin - ((yreal-ymin)*yscale);
  return (ywidget);
}

void PlotWindow::resizeEvent(QResizeEvent *) // Redraw widget after scaling the widget
{
  update();
}

void PlotWindow::paintEvent(QPaintEvent *)
{
  // Size of widget (pixels)
  int pwidget_width = geometry().width();
  int pwidget_height = geometry().height();
  // Size of plotable area within widget (pixels)
  double plotwidth = pwidget_width - 2 * plotmargin;
  double plotheight = pwidget_height - 2 * plotmargin;

  double xmin = 0;                           // min X-value
  double ymin = 0;                           // min Y-value
  double xmax = 4 * 3.14159;                 // max X-value
  double ymax = 1;                           // max Y-value
  double xstep = 0.1;                        // x-interval for loop
  double yscale = plotheight / (ymax-ymin);  // scaling factor for f(real-coordinate to Widget-coordinate)
  double xscale = plotwidth / (xmax-xmin);   // scaling factor for f(real-coordinate to Widget-coordinate)

  QPainter paint(this);
  QPen pen(Qt::black, 1, Qt::SolidLine);
  paint.setPen(pen);

  paint.drawLine(plotmargin,pwidget_height - plotmargin,pwidget_width-plotmargin,pwidget_height-plotmargin);  // Draw X-axis
  paint.drawLine(plotmargin,pwidget_height - plotmargin,plotmargin,plotmargin);  // Draw Y-axis

  // Set labels Y-axis, scalable
  paint.drawText(5,plotmargin-5,40,13,Qt::AlignRight, QString::number(ymax));
  paint.drawText(5,(plotheight*0.25)+plotmargin-5,40,13,Qt::AlignRight, QString::number(ymax-((ymax-ymin)*0.25)));
  paint.drawText(5,(plotheight*0.5)+plotmargin-5,40,13,Qt::AlignRight, QString::number(ymax-((ymax-ymin)*0.50)));
  paint.drawText(5,(plotheight*0.75)+plotmargin,40,13,Qt::AlignRight, QString::number(ymax-((ymax-ymin)*0.75)));
  paint.drawText(5,pwidget_height-55,40,13,Qt::AlignRight, QString::number(ymin));

  // Hereafter; clip data plot
  paint.setClipping(true);
  paint.setClipRect(plotmargin,plotmargin,plotwidth,plotheight);

  paint.setPen( Qt::blue );
  for (double i=xmin; i<xmax; i+=xstep)
  {
    paint.drawLine(transfx(i,xscale,xmin),transfy(cos(i),yscale,ymin),transfx((i+xstep),xscale,xmin),transfy(cos(i+xstep),yscale,ymin));
  }

}

PlotWindow::~PlotWindow()
{
    delete ui;
}
