#include "plotwindow.h"
#include "subplot.h"
#include "ui_subplot.h"
#include "ui_plotwindow.h"
#include <iostream>
#include "QMessageBox"

plotwindow::plotwindow(QMap<int, modelrun> *runs, QList<int> *initialselected, QMainWindow *parent) : QMainWindow(parent=0), ui(new Ui::plotwindow)
{
  ui->setupUi(this);

  // Switch for Mac OS (X); changes:
  // 1) Detaches and positions the docks (within the spirit of Mac OS)
  // 2) Forces closing of docks
  // 3) Changes font-size plots (in subplot.cpp)
  MacOS = false;
  if(MacOS)
    this->setAttribute(Qt::WA_DeleteOnClose,true);

  selectedruns = new QList<int>;
  runlist = runs;

  this->setMouseTracking(true);

  // Place left dockwidget in corner
  this->setCorner(Qt::TopLeftCorner,Qt::LeftDockWidgetArea);
  ui->AdvancedDock->setShown(true);
  ui->PlotvarDock->setShown(false);

  // Create plotarea to draw in
  plotar = new plotarea(runlist,selectedruns,this);

  // Set MacOS switch in plotarea
  plotar->MacOS = MacOS;

  // Signal/slots -------------------------------------------------------------------------------------
  connect(plotar, SIGNAL(axischanged()), this, SLOT(changeaxis()));
  connect(plotar, SIGNAL(zoombymouse()), this, SLOT(zoomebymouse()));
  connect(plotar, SIGNAL(cursormoved()), this, SLOT(cursormoved()));
  connect(ui->autoscaleaxis, SIGNAL(clicked(bool)), this, SLOT(changeaxis()));
  connect(ui->autoscaleaxis, SIGNAL(clicked(bool)), plotar, SLOT(update()));
  connect(ui->sw_scatterplot, SIGNAL(clicked(bool)), this, SLOT(changeplottype()));
  connect(ui->sw_scatterplot, SIGNAL(clicked(bool)), plotar, SLOT(update()));

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
    if (runlist->find(i.key()).value().hasrun)
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
      point->setText(1, runlist->find(i.key()).value().runname);
    }
  ++i;
  }

  // Create dropdown menu with plotvariables for basic plotting
  QStringList varnames;
  modeloutput modelout(0,22);

  varnames << QString::fromStdString(modelout.h.description) << QString::fromStdString(modelout.theta.description) << QString::fromStdString(modelout.dtheta.description) << QString::fromStdString(modelout.wtheta.description)
          << QString::fromStdString(modelout.q.description) << QString::fromStdString(modelout.dq.description) << QString::fromStdString(modelout.wq.description);
  outputnames << "h" << "theta" << "dtheta" << "wtheta" << "q" << "dq" << "wq";

  //ui->plotvar->addItems(varnames);


  // Create the advanced plot QTreeWidget
  QStringList advancedtreeheader;
  advancedtreeheader << "Variables" << "X" << "Y" << "Description";
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
  QList<outputvar> scalarvars;
  QList<outputvar> surfacelayervars;
  QList<outputvar> radiationvars;
  QList<outputvar> surfacevars;
  QList<outputvar> cloudvars;
  QList<outputvar> vertprof;
  QList<outputvar> chemistry;

  QList<QString> advancedtreegroups;
  advancedtreegroups
      << "Mixed-layer"
      << "Temperature"
      << "Moisture"
      << "Wind"
      << "Scalars"
      << "Surface-layer"
      << "Radiation"
      << "Surface"
      << "Clouds"
      << "Vertical profiles"
      << "Chemistry";

  mixedlayervars
      << modelout.t
      << modelout.tutc
      << modelout.h
      << modelout.ws
      << modelout.we;

  temperaturevars
      << modelout.theta
      << modelout.thetav
      << modelout.dtheta
      << modelout.dthetav
      << modelout.wtheta
      << modelout.wthetae
      << modelout.wthetav
      //<< modelout.wthetaM
      << modelout.sigmatheta;

  moisturevars
      << modelout.q
      << modelout.dq
      << modelout.wq
      << modelout.wqe
      << modelout.wqM
      << modelout.sigmaq
      << modelout.lcl
      << modelout.RH
      << modelout.RHtop;

  windvars
      << modelout.u
      << modelout.du
      << modelout.uw
      << modelout.uwe
      << modelout.v
      << modelout.dv
      << modelout.vw
      << modelout.vwe
      << modelout.ustar;

  scalarvars
      << modelout.sca
      << modelout.dsca
      << modelout.wsca
      << modelout.wscae
      << modelout.wscaM
      << modelout.sigmasca
      << modelout.CO2
      << modelout.dCO2
      << modelout.wCO2
      << modelout.wCO2A
      << modelout.wCO2R
      << modelout.wCO2e
      << modelout.wCO2M
      << modelout.sigmaCO2;

  surfacelayervars
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

  cloudvars
      << modelout.ac
      << modelout.M;

  vertprof
      << modelout.thetaprof
      << modelout.wthetaprof
      << modelout.wthetavprof
      << modelout.qprof
      << modelout.wqprof
      << modelout.scaprof
      << modelout.wscaprof;

  chemistry
      << modelout.phi
      << modelout.k_r05;

  for(int n=0; n<22; n++)
    if(n < 14 || n > 17)
      chemistry << modelout.sc[n];

  ui->advancedplottree->setFocusPolicy(Qt::NoFocus);

  allvariables.insert(0,mixedlayervars);
  allvariables.insert(1,temperaturevars);
  allvariables.insert(2,moisturevars);
  allvariables.insert(3,windvars);
  allvariables.insert(4,scalarvars);
  allvariables.insert(5,surfacelayervars);
  allvariables.insert(6,radiationvars);
  allvariables.insert(7,surfacevars);
  allvariables.insert(8,cloudvars);
  allvariables.insert(9,vertprof);
  allvariables.insert(10,chemistry);

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

      if (advancedtreegroups.value(n) != "Vertical profiles")
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

  // BvS; Extra code to automatically detach and position docks for Mac OS (X)
  if(MacOS)
  {
    ui->AdvancedDock->setFloating(true);
    ui->PlotsettingsDock->setFloating(true);
    ui->ModelruntreeDock->setFloating(true);

    this->resize(400,400);

    ui->AdvancedDock->resize(350,425);
    ui->AdvancedDock->move(this->pos().x()+420,this->pos().y());

    ui->PlotsettingsDock->setMinimumWidth(200);
    ui->PlotsettingsDock->move(this->pos().x()-220,this->pos().y()+225);

    ui->ModelruntreeDock->setMinimumWidth(200);
    ui->ModelruntreeDock->resize(200,200);
    ui->ModelruntreeDock->move(this->pos().x()-220,this->pos().y());
  }
}

plotwindow::~plotwindow()
{
  delete ui;
}

void plotwindow::closeEvent(QCloseEvent *event)
{
  emit graphclosed(this);
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
  if(ui->modelruntree->topLevelItemCount() > 0)
  {
    int id = ui->modelruntree->currentItem()->text(0).toInt();
    if (ui->modelruntree->currentItem()->checkState(1) == 2)
    {
      if (!selectedruns->contains(id))
        selectedruns->append(id);
    }
    else if(ui->modelruntree->currentItem()->checkState(1) == 0)
      selectedruns->removeAt(selectedruns->indexOf(id));
  }

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
        {
          if (plotvarx == "thetaprof" || plotvarx == "qprof"  || plotvarx == "wthetaprof"  || plotvarx == "wthetavprof"  || plotvarx == "wqprof" || plotvarx == "scaprof" || plotvarx == "wscaprof")
          {
            setplotvar("h", &plotvary);
            ui->advancedplottree->findItems(plotvary, Qt::MatchRecursive, 4).value(0)->setCheckState(2, Qt::Checked);
          }
          ui->advancedplottree->findItems(plotvarx, Qt::MatchRecursive, 4).value(0)->setCheckState(column, Qt::Unchecked);
        }

      setplotvar(olditem->text(4), &plotvarx);

      if ((plotvarx == "thetaprof" || plotvarx == "qprof" || plotvarx == "wthetaprof" || plotvarx == "wthetavprof" || plotvarx == "wqprof" || plotvarx == "scaprof" || plotvarx == "wscaprof") && plotvary != "zprof")
      {
        ui->advancedplottree->findItems(plotvary, Qt::MatchRecursive, 4).value(0)->setCheckState(2, Qt::Unchecked);
        setplotvar("zprof", &plotvary);
      }

    }
    else if(column == 2)
    {
      if(ui->advancedplottree->findItems(plotvary, Qt::MatchRecursive, 4).count() > 0)
        if(ui->advancedplottree->findItems(plotvary, Qt::MatchRecursive, 4).value(0) != olditem)
          ui->advancedplottree->findItems(plotvary, Qt::MatchRecursive, 4).value(0)->setCheckState(column, Qt::Unchecked);

      if (plotvarx == "thetaprof" || plotvarx == "qprof" || plotvarx == "wthetaprof" || plotvarx == "wthetavprof" || plotvarx == "wqprof" || plotvarx == "scaprof" || plotvarx == "wscaprof")
      {
       setplotvar("t", &plotvarx);
       ui->advancedplottree->findItems(plotvarx, Qt::MatchRecursive, 4).value(0)->setCheckState(1, Qt::Checked);
       ui->advancedplottree->findItems("thetaprof", Qt::MatchRecursive, 4).value(0)->setCheckState(1, Qt::Unchecked);
       ui->advancedplottree->findItems("qprof", Qt::MatchRecursive, 4).value(0)->setCheckState(1, Qt::Unchecked);
       ui->advancedplottree->findItems("wthetaprof", Qt::MatchRecursive, 4).value(0)->setCheckState(1, Qt::Unchecked);
       ui->advancedplottree->findItems("wthetavprof", Qt::MatchRecursive, 4).value(0)->setCheckState(1, Qt::Unchecked);
       ui->advancedplottree->findItems("wqprof", Qt::MatchRecursive, 4).value(0)->setCheckState(1, Qt::Unchecked);
       ui->advancedplottree->findItems("scaprof", Qt::MatchRecursive, 4).value(0)->setCheckState(1, Qt::Unchecked);
       ui->advancedplottree->findItems("wscaprof", Qt::MatchRecursive, 4).value(0)->setCheckState(1, Qt::Unchecked);
      }
      setplotvar(olditem->text(4), &plotvary);
    }

    plotar->legendmoved = false;
    ui->autoscaleaxis->setCheckState(Qt::Checked);
    plotar->autoaxis = true;
    updateplotdata();
    plotar->update();
  }
//  else
//  {
//    if(ui->advancedplottree->findItems(plotvarx, Qt::MatchRecursive, 4).count() > 0)
//      if(ui->advancedplottree->findItems(plotvarx, Qt::MatchRecursive, 4).value(0) == olditem)
//        ui->advancedplottree->findItems(plotvarx, Qt::MatchRecursive, 4).value(0)->setCheckState(column, Qt::Checked);
//
//    if(ui->advancedplottree->findItems(plotvary, Qt::MatchRecursive, 4).count() > 0)
//      if(ui->advancedplottree->findItems(plotvary, Qt::MatchRecursive, 4).value(0) == olditem)
//        ui->advancedplottree->findItems(plotvary, Qt::MatchRecursive, 4).value(0)->setCheckState(column, Qt::Checked);
//  }
}

void plotwindow::updateplotdata()
{
  outputvar xdata, ydata;

  QMap<int, modelrun>::const_iterator i = runlist->constBegin();
  while (i != runlist->constEnd())
  {
    getdata(&xdata, runlist->find(i.key()).value(), plotvarx);
    getdata(&ydata, runlist->find(i.key()).value(), plotvary);

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
    point->setText(1, runlist->find(num).value().runname);
  }
  if (ui->modelruntree->findItems(id,Qt::MatchExactly,0).count() == 1)
    ui->modelruntree->findItems(id,Qt::MatchExactly,0).value(0)->setText(1,runlist->find(num).value().runname);

  updateplotdata();
  plotar->update();
}

void plotwindow::changeaxis()
{
  if (ui->autoscaleaxis->checkState() == Qt::Checked)
  {
    plotar->autoaxis = true;
    plotar->legendmoved = false;
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

void plotwindow::changeplottype()
{
  if (ui->sw_scatterplot->checkState() == Qt::Checked)
    plotar->scatterplot = true;
  else
    plotar->scatterplot = false;
}

void plotwindow::zoomebymouse()
{
  ui->autoscaleaxis->setCheckState(Qt::Unchecked);
  ui->xminInput->setDisabled(false);
  ui->xmaxInput->setDisabled(false);
  ui->yminInput->setDisabled(false);
  ui->ymaxInput->setDisabled(false);

  ui->xminInput->setText(QString::number(plotar->graphminx));
  ui->xmaxInput->setText(QString::number(plotar->graphmaxx));
  ui->yminInput->setText(QString::number(plotar->graphminy));
  ui->ymaxInput->setText(QString::number(plotar->graphmaxy));
}

void plotwindow::mouseMoveEvent(QMouseEvent *e)
 {
   cursormoved();
 }

void plotwindow::cursormoved()
{
  if (plotar->selectedruns->count() > 0 && plotar->underMouse())
  {
    QString statusmessage =
        QString::fromUtf8(plotar->xdatalist.value(1).name.c_str()) +
        " = " +
        QString::number(plotar->x_current,'g',5) +
        " " +
        QString::fromUtf8(plotar->xdatalist.value(1).unit.c_str()) +
        ", " +
        QString::fromUtf8(plotar->ydatalist.value(1).name.c_str()) +
        " = " +
        QString::number(plotar->y_current,'g',5) +
        " " +
        QString::fromUtf8(plotar->ydatalist.value(1).unit.c_str());

    ui->statusbar->showMessage(statusmessage);
  }
  else
    ui->statusbar->showMessage(" ");
}




