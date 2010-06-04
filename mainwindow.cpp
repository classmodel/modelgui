#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "plotwindow.h"

#include <string>    // Needed for basename modelrun
#include <sstream>   // Needed for basename modelrun
#include <iostream>
#include <QString>
#include <QFileDialog>
#include <QFont>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent), ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  connect(ui->startButton,    SIGNAL(clicked()),                this, SLOT(startrun()));
  connect(ui->cancelButton,   SIGNAL(clicked()),                this, SLOT(canceledit()));

  connect(ui->newRunButton,   SIGNAL(clicked()),                this, SLOT(newrun()));
  connect(ui->cloneRunButton, SIGNAL(clicked()),                this, SLOT(clonerun()));
  connect(ui->modelRunTree,   SIGNAL(itemSelectionChanged()),   this, SLOT(runTreeChanged()));
  connect(ui->deleteButton,   SIGNAL(clicked()),                this, SLOT(deleteRun()));
  connect(ui->graphButton,    SIGNAL(clicked()),                this, SLOT(startGraph()));
  connect(ui->input_name,     SIGNAL(editingFinished()),        this, SLOT(updateRunName()));
  connect(ui->exportButton,   SIGNAL(clicked()),                this, SLOT(exportRuns()));

  // Switches
  connect(ui->sw_wind,        SIGNAL(stateChanged(int)),        this, SLOT(wind_switch(int)));

  loadfieldslots();

  // Setup QTreeWidget with model runs
    QStringList heading;
  heading << "ID" << "Name";
  ui->modelRunTree->setColumnCount(2);
  ui->modelRunTree->setHeaderLabels(heading);
  ui->modelRunTree->setColumnWidth(0,35);
  ui->modelRunTree->hideColumn(0);
  ui->modelRunTree->setSelectionMode(QAbstractItemView::ExtendedSelection);

  modelrunlist = new QMap<int, modelrun>;
  selectedruns = new QList<int>;

  newrun();
  ui->modelRunTree->setCurrentItem(ui->modelRunTree->topLevelItem(0));

  // if all fields are properly assigned, the next line can be removed
  formvalues            = defaultinput;
}

MainWindow::~MainWindow()
{
  delete ui;
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

  QFont font;
  font = point->font(1);
  font.setItalic(true);
  point->setFont(1,font);
  point->setTextColor(1,Qt::gray);

  ui->modelRunTree->setCurrentItem(point);

  updateForm();
}

void MainWindow::clonerun()
{
  updateInputdata();

  for (int n=0; n<ui->modelRunTree->selectedItems().count(); n++)
  {
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

    int id = ui->modelRunTree->selectedItems()[n]->text(0).toInt();
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

    QFont font;
    font = point->font(1);
    font.setItalic(true);
    point->setFont(1,font);
    point->setTextColor(1,Qt::gray);

    if(n == (ui->modelRunTree->selectedItems().count() - 1))
      ui->modelRunTree->setCurrentItem(point);
  }

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
  ui->cloneRunButton->setEnabled(deleteitems);
  ui->deleteButton->setEnabled(deleteitems);
  ui->startButton->setEnabled(deleteitems);
  ui->cancelButton->setEnabled(inputfields);
  ui->exportButton->setEnabled(deleteitems);
  updateSelectedRuns();
  updateForm();
}

void MainWindow::updateSelectedRuns()
{
  selectedruns->clear();
  for (int i = 0; i < ui->modelRunTree->topLevelItemCount(); i++)
  {
    QTreeWidgetItem *item =  ui->modelRunTree->topLevelItem (i);
      if (item->isSelected() && modelrunlist->value(item->text(0).toInt()).hasrun)
        selectedruns->append(item->text(0).toInt());
  }
}


void MainWindow::updateInputdata()
{
  formvalues.dt         = ui->input_timestep->text().toDouble();        // time step [s]
  formvalues.runtime    = ui->input_time->text().toDouble() * 3600;     // total run time [s]

  // MIXED-LAYER
  formvalues.h          = ui->input_ml_h->text().toDouble();             // initial ABL height [m]
  formvalues.Ps         = ui->input_ml_ps->text().toDouble() * 100;      // surface pressure [Pa]
  formvalues.ws         = ui->input_ml_ws->text().toDouble();            // large scale vertical velocity [m s-1]
  formvalues.beta       = ui->input_ml_beta->text().toDouble();             // entrainment ratio for virtual heat [-]

  // HEAT
  formvalues.theta      = ui->input_heat_theta->text().toDouble();       // initial mixed-layer potential temperature [K]
  formvalues.dtheta     = ui->input_heat_dtheta->text().toDouble();      // initial temperature jump at h [K]
  formvalues.gammatheta = ui->input_heat_gammatheta->text().toDouble();  // free atmosphere potential temperature lapse rate [K m-1]
  formvalues.advtheta   = ui->input_heat_advtheta->text().toDouble();    // advection of heat [K s-1]
  formvalues.wtheta     = ui->input_heat_wtheta->text().toDouble();        // surface kinematic heat flux [K m s-1]

  // MOISTURE
  formvalues.q          = ui->input_moisture_q->text().toDouble() / 1000;     // initial mixed-layer specific humidity [kg kg-1]
  formvalues.dq         = ui->input_moisture_dq->text().toDouble() / 1000;    // initial specific humidity jump at h [kg kg-1]
  formvalues.gammaq     = ui->input_moisture_gammaq->text().toDouble() / 1000;// free atmosphere specific humidity lapse rate [kg kg-1 m-1]
  formvalues.advq       = ui->input_moisture_advq->text().toDouble() / 1000;   // advection of moisture [kg kg-1 s-1]
  formvalues.wq         = ui->input_moisture_wq->text().toDouble() / 1000;     // surface kinematic moisture flux [kg kg-1 m s-1]

  // WIND
  //formvalues.sw_wind    = ui->switch_wind->checkState();              // prognostic wind switch
  formvalues.u          = ui->input_wind_u->text().toDouble();            // initial mixed-layer u-wind speed [m s-1]
  formvalues.du         = ui->input_wind_ug->text().toDouble()
                          - ui->input_wind_u->text().toDouble();          // initial u-wind jump at h [m s-1]
  formvalues.gammau     = ui->input_wind_gammau->text().toDouble();       // free atmosphere u-wind speed lapse rate [s-1]
  formvalues.advu       = ui->input_wind_advu->text().toDouble();         // advection of u-wind [m s-2]
  formvalues.v          = ui->input_wind_v->text().toDouble();            // initial mixed-layer v-wind speed [m s-1]
  formvalues.dv         = ui->input_wind_vg->text().toDouble()
                          - ui->input_wind_v->text().toDouble();          // initial u-wind jump at h [m s-1]
  formvalues.gammav     = ui->input_wind_gammav->text().toDouble();       // free atmosphere v-wind speed lapse rate [s-1]
  formvalues.advv       = ui->input_wind_advv->text().toDouble();         // advection of v-wind [m s-2]
  formvalues.ustar      = ui->input_wind_ustar->text().toDouble();         // surface friction velocity [m s-1]
  formvalues.fc         = ui->input_wind_fc->text().toDouble();            // Coriolis parameter [m s-1]

  formvalues.z0h        = ui->input_surfacelayer_z0h->text().toDouble();
  formvalues.z0m        = ui->input_surfacelayer_z0m->text().toDouble();

  // SOIL
  formvalues.T2         = ui->input_soil_T2->text().toDouble();
  formvalues.Tsoil      = ui->input_soil_Tsoil->text().toDouble();
  formvalues.w2         = ui->input_soil_W2->text().toDouble();
  formvalues.wg         = ui->input_soil_Wg->text().toDouble();
  formvalues.wsat       = ui->input_soil_wsat->text().toDouble();
  formvalues.wfc        = ui->input_soil_wfc->text().toDouble();
  formvalues.wwilt      = ui->input_soil_wwilt->text().toDouble();
  formvalues.T2         = ui->input_soil_T2->text().toDouble();
  formvalues.C1sat      = ui->input_soil_c1sat->text().toDouble();
  formvalues.C2ref      = ui->input_soil_c2ref->text().toDouble();
  formvalues.a          = ui->input_soil_a->text().toDouble();
  formvalues.b          = ui->input_soil_b->text().toDouble();
  formvalues.p          = ui->input_soil_p->text().toDouble();
  formvalues.CGsat      = ui->input_soil_CGsat->text().toDouble();

  // SURFACE
  formvalues.Ts         = ui->input_surface_Ts->text().toDouble();
  formvalues.Wl         = ui->input_surface_Wl->text().toDouble();
  formvalues.LAI        = ui->input_surface_LAI->text().toDouble();
  formvalues.gD         = ui->input_surface_gD->text().toDouble();
  formvalues.rsmin      = ui->input_surface_rsmin->text().toDouble();
  formvalues.alpha      = ui->input_surface_alpha->text().toDouble();
  formvalues.cveg       = ui->input_surface_cveg->text().toDouble();
  formvalues.Lambda     = ui->input_surface_Lambda->text().toDouble();
  formvalues.Wmax       = ui->input_surface_Wlmax->text().toDouble();
  formvalues.z0m        = ui->input_surface_z0m->text().toDouble();
  formvalues.z0h        = ui->input_surface_z0h->text().toDouble();

  // RADIATION
  formvalues.doy        = ui->input_rad_DOY->text().toDouble();
  formvalues.lat        = ui->input_rad_lat->text().toDouble();
  formvalues.lon        = ui->input_rad_lon->text().toDouble();
  formvalues.tstart     = ui->input_rad_time->text().toDouble();

  formvalues.Q          = ui->input_rad_Qnet->text().toDouble();
  formvalues.cc         = ui->input_rad_clouds->text().toDouble();

  // OTHER
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

    // SWITCHES
    Qt::CheckState check;
    if (modelrunlist->value(n).run->input.sw_wind == true)
      check = Qt::Checked;
    else
      check = Qt::Unchecked;
    ui->sw_wind->setCheckState(check);

    if (modelrunlist->value(n).run->input.sw_ml == true)
      check = Qt::Checked;
    else
      check = Qt::Unchecked;
    ui->sw_ml->setCheckState(check);

    if (modelrunlist->value(n).run->input.sw_rad == true)
      check = Qt::Checked;
    else
      check = Qt::Unchecked;
    ui->sw_rad->setCheckState(check);

    if (modelrunlist->value(n).run->input.sw_sl == true)
      check = Qt::Checked;
    else
      check = Qt::Unchecked;
    ui->sw_sl->setCheckState(check);

    if (modelrunlist->value(n).run->input.sw_ls == true)
      check = Qt::Checked;
    else
      check = Qt::Unchecked;
    ui->sw_ls->setCheckState(check);

    // MIXED-LAYER
    ui->input_ml_h->setText(QString::number(modelrunlist->value(n).run->input.h));
    ui->input_ml_ps->setText(QString::number(modelrunlist->value(n).run->input.Ps / 100.));
    ui->input_ml_ws->setText(QString::number(modelrunlist->value(n).run->input.ws));
    ui->input_ml_beta->setText(QString::number(modelrunlist->value(n).run->input.beta));

    // HEAT
    ui->input_heat_theta->setText(QString::number(modelrunlist->value(n).run->input.theta));
    ui->input_heat_dtheta->setText(QString::number(modelrunlist->value(n).run->input.dtheta));
    ui->input_heat_gammatheta->setText(QString::number(modelrunlist->value(n).run->input.gammatheta));
    ui->input_heat_advtheta->setText(QString::number(modelrunlist->value(n).run->input.advtheta));
    ui->input_heat_wtheta->setText(QString::number(modelrunlist->value(n).run->input.wtheta));

    // MOISTURE
    ui->input_moisture_q->setText(QString::number(modelrunlist->value(n).run->input.q * 1000.));
    ui->input_moisture_dq->setText(QString::number(modelrunlist->value(n).run->input.dq * 1000.));
    ui->input_moisture_gammaq->setText(QString::number(modelrunlist->value(n).run->input.gammaq * 1000.));
    ui->input_moisture_advq->setText(QString::number(modelrunlist->value(n).run->input.advq * 1000.));
    ui->input_moisture_wq->setText(QString::number(modelrunlist->value(n).run->input.wq * 1000.));

    // WIND
    ui->input_wind_u->setText(QString::number(modelrunlist->value(n).run->input.u));
    ui->input_wind_ug->setText(QString::number(modelrunlist->value(n).run->input.u + modelrunlist->value(n).run->input.du));
    ui->input_wind_gammau->setText(QString::number(modelrunlist->value(n).run->input.gammau));
    ui->input_wind_advu->setText(QString::number(modelrunlist->value(n).run->input.advu));

    ui->input_wind_v->setText(QString::number(modelrunlist->value(n).run->input.v));
    ui->input_wind_vg->setText(QString::number(modelrunlist->value(n).run->input.v + modelrunlist->value(n).run->input.dv));
    ui->input_wind_gammav->setText(QString::number(modelrunlist->value(n).run->input.gammav));
    ui->input_wind_advv->setText(QString::number(modelrunlist->value(n).run->input.advv));

    ui->input_wind_fc->setText(QString::number(modelrunlist->value(n).run->input.fc));
    ui->input_wind_ustar->setText(QString::number(modelrunlist->value(n).run->input.ustar));

    ui->input_surfacelayer_z0h->setText(QString::number(modelrunlist->value(n).run->input.z0h));
    ui->input_surfacelayer_z0m->setText(QString::number(modelrunlist->value(n).run->input.z0m));

    //if (modelrunlist->value(n).run->input.sw_wind == true)
      //ui->switch_wind->setChecked(true);
    //else
      //ui->switch_wind->setChecked(false);

    // SOIL
    ui->input_soil_T2->setText(QString::number(modelrunlist->value(n).run->input.T2));
    ui->input_soil_Tsoil->setText(QString::number(modelrunlist->value(n).run->input.Tsoil));
    ui->input_soil_W2->setText(QString::number(modelrunlist->value(n).run->input.w2));
    ui->input_soil_Wg->setText(QString::number(modelrunlist->value(n).run->input.wg));

    ui->input_soil_wsat->setText(QString::number(modelrunlist->value(n).run->input.wsat));
    ui->input_soil_wfc->setText(QString::number(modelrunlist->value(n).run->input.wfc));
    ui->input_soil_wwilt->setText(QString::number(modelrunlist->value(n).run->input.wwilt));

    ui->input_soil_c1sat->setText(QString::number(modelrunlist->value(n).run->input.C1sat));
    ui->input_soil_c2ref->setText(QString::number(modelrunlist->value(n).run->input.C2ref));

    ui->input_soil_a->setText(QString::number(modelrunlist->value(n).run->input.a));
    ui->input_soil_b->setText(QString::number(modelrunlist->value(n).run->input.b));
    ui->input_soil_p->setText(QString::number(modelrunlist->value(n).run->input.p));
    ui->input_soil_CGsat->setText(QString::number(modelrunlist->value(n).run->input.CGsat));

    // SURFACE
    ui->input_surface_Ts->setText(QString::number(modelrunlist->value(n).run->input.Ts));
    ui->input_surface_Wl->setText(QString::number(modelrunlist->value(n).run->input.Wl));

    ui->input_surface_LAI->setText(QString::number(modelrunlist->value(n).run->input.LAI));
    ui->input_surface_gD->setText(QString::number(modelrunlist->value(n).run->input.gD));
    ui->input_surface_rsmin->setText(QString::number(modelrunlist->value(n).run->input.rsmin));
    ui->input_surface_alpha->setText(QString::number(modelrunlist->value(n).run->input.alpha));
    ui->input_surface_cveg->setText(QString::number(modelrunlist->value(n).run->input.cveg));

    ui->input_surface_Lambda->setText(QString::number(modelrunlist->value(n).run->input.Lambda));
    ui->input_surface_Wlmax->setText(QString::number(modelrunlist->value(n).run->input.Wmax));
    ui->input_surface_z0m->setText(QString::number(modelrunlist->value(n).run->input.z0m));
    ui->input_surface_z0h->setText(QString::number(modelrunlist->value(n).run->input.z0h));

    // RADIATION
    ui->input_rad_DOY->setText(QString::number(modelrunlist->value(n).run->input.doy));
    ui->input_rad_lat->setText(QString::number(modelrunlist->value(n).run->input.lat));
    ui->input_rad_lon->setText(QString::number(modelrunlist->value(n).run->input.lon));
    ui->input_rad_time->setText(QString::number(modelrunlist->value(n).run->input.tstart));

    ui->input_rad_Qnet->setText(QString::number(modelrunlist->value(n).run->input.Q));
    ui->input_rad_clouds->setText(QString::number(modelrunlist->value(n).run->input.cc));

    // OTHER
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
  updateSelectedRuns();
}

void MainWindow::updateRunName()
{
  if(ui->modelRunTree->selectedItems().count() > 0)
    ui->modelRunTree->currentItem()->setText(1,ui->input_name->text());
}

void MainWindow::wind_switch(int state)
{
  bool checkstate;
  if (state == Qt::Checked)
    checkstate = true;
  else
    checkstate = false;

  formvalues.sw_wind = checkstate;
  ui->wind_U_group->setEnabled(checkstate);
  ui->wind_V_group->setEnabled(checkstate);
}

void MainWindow::startrun()
{
  if(ui->modelRunTree->selectedItems().count() > 0)
  {
    for (int i=0; i<ui->modelRunTree->selectedItems().count(); i++)
    {
      updateInputdata();
      int id = ui->modelRunTree->selectedItems()[i]->text(0).toInt();
      modelrunlist->find(id).value().previnput = modelrunlist->find(id).value().run->input;
      modelrunlist->find(id).value().run->runmodel();
      modelrunlist->find(id).value().hasrun = true;

      QFont font;
      font = ui->modelRunTree->selectedItems()[i]->font(1);
      font.setItalic(false);
      ui->modelRunTree->selectedItems()[i]->setFont(1,font);
      ui->modelRunTree->selectedItems()[i]->setTextColor(1,Qt::black);
      updateSelectedRuns();
      emit runadded(id);
    }
  }
}


void MainWindow::canceledit()
{
  int id = ui->modelRunTree->currentItem()->text(0).toInt();
  modelrunlist->find(id).value().run->input = modelrunlist->find(id).value().previnput;
  updateForm();
}

void MainWindow::startGraph()
{
  showGraph(modelrunlist,selectedruns);
}

void MainWindow::showGraph(QMap<int, modelrun> *main, QList<int> *selected)
{
  graph = new plotwindow(main, selected, this);
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
      QString ident    = ui->modelRunTree->selectedItems()[i]->text(0);
      QString runname0 = ui->modelRunTree->selectedItems()[i]->text(1);

      QString runname  = runname0;

      QString testfile = dirname + "/" + runname + ".csv";

      QString istr;
      int i = 0;
      while (QFile::exists(testfile))
      {
        runname  = runname0 + "_" + istr.setNum(i);
        testfile = dirname + "/" + runname + ".csv";
        i++;
      }

      int n = ident.toInt(0,10);
      if (modelrunlist->find(n).value().hasrun)
        modelrunlist->find(n).value().run->run2file(dirname.toStdString(),runname.toStdString());
    }
  }
}

//void MainWindow::switches_changed()
//{
//  QTreeWidgetItem *mixedlayer = new QTreeWidgetItem;
//  mixedlayer = ui->switches_treewidget->findItems("1", Qt::MatchExactly, 1)[0];
//  if (mixedlayer->checkState(0) == 0)
//    formvalues.sw_ml = false;
//  else if (mixedlayer->checkState(0) == 2)
//    formvalues.sw_ml = true;
//
//  QTreeWidgetItem *wind = new QTreeWidgetItem;
//  wind = ui->switches_treewidget->findItems("4", Qt::MatchRecursive, 1)[0];
//  if (wind->checkState(0) == 0)
//    formvalues.sw_wind = false;
//  else if (wind->checkState(0) == 2)
//    formvalues.sw_wind = true;
//  ui->wind_U_group->setEnabled(formvalues.sw_wind);
//  ui->wind_V_group->setEnabled(formvalues.sw_wind);
//  ui->wind_general_group->setEnabled(formvalues.sw_wind);
//
//
//  QTreeWidgetItem *surfacelayer = new QTreeWidgetItem;
//  surfacelayer = ui->switches_treewidget->findItems("5", Qt::MatchExactly, 1)[0];
//  if (surfacelayer->checkState(0) == 0)
//    formvalues.sw_sl = false;
//  else if (surfacelayer->checkState(0) == 2)
//    formvalues.sw_sl = true;
//  ui->surfacelayer_group->setEnabled(formvalues.sw_sl);
//
//
//  QTreeWidgetItem *landsurface = new QTreeWidgetItem;
//  landsurface = ui->switches_treewidget->findItems("6", Qt::MatchExactly, 1)[0];
//  if (landsurface->checkState(0) == 0)
//    formvalues.sw_ls = false;
//  else if (landsurface->checkState(0) == 2)
//    formvalues.sw_ls = true;
//  ui->soil_heat_group->setEnabled(formvalues.sw_ls);
//  ui->soil_moisture_group->setEnabled(formvalues.sw_ls);
//  ui->soil_parameters_group->setEnabled(formvalues.sw_ls);
//
//  QTreeWidgetItem *radiation = new QTreeWidgetItem;
//  radiation = ui->switches_treewidget->findItems("7", Qt::MatchExactly, 1)[0];
//  if (radiation->checkState(0) == 0)
//    formvalues.sw_rad = false;
//  else if (radiation->checkState(0) == 2)
//    formvalues.sw_rad = true;
//  ui->rad_group->setEnabled(formvalues.sw_rad);
//  ui->rad_group2->setEnabled(formvalues.sw_rad);
//}
