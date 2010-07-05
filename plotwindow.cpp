#include "plotwindow.h"
#include "subplot.h"
#include "ui_subplot.h"
#include "ui_plotwindow.h"
#include <iostream>

#include "QMessageBox"

plotwindow::plotwindow(QMap<int, modelrun> *runs, QList<int> *initialselected, QMainWindow *parent) : QMainWindow(parent), ui(new Ui::plotwindow)
{
  ui->setupUi(this);
  selectedruns = new QList<int>;
  runlist = runs;

  // Place left dockwidget in corner
  this->setCorner(Qt::TopLeftCorner,Qt::LeftDockWidgetArea);
  ui->AdvancedDock->setShown(true);
  ui->PlotvarDock->setShown(false);

  // Disable some option for demo @ 20 May
  //ui->plotintervalInput->setEnabled(false);
  //ui->plottype->setEnabled(false);

  // Create plotarea to draw in
  plotar = new plotarea(runlist,selectedruns,this);

  // Default plotinterval from subplot.cpp
  //ui->plotintervalInput->setText(QString::number(plotar->plotinterval));

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
  //connect(ui->plotintervalInput, SIGNAL(editingFinished()), this, SLOT(changeplotinterval()));
  // Menu interface:
  connect(ui->view_basicmode, SIGNAL(triggered()), this, SLOT(switchtobasicplotting()));
  connect(ui->view_advancedmode, SIGNAL(triggered()), this, SLOT(switchtoadvancedplotting()));
  connect(ui->view_menu, SIGNAL(aboutToShow()), this, SLOT(viewmenutriggered()));
  connect(ui->view_basicplotsettings, SIGNAL(toggled(bool)), this, SLOT(togglebasicsettings(bool)));
  connect(ui->view_advancedplotsettings, SIGNAL(toggled(bool)), this, SLOT(toggleadvancedsettings(bool)));
  connect(ui->view_axissettings, SIGNAL(toggled(bool)), this, SLOT(toggleaxissettings(bool)));
  connect(ui->view_modelruns, SIGNAL(toggled(bool)), this, SLOT(togglemodelruns(bool)));
  connect(ui->advancedplottree, SIGNAL(itemClicked(QTreeWidgetItem*, int)), SLOT(selectadvanceddata(QTreeWidgetItem*, int)));

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

  //ui->plotvar->addItems(varnames);


  // Create the advanced plot QTreeWidget
  QStringList advancedtreeheader;
  advancedtreeheader << "Variabele" << "X" << "Y" << "Description";
  ui->advancedplottree->setHeaderLabels(advancedtreeheader);
  ui->advancedplottree->setColumnCount(5);
  ui->advancedplottree->setColumnWidth(0,150);
  ui->advancedplottree->setColumnWidth(1,30);
  ui->advancedplottree->setColumnWidth(2,30);
  ui->advancedplottree->hideColumn(4);

  QMap<int,QList<outputvar> > allvariables;       // QMap to store the different QLists..
  QList<outputvar> mixedlayervars;
  QList<outputvar> temperaturevars;
  QList<outputvar> moisturevars;
  QList<outputvar> windvars;
  QList<outputvar> surfacelayervars;
  QList<outputvar> radiationvars;
  QList<outputvar> surfacevars;

  QList<QString> advancedtreegroups;
  advancedtreegroups
      << "Mixed-layer"
      << "Temperature"
      << "Moisture"
      << "Wind"
      << "Surface-layer"
      << "Radiation"
      << "Surface";

  mixedlayervars
      << modelout.t
      << modelout.h;

  temperaturevars
      << modelout.theta
      << modelout.thetav
      << modelout.dtheta
      << modelout.dthetav
      << modelout.wtheta
      << modelout.wthetae
      << modelout.wthetav;

  moisturevars
      << modelout.q
      << modelout.dq
      << modelout.wq
      << modelout.wqe;

  windvars
      << modelout.u
      << modelout.du
      << modelout.uw
      << modelout.uwe
      << modelout.v
      << modelout.dv
      << modelout.vw
      << modelout.vwe;

  surfacelayervars
      << modelout.ustar
      << modelout.L
      << modelout.Rib
      << modelout.ra
      << modelout.Cm
      << modelout.Cs;

  radiationvars
      << modelout.Swin
      << modelout.Swout
      << modelout.Lwin
      << modelout.Lwout
      << modelout.Q;

  surfacevars
      << modelout.wg
      << modelout.Tsoil
      << modelout.Ts
      << modelout.Wl
      << modelout.rs
      << modelout.H
      << modelout.LE
      << modelout.G;

  allvariables.insert(0,mixedlayervars);
  allvariables.insert(1,temperaturevars);
  allvariables.insert(2,moisturevars);
  allvariables.insert(3,windvars);
  allvariables.insert(4,surfacelayervars);
  allvariables.insert(5,radiationvars);
  allvariables.insert(6,surfacevars);

  for (int n=0; n<advancedtreegroups.size(); n++)
  {
    QTreeWidgetItem *treegroup = new QTreeWidgetItem;
    treegroup->setText(0,advancedtreegroups.value(n));
    ui->advancedplottree->addTopLevelItem(treegroup);
    if (n == 0)
      ui->advancedplottree->expandItem(treegroup);

    for (int i=0; i<allvariables.value(n).size(); i++)
    {
      QTreeWidgetItem *treeitem = new QTreeWidgetItem;
      outputvar item = allvariables.value(n).value(i);
      treeitem->setCheckState(1,Qt::Unchecked);
      treeitem->setCheckState(2,Qt::Unchecked);
      QString variable = QString::fromUtf8(item.name.c_str()) + " [" + QString::fromUtf8(item.unit.c_str()) + "]";
      QString description = QString::fromUtf8(item.description.c_str());
      QString id = QString::fromUtf8(item.id.c_str());

      treeitem->setText(0, variable);
      treeitem->setText(3, description);
      treeitem->setText(4, id);
      treegroup->addChild(treeitem);
    }
  }

  ui->plotvar->addItems(varnames);
}

plotwindow::~plotwindow()
{
  delete ui;
}

// Add functionality to view-menu
// ++++++++++++++++++++++++++++++++++++++++++++++++++
void plotwindow::viewmenutriggered()
{
  ui->view_basicplotsettings->setChecked(ui->PlotvarDock->isVisible());
  ui->view_advancedplotsettings->setChecked(ui->AdvancedDock->isVisible());
  ui->view_axissettings->setChecked(ui->PlotsettingsDock->isVisible());
  ui->view_modelruns->setChecked(ui->ModelruntreeDock->isVisible());
}

void plotwindow::switchtobasicplotting()
{
  ui->AdvancedDock->setShown(false);
  ui->PlotvarDock->setShown(true);
    ui->PlotvarDock->setFloating(false);
  ui->PlotsettingsDock->setShown(true);
    ui->PlotsettingsDock->setFloating(false);
  ui->ModelruntreeDock->setShown(true);
    ui->ModelruntreeDock->setFloating(false);
}

void plotwindow::switchtoadvancedplotting()
{
  ui->AdvancedDock->setShown(true);
    ui->AdvancedDock->setFloating(false);
  ui->PlotvarDock->setShown(false);
  ui->PlotsettingsDock->setShown(true);
    ui->PlotsettingsDock->setFloating(false);
  ui->ModelruntreeDock->setShown(true);
    ui->ModelruntreeDock->setFloating(false);
}

void plotwindow::togglebasicsettings(bool checkstate)
{
  ui->PlotvarDock->setVisible(checkstate);
}

void plotwindow::toggleadvancedsettings(bool checkstate)
{
  ui->AdvancedDock->setVisible(checkstate);
}

void plotwindow::toggleaxissettings(bool checkstate)
{
  ui->PlotsettingsDock->setVisible(checkstate);
}

void plotwindow::togglemodelruns(bool checkstate)
{
  ui->ModelruntreeDock->setVisible(checkstate);
}
// ++++++++++++++++++++++++++++++++++++++++++++++++++


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



void plotwindow::selectadvanceddata(QTreeWidgetItem *olditem, int column)
{
  if(olditem->checkState(column) == Qt::Checked)
  {
    if(column == 1)
    {
      if(ui->advancedplottree->findItems(plotvarx, Qt::MatchRecursive, 4).count() > 0)
        if(ui->advancedplottree->findItems(plotvarx, Qt::MatchRecursive, 4).value(0) != olditem)
          ui->advancedplottree->findItems(plotvarx, Qt::MatchRecursive, 4).value(0)->setCheckState(column, Qt::Unchecked);

      setplotvar(olditem->text(4), &plotvarx);
    }
    else if(column == 2)
    {
      if(ui->advancedplottree->findItems(plotvary, Qt::MatchRecursive, 4).count() > 0)
        if(ui->advancedplottree->findItems(plotvary, Qt::MatchRecursive, 4).value(0) != olditem)
          ui->advancedplottree->findItems(plotvary, Qt::MatchRecursive, 4).value(0)->setCheckState(column, Qt::Unchecked);

      setplotvar(olditem->text(4), &plotvary);
    }

    updateplotdata();
    plotar->update();
  }
  else
  {
    if(ui->advancedplottree->findItems(plotvarx, Qt::MatchRecursive, 4).count() > 0)
      if(ui->advancedplottree->findItems(plotvarx, Qt::MatchRecursive, 4).value(0) == olditem)
        ui->advancedplottree->findItems(plotvarx, Qt::MatchRecursive, 4).value(0)->setCheckState(column, Qt::Checked);

    if(ui->advancedplottree->findItems(plotvary, Qt::MatchRecursive, 4).count() > 0)
      if(ui->advancedplottree->findItems(plotvary, Qt::MatchRecursive, 4).value(0) == olditem)
        ui->advancedplottree->findItems(plotvary, Qt::MatchRecursive, 4).value(0)->setCheckState(column, Qt::Checked);
  }
}

void plotwindow::updateplotdata()
{
  outputvar xdata, ydata;

  QMap<int, modelrun>::const_iterator i = runlist->constBegin();
  while (i != runlist->constEnd())
  {
    getdata(&xdata, runlist->value(i.key()), plotvarx);
    getdata(&ydata, runlist->value(i.key()), plotvary);

    int key = i.key();
    plotar->xdatalist.insert(key, xdata);
    plotar->ydatalist.insert(key, ydata);
    ++i;
  }
}

void plotwindow::changeplotvar()
{
  // Uncheck items in advanced mode
  if(ui->advancedplottree->findItems(plotvarx, Qt::MatchRecursive, 4).count() > 0)
  {
    if(ui->advancedplottree->findItems(plotvarx, Qt::MatchRecursive, 4).count() > 0)
      ui->advancedplottree->findItems(plotvarx, Qt::MatchRecursive, 4).value(0)->setCheckState(1, Qt::Unchecked);
    if(ui->advancedplottree->findItems(plotvary, Qt::MatchRecursive, 4).count() > 0)
      ui->advancedplottree->findItems(plotvary, Qt::MatchRecursive, 4).value(0)->setCheckState(2, Qt::Unchecked);
  }

  plotar->lines = runlist->count();
  plotvarx = "t";
  plotvary = outputnames[ui->plotvar->currentIndex()];

  // Check items in advanced mode
  if(ui->advancedplottree->findItems(plotvarx, Qt::MatchRecursive, 4).count() > 0)
  {
    if(ui->advancedplottree->findItems(plotvarx, Qt::MatchRecursive, 4).count() > 0)
      ui->advancedplottree->findItems(plotvarx, Qt::MatchRecursive, 4).value(0)->setCheckState(1, Qt::Checked);
    if(ui->advancedplottree->findItems(plotvary, Qt::MatchRecursive, 4).count() > 0)
      ui->advancedplottree->findItems(plotvary, Qt::MatchRecursive, 4).value(0)->setCheckState(2, Qt::Checked);
  }


  updateplotdata();

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
  if (ui->modelruntree->findItems(id,Qt::MatchExactly,0).count() == 1)
    ui->modelruntree->findItems(id,Qt::MatchExactly,0).value(0)->setText(1,runlist->value(num).runname);

  updateplotdata();
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




