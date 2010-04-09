#include <cmath>
#include "plotarea.h"

plotarea::plotarea(QMap<int, modelrun> *runs, QList<int> *selected, QWidget *parent) : QWidget(parent)
{
  selectedruns    = selected;
  runlist         = runs;
  setBackgroundRole(QPalette::Base);
  setAutoFillBackground(true);

  plotvar         = "h";
  topmargin       = 40;
  bottommargin    = 60;
  leftmargin      = 60;
  rightmargin     = 30;
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
    double xmin = 1e5, xmax = -1e3, ymin = 1e5, ymax = 0;
    for(int i=0; i<selectedruns->count(); i++)
    {
      double *tempplotvar = new double;
      if (plotvar == "h")
        tempplotvar = runlist->value(selectedruns->value(i)).run->output->h;
      else if (plotvar == "theta")
        tempplotvar = runlist->value(selectedruns->value(i)).run->output->theta;
      else if (plotvar == "dtheta")
        tempplotvar = runlist->value(selectedruns->value(i)).run->output->dtheta;
      else if (plotvar == "wtheta")
        tempplotvar = runlist->value(selectedruns->value(i)).run->output->wtheta;
      else if (plotvar == "q")
        tempplotvar = runlist->value(selectedruns->value(i)).run->output->q;
      else if (plotvar == "dq")
        tempplotvar = runlist->value(selectedruns->value(i)).run->output->dq;
      else if (plotvar == "wq")
        tempplotvar = runlist->value(selectedruns->value(i)).run->output->wq;

      int tsteps = int(runlist->value(selectedruns->value(i)).run->input.runtime / runlist->value(selectedruns->value(i)).run->input.dt) + 1;

      for(int m=0; m<tsteps; m++)
      {
        if (tempplotvar[m] > ymax)
          ymax = tempplotvar[m];
        if (tempplotvar[m] < ymin)
          ymin = tempplotvar[m];
      }
      if (runlist->value(selectedruns->value(i)).run->output->t[0] < xmin)
        xmin = runlist->value(selectedruns->value(i)).run->output->t[0];
      if (runlist->value(selectedruns->value(i)).run->output->t[tsteps-1] > xmax)
        xmax = runlist->value(selectedruns->value(i)).run->output->t[tsteps-1];
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

    double graphminx, graphmaxx;
    double graphminy, graphmaxy;
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
      paint.drawText((plotwidth * x/(graphmaxx - graphminx))+leftmargin-20,plotwidget_height-bottommargin+8,40,13,Qt::AlignCenter, QString::number(x,'f',nfrac));

    // Draw title
    paint.drawText((plotwidget_width/2)-100,(topmargin/2)-10,200,13,Qt::AlignCenter, "Something..");

    // Hereafter; clip data plot .
    paint.setClipping(true);
    paint.setClipRect(leftmargin,topmargin,plotwidth,plotheight);


    QList<QColor> colors;
    colors << QColor(Qt::red) << QColor(Qt::blue) << QColor(Qt::darkGreen) << QColor(Qt::black) << QColor(Qt::magenta) << QColor(Qt::darkRed);

    for(int i=0; i<selectedruns->count(); i++)
    {
      double *tempplotvar = new double;
      if (plotvar == "h")
        tempplotvar = runlist->value(selectedruns->value(i)).run->output->h;
      else if (plotvar == "theta")
        tempplotvar = runlist->value(selectedruns->value(i)).run->output->theta;
      else if (plotvar == "dtheta")
        tempplotvar = runlist->value(selectedruns->value(i)).run->output->dtheta;
      else if (plotvar == "wtheta")
        tempplotvar = runlist->value(selectedruns->value(i)).run->output->wtheta;
      else if (plotvar == "q")
        tempplotvar = runlist->value(selectedruns->value(i)).run->output->q;
      else if (plotvar == "dq")
        tempplotvar = runlist->value(selectedruns->value(i)).run->output->dq;
      else if (plotvar == "wq")
        tempplotvar = runlist->value(selectedruns->value(i)).run->output->wq;
      int tsteps = int(runlist->value(selectedruns->value(i)).run->input.runtime / runlist->value(selectedruns->value(i)).run->input.dt) + 1;

      double yscale = plotheight / (graphmaxy-graphminy);   // scaling factor for f(real-coordinate to Widget-coordinate)
      double xscale = plotwidth  / (graphmaxx-graphminx);   // scaling factor for f(real-coordinate to Widget-coordinate)

      paint.setPen( colors.value(i) );
      for(int m=0; m<tsteps-1; m=m+5)
      {
        paint.drawLine(transfx((runlist->value(selectedruns->value(i)).run->output->t[m]),xscale,graphminx),
                       transfy((tempplotvar[m]),yscale,graphminy),
                       transfx((runlist->value(selectedruns->value(i)).run->output->t[m+5]),xscale,graphminx),
                       transfy((tempplotvar[m+5]),yscale,graphminy));
      }
    }
  }
}
