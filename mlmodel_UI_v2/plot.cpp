#include <QtGui>
#include "plot.h"
#include "mlm_main.h"
#include <cmath>
#include <iostream>
#include <QList>
#include <QStringList>

const int IdRole = Qt::UserRole;

Window::Window(QMap<int, rundata> *givenrun, QWidget *parent) : QWidget(parent)
{
  runs = givenrun;
  selectedruns = new QList<int>;

  // Make widgets
  renderArea = new RenderArea(runs, selectedruns, this);
  drawButton = new QPushButton(tr("draw"));
  updateListButton = new QPushButton(tr("update list"));
  xminInput = new QLineEdit(tr(""));
  xminLabel = new QLabel(tr("X-min:"));
  xminLabel->setBuddy(xminInput);
  xmaxInput = new QLineEdit(tr(""));
  xmaxLabel = new QLabel(tr("X-max:"));
  xmaxLabel->setBuddy(xmaxInput);
  yminInput = new QLineEdit(tr(""));
  yminLabel = new QLabel(tr("Y-min:"));
  yminLabel->setBuddy(yminInput);
  ymaxInput = new QLineEdit(tr(""));
  ymaxLabel = new QLabel(tr("Y-max:"));
  ymaxLabel->setBuddy(ymaxInput);

  yminInput->setDisabled(true);
  ymaxInput->setDisabled(true);
  xminInput->setDisabled(true);
  xmaxInput->setDisabled(true);

  // Overview modelruns
  QStringList heading;
  heading << "ID" << "Name";
  runlist = new QTreeWidget();
  runlist->setColumnCount(2);
  runlist->setHeaderLabels(heading);
  //runlist->setColumnWidth(0,30);
  runlist->hideColumn(0);                    // (DIS)/(EN)ABLE FOR HIDING ID COLUMN
  runlist->setFixedWidth(155);
  runlist->setSelectionMode(QAbstractItemView::ExtendedSelection);
  updaterunlist();


  // Buttons
  //connect(drawButton, SIGNAL(clicked()), this, SLOT(axisChanged()));
  connect(drawButton, SIGNAL(clicked()), this, SLOT(plotData()));
  connect(updateListButton, SIGNAL(clicked()), this, SLOT(updaterunlist()));

  // Draw layout
  QGridLayout *mainLayout = new QGridLayout;
  mainLayout->addWidget(runlist, 0,7,1,3);
  mainLayout->addWidget(updateListButton, 1,9);
  mainLayout->setColumnStretch(0, 1);
  mainLayout->setColumnStretch(6, 1);
  mainLayout->addWidget(renderArea, 0, 0, 7, 7);
  mainLayout->setRowMinimumHeight(1, 6);

  mainLayout->addWidget(xminLabel, 2,8, Qt::AlignRight);
  mainLayout->addWidget(xminInput, 2,9);
  mainLayout->addWidget(xmaxLabel, 3,8, Qt::AlignRight);
  mainLayout->addWidget(xmaxInput, 3,9);

  mainLayout->addWidget(yminLabel, 4,8, Qt::AlignRight);
  mainLayout->addWidget(yminInput, 4,9);
  mainLayout->addWidget(ymaxLabel, 5,8, Qt::AlignRight);
  mainLayout->addWidget(ymaxInput, 5,9);

  mainLayout->addWidget(drawButton, 6,9);
  setLayout(mainLayout);

  setWindowTitle(tr("ML-model plot"));
}

void Window::updaterunlist()
{\
  runlist->clear();
  QMap<int, rundata>::const_iterator i = runs->constBegin();
  while (i != runs->constEnd()) {
    QTreeWidgetItem *point = new QTreeWidgetItem(runlist);
    point->setCheckState(1,Qt::Unchecked);
    point->setFlags(Qt::ItemIsUserCheckable);
    point->setDisabled(false);
    point->setText(0, QString::number(i.key()));
    point->setText(1, runs->value(i.key()).name);
    ++i;
    }
}

void Window::axisChanged()
{
  double xmin = xminInput->text().toDouble();
  double xmax = xmaxInput->text().toDouble();
  double ymin = yminInput->text().toDouble();
  double ymax = ymaxInput->text().toDouble();
  renderArea->setAxis(xmin,xmax,ymin,ymax);
}

void Window::plotData()
{
  selectedruns->clear();
  int i=0;
  while(QTreeWidgetItem *item = runlist->topLevelItem(i)){
    if (item->checkState(1) == 2)
    {
      QString ident = item->text(0);
      int n = ident.toInt(0,10);
      selectedruns->insert(0,n);
    }
    i++;
  }
  renderArea->drawGraphs();
}

// =======================================
//
// =======================================

RenderArea::RenderArea(QMap<int, rundata> *givenrun, QList<int> *selectedr, QWidget *parent) : QWidget(parent)
{
  runlist = givenrun; // Data objects
  selectedruns = selectedr; // List with selected runs
  setBackgroundRole(QPalette::Base);
  setAutoFillBackground(true);
  axisset = false;
}

QSize RenderArea::minimumSizeHint() const
{
  return QSize(220, 220);
}

QSize RenderArea::sizeHint() const
{
  return QSize(400, 400);
}

void RenderArea::setAxis(double xmin, double xmax, double ymin, double ymax)
{
  axisset = true;
  this->xmin = xmin;
  this->xmax = xmax;
  this->ymin = ymin;
  this->ymax = ymax;
  update();
}

void RenderArea::drawGraphs()
{
  update();
}

double RenderArea::transfx(double xreal, double xscale, double xmin)
{
  double xwidget = ((xreal-xmin)*xscale) + plotmargin;
  return (xwidget);
}

double RenderArea::transfy(double yreal, double yscale, double ymin)
{
  int pwidget_height = geometry().height();
  double ywidget = pwidget_height -plotmargin - ((yreal-ymin)*yscale);
  return (ywidget);
}

void RenderArea::paintEvent(QPaintEvent * /* event */)
{
  if (selectedruns->count() > 0)
  {
      double xmin = 1e5, xmax = -1e3, ymin = 0, ymax = 0;
      for(int i=0; i<selectedruns->count(); i++)
      {
        int tstep = runlist->value(selectedruns->value(i)).tsteps;
        for(int m=0; m<tstep; m++)
          {
            if (runlist->value(selectedruns->value(i)).h[m] > ymax)
              ymax = runlist->value(selectedruns->value(i)).h[m];
            if (runlist->value(selectedruns->value(i)).h[m] < ymin)
              ymin = runlist->value(selectedruns->value(i)).h[m];
          }
        if (runlist->value(selectedruns->value(i)).t[0] < xmin)
          xmin = runlist->value(selectedruns->value(i)).t[0];
        if (runlist->value(selectedruns->value(i)).t[tstep-1] > xmax)
          xmax = runlist->value(selectedruns->value(i)).t[tstep-1];
        }

    xmin = std::floor(xmin);
    xmax = std::ceil(xmax);
    ymin = std::floor(ymin);
    ymax = std::ceil(ymax);

    // Size of widget (pixels)
    int pwidget_width = geometry().width();
    int pwidget_height = geometry().height();
    // Size of plotable area within widget (pixels)
    double plotwidth = pwidget_width - 2 * plotmargin;
    double plotheight = pwidget_height - 2 * plotmargin;

    double yscale = plotheight / (ymax-ymin);  // scaling factor for f(real-coordinate to Widget-coordinate)
    double xscale = plotwidth / (xmax-xmin);   // scaling factor for f(real-coordinate to Widget-coordinate)

    QPainter paint(this);
    QPen pen(Qt::black, 1, Qt::SolidLine);
    paint.setPen(pen);

    // Extra line through y=zero for negative y-values
    if (ymin < 0){
      paint.setPen(Qt::DashLine);
      paint.drawLine(transfx(xmin,xscale,xmin),transfy(0,yscale,ymin),transfx(xmax,xscale,xmin),transfy(0,yscale,ymin));
    }

    // Draw X and Y axis
    paint.setPen(Qt::SolidLine);
    paint.drawLine(plotmargin,pwidget_height - plotmargin,pwidget_width-plotmargin,pwidget_height-plotmargin);
    paint.drawLine(plotmargin,pwidget_height - plotmargin,plotmargin,plotmargin);  // Draw Y-axis

    // Draw labels Y-axis
    paint.drawText(5,plotmargin-5,40,13,Qt::AlignRight, QString::number(ymax));
    paint.drawText(5,(plotheight*0.25)+plotmargin-5,40,13,Qt::AlignRight, QString::number(ymax-((ymax-ymin)*0.25)));
    paint.drawText(5,(plotheight*0.5)+plotmargin-5,40,13,Qt::AlignRight, QString::number(ymax-((ymax-ymin)*0.50)));
    paint.drawText(5,(plotheight*0.75)+plotmargin-5,40,13,Qt::AlignRight, QString::number(ymax-((ymax-ymin)*0.75)));
    paint.drawText(5,pwidget_height-60,40,13,Qt::AlignRight, QString::number(ymin));

    // Draw labels X-axis
    paint.drawText(30,pwidget_height-45,40,13,Qt::AlignCenter, QString::number(xmin));
    paint.drawText((plotwidth*0.75)+plotmargin-20,pwidget_height-45,40,13,Qt::AlignCenter, QString::number((xmax-((xmax-xmin)*0.25))));
    paint.drawText((plotwidth*0.5)+plotmargin-20,pwidget_height-45,40,13,Qt::AlignCenter, QString::number((xmax-((xmax-xmin)*0.5))));
    paint.drawText((plotwidth*0.25)+plotmargin-20,pwidget_height-45,40,13,Qt::AlignCenter, QString::number((xmax-((xmax-xmin)*0.75))));
    paint.drawText(pwidget_width-70,pwidget_height-45,40,13,Qt::AlignCenter, QString::number(xmax));

    // Draw title
    paint.drawText((pwidget_width/2)-100,(plotmargin/2)-10,200,13,Qt::AlignCenter, "Something..");

    // Hereafter; clip data plot .
    paint.setClipping(true);
    paint.setClipRect(plotmargin,plotmargin,plotwidth,plotheight);


    QList<QColor> colors;
    colors << QColor(Qt::red) << QColor(Qt::blue) << QColor(Qt::darkGreen) << QColor(Qt::black) << QColor(Qt::magenta) << QColor(Qt::darkRed);

    for(int i=0; i<selectedruns->count(); i++)
    {
      int tstep = runlist->value(selectedruns->value(i)).tsteps;
      paint.setPen( colors.value(i) );
      for(int m=0; m<tstep-1; m++)
        {
          paint.drawLine(transfx((runlist->value(selectedruns->value(i)).t[m]),xscale,xmin),
                         transfy((runlist->value(selectedruns->value(i)).h[m]),yscale,ymin),
                         transfx((runlist->value(selectedruns->value(i)).t[m+1]),xscale,xmin),
                         transfy((runlist->value(selectedruns->value(i)).h[m+1]),yscale,ymin));
        }
      }
  }
}
