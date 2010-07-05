#include <cmath>
#include "subplot.h"
#include "ui_subplot.h"

plotarea::plotarea(QMap<int, modelrun> *runs, QList<int> *selected, QWidget *parent) : QWidget(parent)
{
  selectedruns          = selected;     // QList with selected runs in plotwindow
  runlist               = runs;         // QMap with modelruns

  // Set background of plotarea to white
  setBackgroundRole(QPalette::Base);
  setAutoFillBackground(true);

  // Default margins for plot, can be changed by for instance the save-PNG function
  defaulttopmargin      = 30;
  defaultbottommargin   = 50;
  defaultleftmargin     = 70;
  defaultrightmargin    = 30;

  // Set autoaxis as default to false
  autoaxis              = false;

  // Line plot by default
  scatterplot           = false;

  // Support variable used when saving as PNG
  saveImageMode         = 0;

  // Default plot interval
  plotinterval          = 1;

  // Define 8 colors for plotting, set them unassigned.
  colors << QColor(Qt::blue) << QColor(Qt::darkGreen) << QColor(Qt::red) << QColor(Qt::cyan) << QColor(Qt::magenta) << QColor(Qt::yellow) << QColor(Qt::black) << QColor(Qt::gray);
  assignedcolors << -1 << -1 << -1 << -1 << -1 << -1 << -1 << -1;
}

double plotarea::transfx(double xreal, double xscale, double xmin)
{
  double xwidget = ((xreal-xmin)*xscale) + leftmargin;
  return (xwidget);
}

double plotarea::transfy(double yreal, double yscale, double ymin)
{
  double ywidget = plotwidget_height - bottommargin - ((yreal-ymin)*yscale);
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

void plotarea::saveImage()
{
  saveImageName = QFileDialog::getSaveFileName(this, "Save image as", "~", tr("Images (*.png)"));
  if (!saveImageName.endsWith(".png"))
      saveImageName = saveImageName+".png";

  saveImageMode = 1;
  update();
}

void plotarea::paintEvent(QPaintEvent * /* event */)
{
  if (selectedruns->count() > 0)
  {
    outputvar xdata, ydata;

    if (autoaxis)
    {
      xmin = 1e5;
      xmax = -1e3;
      ymin = 1e5;
      ymax = 0;

      for(int i=0; i<selectedruns->count(); i++)
      {
        //getdata(&xdata, &ydata, i);
        xdata = xdatalist.value(selectedruns->value(i));
        ydata = ydatalist.value(selectedruns->value(i));

        int tsteps = int(runlist->value(selectedruns->value(i)).run->input.runtime / runlist->value(selectedruns->value(i)).run->input.dt) + 1;

        for(int m=0; m<tsteps; m++)
        {
          if (xdata.data[m] > xmax)
            xmax = xdata.data[m];
          if (xdata.data[m] < xmin)
            xmin = xdata.data[m];

          if (ydata.data[m] > ymax)
            ymax = ydata.data[m];
          if (ydata.data[m] < ymin)
            ymin = ydata.data[m];
        }
        xmin_auto = xmin;
        xmax_auto = xmax;
      }
    }

    // Get data for axis-labels
    if (!autoaxis)
    {
      xdata = xdatalist.value(selectedruns->value(0));
      ydata = ydatalist.value(selectedruns->value(0));
    }

    // this value uses data that is assigned inside of a loop... here it goes ok, but it is risky generally...
    xlabel = QString::fromUtf8(xdata.name.c_str()) + " [" + QString::fromUtf8(xdata.unit.c_str()) + "]";
    ylabel = QString::fromUtf8(ydata.name.c_str()) + " [" + QString::fromUtf8(ydata.unit.c_str()) + "]";

    if (saveImageMode == 1)
    {
      plotwidget_width = 1000;
      plotwidget_height = 800;
      PNGscale = 2;
    }
    else
    {
      plotwidget_width = geometry().width();
      plotwidget_height = geometry().height();
      PNGscale = 1;
    }

    topmargin     = defaulttopmargin * PNGscale;
    bottommargin  = defaultbottommargin * PNGscale;
    leftmargin    = defaultleftmargin  * PNGscale;
    rightmargin   = defaultrightmargin * PNGscale;

    // Size of plotable area within widget (pixels)
    double plotwidth = plotwidget_width - leftmargin - rightmargin;
    double plotheight = plotwidget_height - topmargin - bottommargin;

    // ------------------------------------
    // Start drawing
    // ------------------------------------

    QPainter paint;
    QImage image(plotwidget_width, plotwidget_height , QImage::Format_ARGB32);

    if (saveImageMode == 1)   // Save as PNG
    {
      image.fill(QColor(Qt::white).rgb());
      paint.begin(&image);\
      QFont font("sans-serif", 26, QFont::Normal);
      paint.setFont(font);
    }

    else                      // Plot on screen
    {
      paint.begin(this);
      QFont font("sans-serif", 9, QFont::Normal);
      paint.setFont(font);
    }

    QPen pen(Qt::black, std::ceil(PNGscale), Qt::SolidLine);
    paint.setPen(pen);

    /*
    // Save as PDF
    QString fileName = "testpdf";
    QPrinter printer(QPrinter::HighResolution);
    printer.setResolution(100);
    printer.setFullPage(true);
    printer.setOutputFormat(QPrinter::PdfFormat);
    if (!fileName.endsWith(".pdf"))
      fileName = fileName+".pdf";
    printer.setOutputFileName(fileName);
    QPainter paint(&printer);
    */

    // Draw X and Y axis
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
    {
      paint.drawText((leftmargin-100),(plotheight * ((graphmaxy - y) / (graphmaxy - graphminy)))+topmargin-13,90,30,0x0082, QString::number(y,'f',nfrac));      // 0x0080 = AlignVCenter, 0x0002 = AlignRight
      paint.drawLine(leftmargin,(plotheight * ((graphmaxy - y) / (graphmaxy - graphminy)))+topmargin,leftmargin+(3 * PNGscale),(plotheight * ((graphmaxy - y) / (graphmaxy - graphminy)))+topmargin);
    }

    // Draw labels X-axis
    range     = nicenumber(xmax - xmin, false);
    d         = nicenumber(range / ((double)nticks - 1.), true);
    graphminx = std::floor(xmin / d) * d;
    graphmaxx = std::ceil(xmax / d) * d;
    nfrac     = std::max(-1 * (int)std::floor(std::log10(d)), 0);

    for(x = graphminx; x <= graphmaxx + .5 * d; x = x + d)
    {
      paint.drawText(((plotwidth * (x-graphminx))/(graphmaxx - graphminx))+leftmargin-30,plotwidget_height-bottommargin+8,60,30, 0x0024, QString::number(x,'f',nfrac));
      paint.drawLine(((plotwidth * (x-graphminx))/(graphmaxx - graphminx))+leftmargin,plotwidget_height-bottommargin,((plotwidth * (x-graphminx))/(graphmaxx - graphminx))+leftmargin,plotwidget_height-bottommargin-(3 * PNGscale));
    } 

    // Axis labels
    paint.drawText((plotwidth / 2) + leftmargin - 150,plotwidget_height - bottommargin + (28 * PNGscale),300,30,Qt::AlignHCenter, xlabel);
    paint.rotate(270);
    paint.drawText(-((plotheight / 2) + topmargin + 150),(5 * PNGscale),300,25,Qt::AlignCenter, ylabel);
    paint.rotate(90);

    // Hereafter; clip data plot .
    paint.setClipping(true);
    paint.setClipRect(leftmargin,topmargin,plotwidth,plotheight);

    int legendy = topmargin+5;

    for(int i=0; i<selectedruns->count(); i++)
    {
      //getdata(&xdata, &ydata, i);
      xdata = xdatalist.value(selectedruns->value(i));
      ydata = ydatalist.value(selectedruns->value(i));
      
      int tsteps = int(runlist->value(selectedruns->value(i)).run->input.runtime / runlist->value(selectedruns->value(i)).run->input.dt) + 1;

      double yscale = plotheight / (graphmaxy-graphminy);   // scaling factor for f(real-coordinate to Widget-coordinate)
      double xscale = plotwidth  / (graphmaxx-graphminx);   // scaling factor for f(real-coordinate to Widget-coordinate)

      // Find first free color in colorlist
      int j = selectedruns->value(i);

      if (assignedcolors.contains(j))
      {
        pen.setColor(colors.value(assignedcolors.indexOf(j)));
        paint.setPen(pen);
      }
      else
      {
        for (int n=0; n<colors.size(); n++)
        {
         if (assignedcolors.value(n) == -1)
         {
           assignedcolors.replace(n,j);
           pen.setColor(colors.value(n));
           paint.setPen(pen);
           break;
         }
        }
      }

      if (!scatterplot)
        plotinterval  = tsteps / 1000 + 1;
      else
        plotinterval  = (tsteps * (xmax - xmin)) / (50 * (xmax_auto - xmin_auto));

      int numpoints = (tsteps/plotinterval);

      QPointF points[numpoints];
      for (int m=0; m < numpoints; m++)
      {
        int n = m * plotinterval;
        points[m] = QPointF(transfx(xdata.data[n],xscale,graphminx), transfy(ydata.data[n],yscale,graphminy));
      }

      paint.setRenderHint(QPainter::Antialiasing, true);

      if (!scatterplot)
      {
        paint.drawPolyline(points, numpoints);
      }
      else
      {
        pen.setWidth(2);
        paint.setPen(pen);
        paint.drawPoints(points,numpoints);
      }

      paint.setRenderHint(QPainter::Antialiasing, false);

      paint.drawLine(leftmargin+(10*PNGscale),legendy+8,leftmargin+(25*PNGscale),legendy+8);
      paint.drawText(leftmargin+(30*PNGscale),legendy-7,400,30, 0x0081, runlist->value(selectedruns->value(i)).runname);
      legendy = legendy+(15 * PNGscale);
    }

    if (saveImageMode == 1)
    {
      image.save(saveImageName);
      paint.end();
      saveImageMode = 0;
      update();
    }

  }

  for(int i=0; i < assignedcolors.count(); i++)
    if(!selectedruns->contains(assignedcolors.value(i)))
      assignedcolors.replace(i,-1);

  if (autoaxis)
    emit axischanged();
}



