#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "plotwindow.h"

#include <string>    // Needed for basename modelrun
#include <sstream>   // Needed for basename modelrun
#include <iostream>
#include <QString>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent), ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  connect(ui->startButton,    SIGNAL(clicked()),                this, SLOT(startrun()));
  connect(ui->cancelButton,   SIGNAL(clicked()),                this, SLOT(canceledit()));

  connect(ui->switch_wind,    SIGNAL(stateChanged(int)),        this, SLOT(wind_switch(int)));
  connect(ui->newRunButton,   SIGNAL(clicked()),                this, SLOT(newrun()));
  connect(ui->cloneRunButton, SIGNAL(clicked()),                this, SLOT(clonerun()));
  connect(ui->modelRunTree,   SIGNAL(itemSelectionChanged()),   this, SLOT(runTreeChanged()));
  connect(ui->deleteButton,   SIGNAL(clicked()),                this, SLOT(deleteRun()));
  connect(ui->graphButton,    SIGNAL(clicked()),                this, SLOT(startGraph()));
  connect(ui->input_name,     SIGNAL(editingFinished()),        this, SLOT(updateRunName()));
  connect(ui->exportButton,   SIGNAL(clicked()),                this, SLOT(exportRuns()));

  // ====== Couple of SIGNAL / SLOTS; update input data when form is changed ===============
  /*
  connect(ui->input_advq,         SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_adv_theta,    SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_adv_u,        SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_adv_v,        SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_beta,         SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_dq0,          SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_d_theta0,     SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_d_u0,         SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_d_v0,         SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_fc,           SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_gamma_q,      SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_gamma_theta,  SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_gamma_u,      SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_gamma_v,      SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_h0,           SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_name,         SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_ps,           SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_q0,           SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_theta0,       SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_time,         SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_timestep,     SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_u0,           SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_ustar,        SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_v0,           SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_wq,           SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_ws,           SIGNAL(editingFinished()), this, SLOT(updateInputdata()));
  connect(ui->input_wtheta,       SIGNAL(editingFinished()), this, SLOT(updateInputdata()));

  connect(ui->switch_wind,        SIGNAL(clicked()),         this, SLOT(updateInputdata()));
  connect(ui->input_name,         SIGNAL(editingFinished()), this, SLOT(updateInputdata()));

  // =======================================================================================
  */

  ui->wind_U_group->setDisabled(true);
  ui->wind_V_group->setDisabled(true);

  // Setup QTreeWidget
  QStringList heading;
  heading << "ID" << "Name";
  ui->modelRunTree->setColumnCount(2);
  ui->modelRunTree->setHeaderLabels(heading);
  ui->modelRunTree->setColumnWidth(0,35);
  ui->modelRunTree->hideColumn(0);
  ui->modelRunTree->setSelectionMode(QAbstractItemView::ExtendedSelection);

  modelrunlist = new QMap<int, modelrun>;
  newrun();
  ui->modelRunTree->setCurrentItem(ui->modelRunTree->topLevelItem(0));
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
  updateInputdata();
  modelrun run;
  run.hasrun = false;

  QMap<int, modelrun>::iterator i = modelrunlist->begin();
  int max=0;
  while (i != modelrunlist->end())
  {
    if (i.key() > max)
      max = i.key();
    ++i;
  }

  QString base = "run";
  QString num;
  num.setNum(max+1);
  base.append(num);
  run.runname.append(base);
  modelrunlist->insert((max+1),run);

  readdefaultinput();
  modelrunlist->value(max+1).run->input       = defaultinput;
  modelrunlist->find(max+1).value().previnput = defaultinput;

  QTreeWidgetItem *point = new QTreeWidgetItem(ui->modelRunTree);
  point->setText(0, QString::number(max+1));
  point->setText(1, base);
  ui->modelRunTree->setCurrentItem(point);

  updateForm();
}

void MainWindow::clonerun()
{
  updateInputdata();
  modelrun run;

  run.hasrun = false;

  QMap<int, modelrun>::iterator i = modelrunlist->begin();
  int max=0;
  while (i != modelrunlist->end())
  {
    if (i.key() > max)
      max = i.key();
    ++i;
  }

  int id = ui->modelRunTree->currentItem()->text(0).toInt();
  QString base = modelrunlist->value(id).runname;
  QString append = " (clone)";
  base.append(append);
  run.runname.append(base);
  modelrunlist->insert((max+1),run);

  modelrunlist->value(max+1).run->input = modelrunlist->value(id).run->input;
  modelrunlist->find(max+1).value().previnput = modelrunlist->value(id).previnput;

  QTreeWidgetItem *point = new QTreeWidgetItem(ui->modelRunTree);
  point->setText(0, QString::number(max+1));
  point->setText(1, base);

  updateForm();
}

void MainWindow::runTreeChanged()
{
  bool inputfields, deleteitems;

  if (ui->modelRunTree->selectedItems().size() == 0)
  {
    inputfields = false;
    deleteitems = false;
  }
  else if(ui->modelRunTree->selectedItems().size() == 1)
  {
    inputfields = true;
    deleteitems = true;
  }
  else
  {
    inputfields = false;
    deleteitems = true;
  }

  ui->tabWidget->setEnabled(inputfields);
  ui->cloneRunButton->setEnabled(inputfields);
  ui->deleteButton->setEnabled(deleteitems);
  ui->startButton->setEnabled(inputfields);
  ui->cancelButton->setEnabled(inputfields);
  updateForm();
}

void MainWindow::updateInputdata()
{
  formvalues.dt         = ui->input_timestep->text().toDouble();      // time step [s]
  formvalues.runtime    = ui->input_time->text().toDouble() * 3600;   // total run time [s]

  formvalues.h          = ui->input_h0->text().toDouble();            // initial ABL height [m]
  formvalues.Ps         = ui->input_ps->text().toDouble() * 100;      // surface pressure [Pa]
  formvalues.ws         = ui->input_ws->text().toDouble();            // large scale vertical velocity [m s-1]
  formvalues.fc         = ui->input_fc->text().toDouble();            // Coriolis parameter [m s-1]

  formvalues.theta      = ui->input_theta0->text().toDouble();        // initial mixed-layer potential temperature [K]
  formvalues.dtheta     = ui->input_d_theta0->text().toDouble();      // initial temperature jump at h [K]
  formvalues.gammatheta = ui->input_gamma_theta->text().toDouble();   // free atmosphere potential temperature lapse rate [K m-1]
  formvalues.advtheta   = ui->input_adv_theta->text().toDouble();     // advection of heat [K s-1]
  formvalues.beta       = ui->input_beta->text().toDouble();          // entrainment ratio for virtual heat [-]
  formvalues.wtheta     = ui->input_wtheta->text().toDouble();        // surface kinematic heat flux [K m s-1]

  formvalues.q          = ui->input_q0->text().toDouble() / 1000;     // initial mixed-layer specific humidity [kg kg-1]
  formvalues.dq         = ui->input_dq0->text().toDouble() / 1000;    // initial specific humidity jump at h [kg kg-1]
  formvalues.gammaq     = ui->input_gamma_q->text().toDouble() / 1000;// free atmosphere specific humidity lapse rate [kg kg-1 m-1]
  formvalues.advq       = ui->input_advq->text().toDouble() / 1000;   // advection of moisture [kg kg-1 s-1]
  formvalues.wq         = ui->input_wq->text().toDouble() / 1000;     // surface kinematic moisture flux [kg kg-1 m s-1]

  formvalues.sw_wind    = ui->switch_wind->checkState();              // prognostic wind switch
  formvalues.u          = ui->input_u0->text().toDouble();            // initial mixed-layer u-wind speed [m s-1]
  formvalues.du         = ui->input_d_u0->text().toDouble();          // initial u-wind jump at h [m s-1]
  formvalues.gammau     = ui->input_gamma_u->text().toDouble();       // free atmosphere u-wind speed lapse rate [s-1]
  formvalues.advu       = ui->input_adv_u->text().toDouble();         // advection of u-wind [m s-2]

  formvalues.v          = ui->input_v0->text().toDouble();            // initial mixed-layer u-wind speed [m s-1]
  formvalues.dv         = ui->input_d_v0->text().toDouble();          // initial u-wind jump at h [m s-1]
  formvalues.gammav     = ui->input_gamma_v->text().toDouble();       // free atmosphere v-wind speed lapse rate [s-1]
  formvalues.advv       = ui->input_adv_v->text().toDouble();         // advection of v-wind [m s-2]

  formvalues.ustar      = ui->input_ustar->text().toDouble();         // surface friction velocity [m s-1]

  QString name          = QString::fromStdString(ui->input_name->text().toStdString());

  if (ui->modelRunTree->selectedItems().size() == 1)                  // Extra check if QTreeWidget has selected item
  {
    int id = ui->modelRunTree->currentItem()->text(0).toInt();
    modelrunlist->value(id).run->input = formvalues;
    modelrunlist->find(id).value().runname = name;
    updateForm();
  }
}

void MainWindow::updateForm()
{
  if (ui->modelRunTree->selectedItems().size() == 1)                  // Extra check if QTreeWidget has selected item
  {
    int n = ui->modelRunTree->currentItem()->text(0).toInt();
    ui->input_timestep->setText(QString::number(modelrunlist->value(n).run->input.dt));
    ui->input_time->setText(QString::number(modelrunlist->value(n).run->input.runtime / 3600.));

    ui->input_h0->setText(QString::number(modelrunlist->value(n).run->input.h));
    ui->input_ps->setText(QString::number(modelrunlist->value(n).run->input.Ps / 100.));
    ui->input_ws->setText(QString::number(modelrunlist->value(n).run->input.ws));
    ui->input_fc->setText(QString::number(modelrunlist->value(n).run->input.fc));

    ui->input_theta0->setText(QString::number(modelrunlist->value(n).run->input.theta));
    ui->input_d_theta0->setText(QString::number(modelrunlist->value(n).run->input.dtheta));
    ui->input_gamma_theta->setText(QString::number(modelrunlist->value(n).run->input.gammatheta));
    ui->input_adv_theta->setText(QString::number(modelrunlist->value(n).run->input.advtheta));
    ui->input_beta->setText(QString::number(modelrunlist->value(n).run->input.beta));
    ui->input_wtheta->setText(QString::number(modelrunlist->value(n).run->input.wtheta));

    ui->input_q0->setText(QString::number(modelrunlist->value(n).run->input.q * 1000.));
    ui->input_dq0->setText(QString::number(modelrunlist->value(n).run->input.dq * 1000.));
    ui->input_gamma_q->setText(QString::number(modelrunlist->value(n).run->input.gammaq * 1000.));
    ui->input_advq->setText(QString::number(modelrunlist->value(n).run->input.advq * 1000.));
    ui->input_wq->setText(QString::number(modelrunlist->value(n).run->input.wq * 1000.));

    if (modelrunlist->value(n).run->input.sw_wind == true)
      ui->switch_wind->setChecked(true);
    else
      ui->switch_wind->setChecked(false);

    ui->input_u0->setText(QString::number(modelrunlist->value(n).run->input.u));
    ui->input_d_u0->setText(QString::number(modelrunlist->value(n).run->input.du));
    ui->input_gamma_u->setText(QString::number(modelrunlist->value(n).run->input.gammau));
    ui->input_adv_u->setText(QString::number(modelrunlist->value(n).run->input.advu));

    ui->input_v0->setText(QString::number(modelrunlist->value(n).run->input.v));
    ui->input_d_v0->setText(QString::number(modelrunlist->value(n).run->input.dv));
    ui->input_gamma_v->setText(QString::number(modelrunlist->value(n).run->input.gammav));
    ui->input_adv_v->setText(QString::number(modelrunlist->value(n).run->input.advv));

    ui->input_ustar->setText(QString::number(modelrunlist->value(n).run->input.ustar));

    ui->input_name->setText(modelrunlist->value(n).runname);
  }
}

void MainWindow::deleteRun()
{
  if(ui->modelRunTree->selectedItems().count() > 0)
  {
    for (int i=0; i<ui->modelRunTree->selectedItems().count(); i++)
    {
      QString ident = ui->modelRunTree->selectedItems()[i]->text(0);
      int n = ident.toInt(0,10);
      modelrunlist->remove(n);
      emit rundeleted(n);
    }
    qDeleteAll(ui->modelRunTree->selectedItems());
    ui->modelRunTree->setCurrentItem(ui->modelRunTree->topLevelItem(0));
  }
}

void MainWindow::updateRunName()
{
  if(ui->modelRunTree->selectedItems().count() > 0)
    ui->modelRunTree->currentItem()->setText(1,ui->input_name->text());
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

void MainWindow::startrun()
{
  updateInputdata();
  int id = ui->modelRunTree->currentItem()->text(0).toInt();
  modelrunlist->find(id).value().previnput = modelrunlist->find(id).value().run->input;
  modelrunlist->find(id).value().run->runmodel();
  modelrunlist->find(id).value().hasrun = true;
  emit runadded(id);
}

void MainWindow::canceledit()
{
  int id = ui->modelRunTree->currentItem()->text(0).toInt();
  modelrunlist->find(id).value().run->input = modelrunlist->find(id).value().previnput;
  updateForm();
}

void MainWindow::startGraph()
{
  showGraph(modelrunlist);
}

void MainWindow::showGraph(QMap<int, modelrun> *main)
{
  graph = new plotwindow(main, this);
  graph->setWindowFlags(Qt::Window);
  graph->show();
  connect(this, SIGNAL(rundeleted(int)), graph, SLOT(deleterun(int)));
  connect(this, SIGNAL(runadded(int)), graph, SLOT(addrun(int)));
}

void MainWindow::exportRuns()
{
  if(ui->modelRunTree->selectedItems().count() > 0)
  {
    QString dirname = QFileDialog::getExistingDirectory(this, "Select directory for saving runs", "~");
    for (int i=0; i<ui->modelRunTree->selectedItems().count(); i++)
    {
      QString runname  = ui->modelRunTree->selectedItems()[i]->text(1);
      QString filename = dirname + "/" + runname + ".csv";

      std::cout << filename.toStdString() << std::endl;

      //int n = ident.toInt(0,10);
      //modelrunlist->remove(n);
    }
  }
}
