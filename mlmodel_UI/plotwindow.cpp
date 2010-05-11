#include "plotwindow.h"
#include "subplot.h"
#include "ui_subplot.h"
#include "ui_plotwindow.h"
//#include "modeloutput.h"
#include <iostream>

plotwindow::plotwindow(QMap<int, modelrun> *runs, QList<int> *initialselected, QMainWindow *parent) : QMainWindow(parent), ui(new Ui::plotwindow)
{
  ui->setupUi(this);
  selectedruns = new QList<int>;
  runlist = runs;

  // Place left dockwidget in corner
  this->setCorner(Qt::TopLeftCorner,Qt::LeftDockWidgetArea);
  ui->AdvancedDock->setShown(false);

  // Create plotarea to draw in
  plotar = new plotarea(runlist,selectedruns,this);

  // Default plotinterval from subplot.cpp
  ui->plotintervalInput->setText(QString::number(plotar->plotinterval));

  // Signal/slots -------------------------------------------------------------------------------------
  connect(plotar, SIGNAL(axischanged()), this, SLOT(changeaxis()));
  connect(ui->autoscaleaxis, SIGNAL(clicked(bool)), this, SLOT(changeaxis()));
  connect(ui->autoscaleaxis, SIGNAL(clicked(bool)), plotar, SLOT(update()));
  connect(ui->menu_savepng, SIGNAL(triggered()), plotar, SLOT(saveImage()));
  connect(ui->xminInput, SIGNAL(editingFinished()), this, SLOT(changeaxis()));
  connect(ui->xmaxInput, SIGNAL(editingFinished()), this, SLOT(changeaxis()));
  connect(ui->yminInput, SIGNAL(editingFinished()), this, SLOT(changeaxis()));
  connect(ui->ymaxInput, SIGNAL(editingFinished()), this, SLOT(changeaxis()));
  connect(ui->modelruntree, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(updateselectedruns()));
  connect(ui->plotvar, SIGNAL(currentIndexChanged(int)), this, SLOT(changeplotvar()));
  connect(ui->plotintervalInput, SIGNAL(editingFinished()), this, SLOT(changeplotinterval()));
  // Menu interface:
  connect(ui->menu_basicplot, SIGNAL(triggered()), this, SLOT(switchtobasicplotting()));
  connect(ui->menu_advancedplot, SIGNAL(triggered()), this, SLOT(switchtoadvancedplotting()));

  // Set "auto scale axis" by default to true
  ui->autoscaleaxis->setChecked(true);

  // Layout and placement plotarea
  QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  sizePolicy.setHorizontalStretch(0);
  sizePolicy.setVerticalStretch(0);
  sizePolicy.setHeightForWidth(plotar->sizePolicy().hasHeightForWidth());
  plotar->setSizePolicy(sizePolicy);
  plotar->setMinimumSize(QSize(400, 400));
  ui->centralLayout->addWidget(plotar);

  // Setup QTreeWidget (List with modelruns / checkboxes)
  QStringList heading;
  heading << "ID" << "Name";
  ui->modelruntree->setColumnCount(2);
  ui->modelruntree->setHeaderLabels(heading);
  ui->modelruntree->setColumnWidth(0,35);
  ui->modelruntree->hideColumn(0);
  ui->modelruntree->setSelectionMode(QAbstractItemView::ExtendedSelection);

  // Iterate through QMap with modelruns; create entry in QtreeWidget when run.hasrun==true
  QMap<int, modelrun>::const_iterator i = runlist->constBegin();
  while (i != runlist->constEnd()) {
    if (runlist->value(i.key()).hasrun)
    {
      QTreeWidgetItem *point = new QTreeWidgetItem(ui->modelruntree);
      if (initialselected->contains(i.key()))
      {
        point->setCheckState(1,Qt::Checked);
        selectedruns->append(i.key());
      }
      else
        point->setCheckState(1,Qt::Unchecked);
      point->setDisabled(false);
      point->setText(0, QString::number(i.key()));
      point->setText(1, runlist->value(i.key()).runname);
    }
  ++i;
  }

  // Create dropdown menu with plotvariables for basic plotting
  QStringList varnames;
  modeloutput modelout(0);

  varnames << QString::fromStdString(modelout.h.description) << QString::fromStdString(modelout.theta.description) << QString::fromStdString(modelout.dtheta.description) << QString::fromStdString(modelout.wtheta.description)
          << QString::fromStdString(modelout.q.description) << QString::fromStdString(modelout.dq.description) << QString::fromStdString(modelout.wq.description);
  outputnames << "h" << "theta" << "dtheta" << "wtheta" << "q" << "dq" << "wq";
  ui->plotvar->addItems(varnames);


  // Create the advanced plot QTreeWidget

  ui->advancedplottree->setColumnCount(4);
  QTreeWidgetItem *time = new QTreeWidgetItem(ui->advancedplottree); time->setText(0, QString::fromStdString(modelout.t.name + "[" + modelout.t.unit + "]")); time->setText(1, tr("X")); time->setText(2, tr("Y")); time->setText(3, QString::fromStdString(modelout.t.description));


}

plotwindow::~plotwindow()
{
  delete ui;
}

void plotwindow::updateselectedruns()  // create QList containing ID's of selected runs
{
  int id = ui->modelruntree->currentItem()->text(0).toInt();
  if (ui->modelruntree->currentItem()->checkState(1) == 2)
  {
    if (!selectedruns->contains(id))
      selectedruns->append(id);
  }
  else if(ui->modelruntree->currentItem()->checkState(1) == 0)
    selectedruns->removeAt(selectedruns->indexOf(id));

  plotar->update();
}

void plotwindow::changeplotvar()
{
  plotar->plotvar = outputnames[ui->plotvar->currentIndex()];
  ui->autoscaleaxis->setChecked(true);
  plotar->autoaxis = true;
  plotar->update();
}

void plotwindow::deleterun(int num)
{
  QString id = QString::number(num,10);
  QTreeWidgetItem *del = ui->modelruntree->findItems(id,Qt::MatchExactly,0).value(0);
  delete del;
  selectedruns->removeAt(selectedruns->indexOf(num));
  plotar->update();
}

void plotwindow::addrun(int num)
{
  QString id = QString::number(num,10);
  if (ui->modelruntree->findItems(id,Qt::MatchExactly,0).count() == 0)
  {
    QTreeWidgetItem *point = new QTreeWidgetItem(ui->modelruntree);
    point->setCheckState(1,Qt::Unchecked);
    point->setDisabled(false);
    point->setText(0, QString::number(num));
    point->setText(1, runlist->value(num).runname);
  }
  plotar->update();
}

void plotwindow::changeaxis()
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

void plotwindow::changeplotinterval()
{
  plotar->plotinterval = ui->plotintervalInput->text().toInt();
  plotar->update();
}

void plotwindow::switchtobasicplotting()
{
  ui->AdvancedDock->setShown(false);
  ui->PlotvarDock->setShown(true);
}

void plotwindow::switchtoadvancedplotting()
{
  ui->AdvancedDock->setShown(true);
  ui->PlotvarDock->setShown(false);
}


