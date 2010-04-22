#include <cmath>
#include "subplot.h"
#include "ui_subplot.h"

subplot::subplot(QMap<int, modelrun> *runs, QList<int> *selected, QWidget *parent) : QWidget(parent), ui(new Ui::subplot)
{
  ui->setupUi(this);
  selectedruns    = selected;
  runlist         = runs;

  plotar = new plotarea(runlist,selectedruns,this);
  connect(plotar, SIGNAL(axischanged()), this, SLOT(changeaxis()));
  connect(ui->autoscaleaxis, SIGNAL(clicked(bool)), this, SLOT(changeaxis()));
  connect(ui->autoscaleaxis, SIGNAL(clicked(bool)), plotar, SLOT(update()));
  
  connect(ui->xminInput, SIGNAL(textEdited(QString)), this, SLOT(changeaxis()));
  connect(ui->xmaxInput, SIGNAL(textEdited(QString)), this, SLOT(changeaxis()));
  connect(ui->yminInput, SIGNAL(textEdited(QString)), this, SLOT(changeaxis()));
  connect(ui->ymaxInput, SIGNAL(textEdited(QString)), this, SLOT(changeaxis()));

  QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  sizePolicy.setHorizontalStretch(0);
  sizePolicy.setVerticalStretch(0);
  sizePolicy.setHeightForWidth(plotar->sizePolicy().hasHeightForWidth());
  plotar->setSizePolicy(sizePolicy);
  plotar->setMinimumSize(QSize(300, 300));
  ui->plotLayout->addWidget(plotar);

  ui->autoscaleaxis->setChecked(true);
}

void subplot::changeaxis()
{
  if (ui->autoscaleaxis->checkState() == Qt::Checked)
  {
    plotar->autoaxis = true;
    ui->xminInput->setText(QString::number(plotar->graphminx));
    ui->xmaxInput->setText(QString::number(plotar->graphmaxx));
    ui->yminInput->setText(QString::number(plotar->graphminy));
    ui->ymaxInput->setText(QString::number(plotar->graphmaxy));
  }
  else
  {
    plotar->autoaxis = false;
    plotar->xmin = ui->xminInput->text().toDouble();
    plotar->xmax = ui->xmaxInput->text().toDouble();
    plotar->ymin = ui->yminInput->text().toDouble();
    plotar->ymax = ui->ymaxInput->text().toDouble();
    plotar->update();
  }

  ui->xminInput->setDisabled(plotar->autoaxis);
  ui->xmaxInput->setDisabled(plotar->autoaxis);
  ui->yminInput->setDisabled(plotar->autoaxis);
  ui->ymaxInput->setDisabled(plotar->autoaxis);
}

// ++++++++++++++++++++++++++++++
//
// ++++++++++++++++++++++++++++++

plotarea::plotarea(QMap<int, modelrun> *runs, QList<int> *selected, QWidget *parent) : QWidget(parent)
{
  selectedruns    = selected;
  runlist         = runs;
  setBackgroundRole(QPalette::Base);
  setAutoFillBackground(true);

  plotvar         = "h";
  topmargin       = 30;
  bottommargin    = 50;
  leftmargin      = 70;
  rightmargin     = 30;
  autoaxis        = false;
}

double plotarea::transfx(double xreal, double xscale, double xmin)
{
  double xwidget = ((xreal-xmin)*xscale) + leftmargin;
  return (xwidget);
}

double plotarea::transfy(double yreal, double yscale, double ymin)
{
  int pwidget_height = geometry().height();
  double ywidget = pwidget_height - bottommargin - ((yreal-ymin)*yscale);
  return (ywidget);
}

double plotarea::nicenumber(double x, bool round)
{
  int exp;
  double f, nf;

  exp = std::floor(log10(x));
  f   = x / std::pow(10., exp);

  if(round)
  {
    if(f < 1.5)
      nf = 1.;
    else if(f < 3.)
      nf = 2.;
    else if(f < 7.)
      nf = 5.;
    else
      nf = 10.;
  }
  else
  {
    if(f <= 1.)
      nf = 1.;
    else if(f <= 2.)
      nf = 2.;
    else if(f <= 5.)
      nf = 5.;
    else
      nf = 10.;
  }
  return nf * std::pow(10., exp);
}

void plotarea::paintEvent(QPaintEvent * /* event */)
{
  if (selectedruns->count() > 0)
  {
    xlabel = runlist->value(selectedruns->value(0)).run->output->t.description;
    xlabel.append(" [");
    xlabel.append(runlist->value(selectedruns->value(0)).run->output->t.unit);
    xlabel.append("]");

    if (autoaxis)
    {
      xmin = 1e5;
      xmax = -1e3;
      ymin = 1e5;
      ymax = 0;

      for(int i=0; i<selectedruns->count(); i++)
      {
        double *tempplotvar = new double;
        if (plotvar == "h")
        {
          tempplotvar = runlist->value(selectedruns->value(i)).run->output->h.data;
          ylabel = runlist->value(selectedruns->value(i)).run->output->h.description;
          ylabel.append(" [");
          ylabel.append(runlist->value(selectedruns->value(i)).run->output->h.unit);
          ylabel.append("]");          
        }
        else if (plotvar == "theta")
          tempplotvar = runlist->value(selectedruns->value(i)).run->output->theta.data;
        else if (plotvar == "dtheta")
          tempplotvar = runlist->value(selectedruns->value(i)).run->output->dtheta.data;
        else if (plotvar == "wtheta")
          tempplotvar = runlist->value(selectedruns->value(i)).run->output->wtheta.data;
        else if (plotvar == "q")
          tempplotvar = runlist->value(selectedruns->value(i)).run->output->q.data;
        else if (plotvar == "dq")
          tempplotvar = runlist->value(selectedruns->value(i)).run->output->dq.data;
        else if (plotvar == "wq")
          tempplotvar = runlist->value(selectedruns->value(i)).run->output->wq.data;

        int tsteps = int(runlist->value(selectedruns->value(i)).run->input.runtime / runlist->value(selectedruns->value(i)).run->input.dt) + 1;

        for(int m=0; m<tsteps; m++)
        {
          if (tempplotvar[m] > ymax)
            ymax = tempplotvar[m];
          if (tempplotvar[m] < ymin)
            ymin = tempplotvar[m];
        }
        if (runlist->value(selectedruns->value(i)).run->output->t.data[0] < xmin)
          xmin = runlist->value(selectedruns->value(i)).run->output->t.data[0];
        if (runlist->value(selectedruns->value(i)).run->output->t.data[tsteps-1] > xmax)
          xmax = runlist->value(selectedruns->value(i)).run->output->t.data[tsteps-1];
      }
    }

    // Size of widget (pixels)
    int plotwidget_width = geometry().width();
    int plotwidget_height = geometry().height();

    // Size of plotable area within widget (pixels)
    double plotwidth = plotwidget_width - leftmargin - rightmargin;
    double plotheight = plotwidget_height - topmargin - bottommargin;

    // ------------------------------------
    // Start drawing
    // ------------------------------------
    QPainter paint(this);
    QPen pen(Qt::black, 1, Qt::SolidLine);
    paint.setPen(pen);

    // Draw X and Y axis
    paint.setPen(Qt::SolidLine);
    paint.drawLine(leftmargin,plotwidget_height - bottommargin,plotwidget_width-rightmargin,plotwidget_height-bottommargin);
    paint.drawLine(leftmargin,plotwidget_height - bottommargin,leftmargin,topmargin);

    int nticks = 8;
    int nfrac;
    double d;
    double range, x, y;

    // Draw labels Y-axis
    // In case of constant values, draw line in center
    if(ymax == ymin)
    {
      ymax = ymax + 0.1 * ymax;
      ymin = ymin - 0.1 * ymin;
    }

    range     = nicenumber(ymax - ymin, false);
    d         = nicenumber(range / ((double)nticks - 1.), true);
    graphminy = std::floor(ymin / d) * d;
    graphmaxy = std::ceil(ymax / d) * d;
    nfrac     = std::max(-1 * (int)std::floor(std::log10(d)), 0);

    for(y = graphminy; y <= graphmaxy + .5 * d; y = y + d)
      paint.drawText((leftmargin-50),(plotheight * ((graphmaxy - y) / (graphmaxy - graphminy)))+topmargin-5,40,13,Qt::AlignRight, QString::number(y,'f',nfrac));

    // Draw labels X-axis
    range     = nicenumber(xmax - xmin, false);
    d         = nicenumber(range / ((double)nticks - 1.), true);
    graphminx = std::floor(xmin / d) * d;
    graphmaxx = std::ceil(xmax / d) * d;
    nfrac     = std::max(-1 * (int)std::floor(std::log10(d)), 0);

    for(x = graphminx; x <= graphmaxx + .5 * d; x = x + d)
    {
      paint.drawText(((plotwidth * (x-graphminx))/(graphmaxx - graphminx))+leftmargin-20,plotwidget_height-bottommargin+8,40,13,Qt::AlignCenter, QString::number(x,'f',nfrac));
    } 

    // Axis labels
    paint.drawText((plotwidth / 2) + leftmargin - 150,plotwidget_height - bottommargin + 28,300,20,Qt::AlignHCenter, QString::fromStdString(xlabel));
    paint.rotate(270);
    //paint.drawText(-350,5,300,20,Qt::AlignCenter, QString::fromStdString(ylabel));
    std::cout << -((plotheight / 2) + topmargin + 150) << std::endl;
    paint.drawText(-((plotheight / 2) + topmargin + 150),5,300,20,Qt::AlignCenter, QString::fromStdString(ylabel));
    paint.rotate(90);

    // Hereafter; clip data plot .
    paint.setClipping(true);
    paint.setClipRect(leftmargin+1,topmargin+1,plotwidth-2,plotheight-2);

    QList<QColor> colors;
    colors << QColor(Qt::blue) << QColor(Qt::darkGreen) << QColor(Qt::red) << QColor(Qt::cyan) << QColor(Qt::magenta) << QColor(Qt::yellow) << QColor(Qt::black);

    int legendy = topmargin;

    for(int i=0; i<selectedruns->count(); i++)
    {
      double *tempplotvar = new double;

      if (plotvar == "h")
        tempplotvar = runlist->value(selectedruns->value(i)).run->output->h.data;
      else if (plotvar == "theta")
        tempplotvar = runlist->value(selectedruns->value(i)).run->output->theta.data;
      else if (plotvar == "dtheta")
        tempplotvar = runlist->value(selectedruns->value(i)).run->output->dtheta.data;
      else if (plotvar == "wtheta")
        tempplotvar = runlist->value(selectedruns->value(i)).run->output->wtheta.data;
      else if (plotvar == "q")
        tempplotvar = runlist->value(selectedruns->value(i)).run->output->q.data;
      else if (plotvar == "dq")
        tempplotvar = runlist->value(selectedruns->value(i)).run->output->dq.data;
      else if (plotvar == "wq")
        tempplotvar = runlist->value(selectedruns->value(i)).run->output->wq.data;
      int tsteps = int(runlist->value(selectedruns->value(i)).run->input.runtime / runlist->value(selectedruns->value(i)).run->input.dt) + 1;

      double yscale = plotheight / (graphmaxy-graphminy);   // scaling factor for f(real-coordinate to Widget-coordinate)
      double xscale = plotwidth  / (graphmaxx-graphminx);   // scaling factor for f(real-coordinate to Widget-coordinate)

      paint.setPen( colors.value(i) );
      for(int m=0; m<tsteps-1; m=m+5)
      {
        paint.drawLine(transfx((runlist->value(selectedruns->value(i)).run->output->t.data[m]),xscale,graphminx),
                       transfy((tempplotvar[m]),yscale,graphminy),
                       transfx((runlist->value(selectedruns->value(i)).run->output->t.data[m+5]),xscale,graphminx),
                       transfy((tempplotvar[m+5]),yscale,graphminy));
      }

      paint.drawLine(leftmargin+5,legendy+8,leftmargin+20,legendy+8);
      paint.drawText(leftmargin+25,legendy,200,13,Qt::AlignLeft, runlist->value(selectedruns->value(i)).runname);
      legendy = legendy+15;
    }
  }

  if (autoaxis)
    emit axischanged();
}



