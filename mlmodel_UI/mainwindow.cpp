#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent), ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  connect(ui->switch_wind, SIGNAL(stateChanged(int)), this, SLOT(wind_switch(int)));
  connect(ui->newRunButton, SIGNAL(clicked()), this, SLOT(newrun()));

  ui->wind_U_group->setDisabled(true);
  ui->wind_V_group->setDisabled(true);

  readdefaultinput();

  modelrunlist = new QMap<int, modelrun>;

  modelrun startrun;
  startrun.runname = "basic";
  modelrunlist->insert(1,startrun);
  updateRunList();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::readdefaultinput()
{
  // temporary function to fill run form with default values

  // model test
  defaultinput.dt         = 60.;      // time step [s]
  defaultinput.runtime    = 36000.;   // total run time [s]

  // mixed-layer input
  defaultinput.h          = 200.;     // initial ABL height [m]
  defaultinput.Ps         = 101300.;  // surface pressure [Pa]
  defaultinput.ws         = 0.;       // large scale vertical velocity [m s-1]
  defaultinput.fc         = 1.e-4;    // Coriolis parameter [m s-1]

  defaultinput.theta      = 288.;     // initial mixed-layer potential temperature [K]
  defaultinput.dtheta     = 5.;       // initial temperature jump at h [K]
  defaultinput.gammatheta = 0.006;    // free atmosphere potential temperature lapse rate [K m-1]
  defaultinput.advtheta   = 0.;       // advection of heat [K s-1]
  defaultinput.beta       = 0.2;      // entrainment ratio for virtual heat [-]
  defaultinput.wtheta     = 0.1;      // surface kinematic heat flux [K m s-1]

  defaultinput.q          = 0.007;    // initial mixed-layer specific humidity [kg kg-1]
  defaultinput.dq         = -0.001;   // initial specific humidity jump at h [kg kg-1]
  defaultinput.gammaq     = 0.;       // free atmosphere specific humidity lapse rate [kg kg-1 m-1]
  defaultinput.advq       = 0.;       // advection of moisture [kg kg-1 s-1]
  defaultinput.wq         = 0.0001;   // surface kinematic moisture flux [kg kg-1 m s-1]

  defaultinput.sw_wind    = true;     // prognostic wind switch
  defaultinput.u          = 6.;       // initial mixed-layer u-wind speed [m s-1]
  defaultinput.du         = 4.;       // initial u-wind jump at h [m s-1]
  defaultinput.gammau     = 0.;       // free atmosphere u-wind speed lapse rate [s-1]
  defaultinput.advu       = 0.;       // advection of u-wind [m s-2]

  defaultinput.v          = -4.0;     // initial mixed-layer u-wind speed [m s-1]
  defaultinput.dv         = 4.0;      // initial u-wind jump at h [m s-1]
  defaultinput.gammav     = 0.;       // free atmosphere v-wind speed lapse rate [s-1]
  defaultinput.advv       = 0.;       // advection of v-wind [m s-2]

  defaultinput.ustar      = 0.3;      // surface friction velocity [m s-1]
}

void MainWindow::newrun()
{
  // 1. Get max(key)
  modelrun run;
  QMap<int, modelrun>::const_iterator i = modelrunlist->constBegin();
    int max=0;
    while (i != modelrunlist->constEnd()) {
       if (i.key() > max)
           max = i.key();
       ++i;
    }
  // 2. Insert in QMap
  modelrunlist->insert((max+1),run);
  // 3. Update list mainwindow
  updateRunList();
}

void MainWindow::setValues()
{
  // Read values from ..., set values in form
}

void MainWindow::updateRunList()
{
  // 1. Setup QMap
  QStringList heading;
  heading << "ID" << "Name";
  ui->modelRunsList->setColumnCount(2);
  ui->modelRunsList->setHeaderLabels(heading);
  ui->modelRunsList->setColumnWidth(0,35);
  //ui->modelRunsList->hideColumn(0);
  ui->modelRunsList->setSelectionMode(QAbstractItemView::ExtendedSelection);

  // 2. Clear QMap, repopulate QMap
  ui->modelRunsList->clear();
  QMap<int, modelrun>::const_iterator i = modelrunlist->constBegin();
    while (i != modelrunlist->constEnd()) {
        QTreeWidgetItem *point = new QTreeWidgetItem(ui->modelRunsList);
        point->setText(0, QString::number(i.key()));
        point->setText(1, modelrunlist->value(i.key()).runname);
        ++i;
  }
}

void MainWindow::wind_switch(int state)
{
  if (state == Qt::Checked) {
    ui->wind_U_group->setDisabled(false);
    ui->wind_V_group->setDisabled(false);
  } else {
    ui->wind_U_group->setDisabled(true);
    ui->wind_V_group->setDisabled(true);
  }
}
