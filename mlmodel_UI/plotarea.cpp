#include "plotarea.h"

plotarea::plotarea(QMap<int, modelrun> *runs, QList<int> *selected, QWidget *parent) : QWidget(parent)
{
  selectedruns = selected;       // Needed?
  runlist = runs;
  plotvar = "h";
  setBackgroundRole(QPalette::Base);
  setAutoFillBackground(true);
}

double plotarea::transfx(double xreal, double xscale, double xmin)
{
  double xwidget = ((xreal-xmin)*xscale) + plotmargin;
  return (xwidget);
}

double plotarea::transfy(double yreal, double yscale, double ymin)
{
  int pwidget_height = geometry().height();
  double ywidget = pwidget_height -plotmargin - ((yreal-ymin)*yscale);
  return (ywidget);
}

void plotarea::paintEvent(QPaintEvent * /* event */)
{
  if (selectedruns->count() > 0)
  {
    double xmin = 1e5, xmax = -1e3, ymin = 1e5, ymax = 0;
    for(int i=0; i<selectedruns->count(); i++)
    {
      if (runlist->value(selectedruns->value(i)).hasrun)
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
    }

    xmin = std::floor(xmin);
    xmax = std::ceil(xmax);
    ymin = std::floor(ymin);
    ymax = std::ceil(ymax);

    std::cout << "xmin= " << xmin << " xmax= " << xmax << std::endl;
    std::cout << "ymin= " << ymin << " ymax= " << ymax << std::endl;

    // Size of widget (pixels)
    int plotwidget_width = geometry().width();
    int plotwidget_height = geometry().height();
    // Size of plotable area within widget (pixels)
    double plotwidth = plotwidget_width - 2 * plotmargin;
    double plotheight = plotwidget_height - 2 * plotmargin;

    double yscale = plotheight / (ymax-ymin);  // scaling factor for f(real-coordinate to Widget-coordinate)
    double xscale = plotwidth / (xmax-xmin);   // scaling factor for f(real-coordinate to Widget-coordinate)

    QPainter paint(this);
    QPen pen(Qt::black, 1, Qt::SolidLine);
    paint.setPen(pen);

    // Draw X and Y axis
    paint.setPen(Qt::SolidLine);
    paint.drawLine(plotmargin,plotwidget_height - plotmargin,plotwidget_width-plotmargin,plotwidget_height-plotmargin);
    paint.drawLine(plotmargin,plotwidget_height - plotmargin,plotmargin,plotmargin);

    // Draw labels Y-axis
    paint.drawText(5,plotmargin-5,40,13,Qt::AlignRight, QString::number(ymax));
    paint.drawText(5,(plotheight*0.25)+plotmargin-5,40,13,Qt::AlignRight, QString::number(ymax-((ymax-ymin)*0.25)));
    paint.drawText(5,(plotheight*0.5)+plotmargin-5,40,13,Qt::AlignRight, QString::number(ymax-((ymax-ymin)*0.50)));
    paint.drawText(5,(plotheight*0.75)+plotmargin-5,40,13,Qt::AlignRight, QString::number(ymax-((ymax-ymin)*0.75)));
    paint.drawText(5,plotwidget_height-60,40,13,Qt::AlignRight, QString::number(ymin));

    // Draw labels X-axis
    paint.drawText(30,plotwidget_height-45,40,13,Qt::AlignCenter, QString::number(xmin));
    paint.drawText((plotwidth*0.75)+plotmargin-20,plotwidget_height-45,40,13,Qt::AlignCenter, QString::number((xmax-((xmax-xmin)*0.25))));
    paint.drawText((plotwidth*0.5)+plotmargin-20,plotwidget_height-45,40,13,Qt::AlignCenter, QString::number((xmax-((xmax-xmin)*0.5))));
    paint.drawText((plotwidth*0.25)+plotmargin-20,plotwidget_height-45,40,13,Qt::AlignCenter, QString::number((xmax-((xmax-xmin)*0.75))));
    paint.drawText(plotwidget_width-70,plotwidget_height-45,40,13,Qt::AlignCenter, QString::number(xmax));

    // Draw title
    paint.drawText((plotwidget_width/2)-100,(plotmargin/2)-10,200,13,Qt::AlignCenter, "Something..");

    // Hereafter; clip data plot .
    paint.setClipping(true);
    paint.setClipRect(plotmargin,plotmargin,plotwidth,plotheight);

      QList<QColor> colors;
    colors << QColor(Qt::red) << QColor(Qt::blue) << QColor(Qt::darkGreen) << QColor(Qt::black) << QColor(Qt::magenta) << QColor(Qt::darkRed);

    for(int i=0; i<selectedruns->count(); i++)
    {
      if (runlist->value(selectedruns->value(i)).hasrun)
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

        paint.setPen( colors.value(i) );
        for(int m=0; m<tsteps-1; m++)
         {
           paint.drawLine(transfx((runlist->value(selectedruns->value(i)).run->output->t[m]),xscale,xmin),
                          transfy((tempplotvar[m]),yscale,ymin),
                          transfx((runlist->value(selectedruns->value(i)).run->output->t[m+1]),xscale,xmin),
                          transfy((tempplotvar[m+1]),yscale,ymin));
         }
      }
    }
  }
}
