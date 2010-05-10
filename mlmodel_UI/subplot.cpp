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
  connect(ui->saveButton, SIGNAL(clicked()), plotar, SLOT(saveImage()));

  connect(ui->xminInput, SIGNAL(editingFinished()), this, SLOT(changeaxis()));
  connect(ui->xmaxInput, SIGNAL(editingFinished()), this, SLOT(changeaxis()));
  connect(ui->yminInput, SIGNAL(editingFinished()), this, SLOT(changeaxis()));
  connect(ui->ymaxInput, SIGNAL(editingFinished()), this, SLOT(changeaxis()));

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

  plotvar           = "h";
  defaulttopmargin  = 30;
  defaultbottommargin = 50;
  defaultleftmargin   = 70;
  defaultrightmargin  = 30;
  autoaxis          = false;
  saveImageMode     = 0;

  // Define 8 colors for plotting, set them unasigned.
  colors << QColor(Qt::blue) << QColor(Qt::darkGreen) << QColor(Qt::red) << QColor(Qt::cyan) << QColor(Qt::magenta) << QColor(Qt::yellow) << QColor(Qt::black) << QColor(Qt::gray);
  asignedcolors << -1 << -1 << -1 << -1 << -1 << -1 << -1 << -1;
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
    xlabel = runlist->value(selectedruns->value(0)).run->output->t.name + " [" + runlist->value(selectedruns->value(0)).run->output->t.unit + "]";

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
          ylabel = runlist->value(selectedruns->value(i)).run->output->h.name + " [" + runlist->value(selectedruns->value(i)).run->output->h.unit + "]";
        }
        else if (plotvar == "theta")
        {
          tempplotvar = runlist->value(selectedruns->value(i)).run->output->theta.data;
          ylabel = runlist->value(selectedruns->value(i)).run->output->theta.name + " [" + runlist->value(selectedruns->value(i)).run->output->theta.unit + "]";
        }
        else if (plotvar == "dtheta")
        {
          tempplotvar = runlist->value(selectedruns->value(i)).run->output->dtheta.data;
          ylabel = runlist->value(selectedruns->value(i)).run->output->dtheta.name + " [" + runlist->value(selectedruns->value(i)).run->output->dtheta.unit + "]";
        }
        else if (plotvar == "wtheta")
        {
          tempplotvar = runlist->value(selectedruns->value(i)).run->output->wtheta.data;
          ylabel = runlist->value(selectedruns->value(i)).run->output->wtheta.name + " [" + runlist->value(selectedruns->value(i)).run->output->wtheta.unit + "]";
        }
        else if (plotvar == "q")
        {
          tempplotvar = runlist->value(selectedruns->value(i)).run->output->q.data;
          ylabel = runlist->value(selectedruns->value(i)).run->output->q.name + " [" + runlist->value(selectedruns->value(i)).run->output->q.unit + "]";
        }
        else if (plotvar == "dq")
        {
          tempplotvar = runlist->value(selectedruns->value(i)).run->output->dq.data;
          ylabel = runlist->value(selectedruns->value(i)).run->output->dq.name + " [" + runlist->value(selectedruns->value(i)).run->output->dq.unit + "]";
        }
        else if (plotvar == "wq")
        {
          tempplotvar = runlist->value(selectedruns->value(i)).run->output->wq.data;
          ylabel = runlist->value(selectedruns->value(i)).run->output->wq.name + " [" + runlist->value(selectedruns->value(i)).run->output->wq.unit + "]";
        }

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
    paint.drawText((plotwidth / 2) + leftmargin - 150,plotwidget_height - bottommargin + (28 * PNGscale),300,30,Qt::AlignHCenter, QString::fromUtf8(xlabel.c_str()));
    paint.rotate(270);
    paint.drawText(-((plotheight / 2) + topmargin + 150),(5 * PNGscale),300,25,Qt::AlignCenter, QString::fromUtf8(ylabel.c_str()));
    paint.rotate(90);

    // Hereafter; clip data plot .
    paint.setClipping(true);
    paint.setClipRect(leftmargin+1,topmargin,plotwidth,plotheight);

    int legendy = topmargin+5;

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

      // Find first free color in colorlist
      if (asignedcolors.contains(i))
      {
        pen.setColor(colors.value(asignedcolors.indexOf(i)));
        paint.setPen(pen);
      }
      else
      {
        for (int n=0; n<colors.size(); n++)
        {
         if (asignedcolors.value(n) == -1)
         {
           asignedcolors.replace(n,i);
           pen.setColor(colors.value(n));
           paint.setPen(pen);
           break;
         }
        }
      }

      QPointF points[tsteps-1];
      for (int m=0; m<tsteps-1; m++)
      {
        points[m] = QPointF(transfx((runlist->value(selectedruns->value(i)).run->output->t.data[m]),xscale,graphminx), transfy((tempplotvar[m]),yscale,graphminy));
      }

      paint.setRenderHint(QPainter::Antialiasing, true);
      paint.drawPolyline(points, tsteps-1);
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
  if (autoaxis)
    emit axischanged();
}



