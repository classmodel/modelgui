/*
 * CLASS
 * Copyright (c) 2010-2013 Meteorology and Air Quality section, Wageningen University and Research centre
 * Copyright (c) 2011-2013 Jordi Vila-Guerau de Arellano
 * Copyright (c) 2011-2013 Chiel van Heerwaarden
 * Copyright (c) 2011-2013 Bart van Stratum
 * Copyright (c) 2011-2013 Kees van den Dries
 *
 * This file is part of CLASS
 *
 * CLASS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * CLASS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with CLASS.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cmath>
#include "subplot.h"
#include "ui_subplot.h"
#include <QTextDocument>
#include <QPointF>


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

  // Start mousetracking for this widget
  this->setMouseTracking(true);

  // Disable rubberband to start with (prevents crash..)
  drawrubberband        = false;
  mousereleased         = false;

  legendmoved           = false;
  legendmoves           = false;
}

double plotarea::transfx(double x, double xscale, double xmin, int mode)
{
  if (mode == 0) // Mode 0 = xreal to xwidget, mode 1 = xwidget to xreal
  {
    double xwidget = ((x-xmin)*xscale) + leftmargin;
    return (xwidget);
  }
  else
  {
    double xreal = ((x - leftmargin) / xscale) + xmin;
    return (xreal);
  }
}

double plotarea::transfy(double y, double yscale, double ymin, int mode)
{
  if (mode == 0) // Mode 0 = xreal to xwidget, mode 1 = xwidget to xreal
  {
    double ywidget = plotwidget_height - bottommargin  - ((y-ymin)*yscale);
    return (ywidget);
  }
  else
  {
    double yreal = ((-y + plotwidget_height - bottommargin) / yscale) + ymin;
    return (yreal);
  }
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

    if (mousereleased && (x_release != 0) && (y_release != 0))
    {
      autoaxis = false;
      if (x_press < x_release)
      {
        xmin = transfx(x_press,xscale,graphminx,1);
        xmax = transfx(x_release,xscale,graphminx,1);
      }
      else if(x_press > x_release)
      {
        xmax = transfx(x_press,xscale,graphminx,1);
        xmin = transfx(x_release,xscale,graphminx,1);
      }

      if (y_press > y_release)
      {
        ymin = transfy(y_press,yscale,graphminy,1);
        ymax = transfy(y_release,yscale,graphminy,1);
      }
      else if(y_press < y_release)
      {
        ymax = transfy(y_press,yscale,graphminy,1);
        ymin = transfy(y_release,yscale,graphminy,1);
      }

    }

    if (autoaxis)
    {
      xmin = 1e5;
      xmax = -1e3;
      ymin = 1e5;
      ymax = 0;

      for(int i=0; i<selectedruns->count(); i++)
      {
        xdata = xdatalist.value(selectedruns->value(i));
        ydata = ydatalist.value(selectedruns->value(i));

        double tempruntime = runlist->find(selectedruns->value(i)).value().run->input.runtime;
        double tempdt      = runlist->find(selectedruns->value(i)).value().run->input.dt;

        int tsteps = int(tempruntime / tempdt) + 1;

        for(int m=0; m<tsteps; m++)
        {
          int m2 = m*4;
          if (ydata.id == "zprof")
          {
            for(int n=0;n<4;n++)
            {
              if (n == 1 || n == 0)
              {
                if (xdata.data[m2+n] > xmax)
                  xmax = xdata.data[m2+n];
                if (xdata.data[m2+n] < xmin)
                  xmin = xdata.data[m2+n];
              }
              if (n == 2)
              {
                double value = xdata.data[m2+n] + (xdata.data[m2+n+1] - xdata.data[m2+n]) * (100. / 1.e6);
                if (value > xmax)
                  xmax = value;
                if (value < xmin)
                  xmin = value;
                if (ydata.data[m2+n] + 100. > ymax)
                  ymax = ydata.data[m2+n] + 100.;
              }
              ymin = 0.;
            }
          }
          else
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
        }

        if(ydata.id == "zprof")
        {
          xmin = xmin - 0.01 * (xmax - xmin);
          xmax = xmax + 0.01 * (xmax - xmin);
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
      plotwidget_width   = 1000;
      plotwidget_height  = 800;
      PNGscale           = 2;
      PNGfontscale       = 2.5;
   }
    else
    {
      plotwidget_width   = geometry().width();
      plotwidget_height  = geometry().height();
      PNGscale           = 1;
      PNGfontscale       = 1;
    }

    topmargin     = defaulttopmargin    * PNGscale;
    bottommargin  = defaultbottommargin * PNGscale;
    leftmargin    = defaultleftmargin   * PNGscale;
    rightmargin   = defaultrightmargin  * PNGscale;

    // Size of plotable area within widget (pixels)
    double plotwidth  = plotwidget_width - leftmargin - rightmargin;
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
      QFont font("sans-serif", 24, QFont::Normal);
      paint.setFont(font);
    }

    else                      // Plot on screen
    {
      paint.begin(this);
      QFont font;
      if(MacOS)
        font.setPointSize(12);
      else
        font.setPointSize(9);
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
    if(std::abs(ymax - ymin) < 1e-10)
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
      paint.drawText(((plotwidth * (x-graphminx))/(graphmaxx - graphminx))+leftmargin-30,plotwidget_height-bottommargin+8,60,(30*PNGfontscale), 0x0024, QString::number(x,'f',nfrac));
      paint.drawLine(((plotwidth * (x-graphminx))/(graphmaxx - graphminx))+leftmargin,plotwidget_height-bottommargin,((plotwidth * (x-graphminx))/(graphmaxx - graphminx))+leftmargin,plotwidget_height-bottommargin-(3 * PNGscale));
    }

    // Axis labels
    paint.drawText((plotwidth / 2) + leftmargin - 150,plotwidget_height - bottommargin + (28 * PNGscale),300,(30*PNGscale),Qt::AlignHCenter, xlabel);
    paint.rotate(270);
    paint.drawText(-((plotheight / 2) + topmargin + 150),(5 * PNGscale),300,(25*PNGscale),Qt::AlignCenter, ylabel);
    paint.rotate(90);

    // Hereafter; clip data plot .
    paint.setClipping(true);
    paint.setClipRect(leftmargin,topmargin,plotwidth,plotheight);

    legend_width = 0;
    legend_height = selectedruns->count() * 15;

    if (!legendmoved)
    {
      legend_y = defaulttopmargin + 5;
      legend_x = defaultleftmargin;
    }

    for(int i=0; i<selectedruns->count(); i++)
    {
      xdata = xdatalist.value(selectedruns->value(i));
      ydata = ydatalist.value(selectedruns->value(i));

      double tempruntime = runlist->find(selectedruns->value(i)).value().run->input.runtime;
      double tempdt      = runlist->find(selectedruns->value(i)).value().run->input.dt;

      int tsteps = int(tempruntime / tempdt) + 1;

      yscale = plotheight / (graphmaxy-graphminy);   // scaling factor for f(real-coordinate to Widget-coordinate)
      xscale = plotwidth  / (graphmaxx-graphminx);   // scaling factor for f(real-coordinate to Widget-coordinate)

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

      if (scatterplot)
        plotinterval  = (tsteps * (xmax - xmin)) / (50 * (xmax_auto - xmin_auto));
      else
      {
        if (ydata.id == "zprof")
          plotinterval = 1.;
        else
          plotinterval  = tsteps / 1000 + 1;
      }

      int numpoints = 0;

      if(plotinterval > 0)
        numpoints = (tsteps/plotinterval);

      QPointF *points = new QPointF[numpoints];
      //QPointF points[numpoints];
      for (int m=0; m < numpoints; m++)
      {
        int n = m * plotinterval;
        points[m] = QPointF(transfx(xdata.data[n],xscale,graphminx,0), transfy(ydata.data[n],yscale,graphminy,0));
      }

      paint.setRenderHint(QPainter::Antialiasing, true);

      if (ydata.id != "zprof")
      {
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
      }
      else
      {
        profinterval = 60;
        for (int m=0; m < 3; m++)
        {
          paint.drawLine(transfx(xdata.data[m],xscale,graphminx,0),transfy(ydata.data[m],yscale,graphminy,0),transfx(xdata.data[m+1],xscale,graphminx,0),transfy(ydata.data[m+1],yscale,graphminy,0));
        }
        for (int m=(4*profinterval); m < tsteps * 4; m = m+(profinterval*4))
        {
          paint.drawLine(transfx(xdata.data[m],xscale,graphminx,0),transfy(ydata.data[m],yscale,graphminy,0),transfx(xdata.data[m+1],xscale,graphminx,0),transfy(ydata.data[m+1],yscale,graphminy,0));
          paint.drawLine(transfx(xdata.data[m+1],xscale,graphminx,0),transfy(ydata.data[m+1],yscale,graphminy,0),transfx(xdata.data[m+2],xscale,graphminx,0),transfy(ydata.data[m+2],yscale,graphminy,0));
          paint.drawLine(transfx(xdata.data[m+2],xscale,graphminx,0),transfy(ydata.data[m+2],yscale,graphminy,0),transfx(xdata.data[m+3],xscale,graphminx,0),transfy(ydata.data[m+3],yscale,graphminy,0));
        }
      }

      delete[] points;

      // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      // Create and draw legend entry
      paint.setRenderHint(QPainter::Antialiasing, false);

      // Create legend label text, add time interval for vertical profiles
      QString legendlabel = runlist->find(selectedruns->value(i)).value().runname;
      if (ydata.id == "zprof")
        legendlabel = runlist->find(selectedruns->value(i)).value().runname + " [dt=" + QString::number((profinterval * runlist->find(selectedruns->value(i)).value().run->input.dt) / 3600.) + "h]";

      // Find the maximum length of a legend string, needed for the movable legend.
      if (legendlabel.length() > legend_width)
        legend_width = legendlabel.length();

      // Find relative position with xy-axis
      double leg_graph_x = double(legend_x - defaultleftmargin) / double(geometry().width() - defaultleftmargin - defaultrightmargin);
      double leg_graph_y = double(legend_y - defaulttopmargin) / double(geometry().height() - defaulttopmargin - defaultbottommargin);

      // Offset from top left corner of xy-axis
      int offset_x = leg_graph_x * (plotwidget_width - leftmargin - rightmargin);
      int offset_y = leg_graph_y * (plotwidget_height - topmargin - bottommargin);

      // Offset from top left corner of window
      int base_x = offset_x + leftmargin;
      int base_y = offset_y + topmargin;

      v_offset = i * 15 * PNGscale;
      line_xs  = base_x   + (10 * PNGscale);
      line_xe  = line_xs  + (15 * PNGscale);
      line_y   = base_y   + v_offset + 8;
      label_x  = base_x   + (30 * PNGscale);
      label_y  = base_y   + v_offset - 7;

      // Draw the legend
      paint.drawLine(line_xs,line_y,line_xe,line_y);
      paint.drawText(label_x,label_y,(legendlabel.length() * 10 * PNGfontscale),30, 0x0081, legendlabel);
      // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
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

  if (autoaxis && !legendmoved)
    emit axischanged();

  if (mousereleased)
    emit zoombymouse();

  if (mousepressed)
    mousepressed = false;

  if (mousereleased)
    mousereleased = false;
}

void plotarea::mousePressEvent( QMouseEvent *e )
{
  x_press = e->x();
  y_press = e->y();

  if (x_press > legend_x + 10 &&
      x_press < legend_x + legend_width * 7 + 30  &&
      y_press > legend_y &&
      y_press < legend_y + legend_height)
  {
    legendmoves = true;
    legendmoved = true;
    legend_x_offset = e->x() - legend_x;
    legend_y_offset = e->y() - legend_y;
  }
  else
  {
    origin_rubberband = e->pos();
    rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    drawrubberband = true;
    rubberBand->setGeometry(QRect(origin_rubberband, QSize()));
    rubberBand->show();
    mousepressed = true;
    update();
  }
}

void plotarea::mouseMoveEvent(QMouseEvent *e)
 {
  if (legendmoves)
   {
     legend_x = e->x() - legend_x_offset;
     legend_y = e->y() - legend_y_offset;
     update();
   }

   if (drawrubberband)
     rubberBand->setGeometry(QRect(origin_rubberband, e->pos()).normalized());

   x_current = transfx(e->x(),xscale,graphminx,1);
   y_current = transfy(e->y(),yscale,graphminy,1);

   emit cursormoved();
 }

void plotarea::mouseReleaseEvent( QMouseEvent *e )
{
  x_release = e->x();
  y_release = e->y();

  if (legendmoves)
  {
    legendmoves = false;
  }

  if (drawrubberband)
  {
    rubberBand->hide();
    drawrubberband = false;
    if (x_release != x_press)
      mousereleased = true;
    update();
  }
}



