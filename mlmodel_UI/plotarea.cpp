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
    double xmin = 1e5, xmax = -1e3, ymin = 0, ymax = 0;
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



    QPainter paint(this);
    QPen pen(Qt::black, 1, Qt::SolidLine);
    paint.setPen(pen);
    paint.drawLine(0,0,1000,1000);
  }
}
