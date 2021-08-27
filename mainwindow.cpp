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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "plotwindow.h"

#include <string>    // Needed for basename modelrun
#include <sstream>   // Needed for basename modelrun
#include <iostream>
#include <QString>
#include <QFileDialog>
#include <QFont>
#include <QTextStream>
#include "QMessageBox"
#include "QSysInfo"
#include "QString"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  connect(ui->actionExit,     SIGNAL(triggered()),              this, SLOT(close()));
  connect(ui->actionSave,     SIGNAL(triggered()),              this, SLOT(saveRuns()));
  connect(ui->actionLoad,     SIGNAL(triggered()),              this, SLOT(loadRuns()));
  connect(ui->actionReset,    SIGNAL(triggered()),              this, SLOT(resetInterface()));

  connect(ui->startButton,    SIGNAL(clicked()),                this, SLOT(startrun()));
  connect(ui->cancelButton,   SIGNAL(clicked()),                this, SLOT(canceledit()));
  connect(ui->newRunButton,   SIGNAL(clicked()),                this, SLOT(newrun()));
  connect(ui->cloneRunButton, SIGNAL(clicked()),                this, SLOT(clonerun()));
  connect(ui->modelRunTree,   SIGNAL(itemSelectionChanged()),   this, SLOT(runTreeChanged()));
  connect(ui->modelRunTree,   SIGNAL(pressed(QModelIndex)),     this, SLOT(runTreePressed(QModelIndex)));
  connect(ui->deleteButton,   SIGNAL(clicked()),                this, SLOT(deleteRun()));
  connect(ui->graphButton,    SIGNAL(clicked()),                this, SLOT(startGraph()));
  connect(ui->input_name,     SIGNAL(textChanged(QString)),     this, SLOT(updateRunName(QString)));
  connect(ui->exportButton,   SIGNAL(clicked()),                this, SLOT(exportRuns()));
  connect(ui->tabWidget,      SIGNAL(currentChanged(int)),      this, SLOT(tabChanged(int)));

  connect(ui->species_treewidget,         SIGNAL(itemSelectionChanged()),   this, SLOT(speciesselectionchanged()));

  connect(ui->actionAbout,    SIGNAL(triggered()),              this, SLOT(showAbout()));

  // Switches
  connect(ui->input_surface_surfacetypes, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSurfacetype(int)));
  connect(ui->input_soil_soiltypes,       SIGNAL(currentIndexChanged(int)), this, SLOT(updateSoiltype(int)));
  connect(ui->sw_wtheta,                  SIGNAL(stateChanged(int)),        this, SLOT(switch_wtheta(int)));
  connect(ui->sw_wq,                      SIGNAL(stateChanged(int)),        this, SLOT(switch_wq(int)));
  connect(ui->sw_wind,                    SIGNAL(stateChanged(int)),        this, SLOT(switch_wind(int)));
  connect(ui->sw_sl,                      SIGNAL(stateChanged(int)),        this, SLOT(switch_sl(int)));
  connect(ui->sw_ls,                      SIGNAL(stateChanged(int)),        this, SLOT(switch_ls(int)));
  connect(ui->sw_sea,                     SIGNAL(currentIndexChanged(int)), this, SLOT(switch_sea(int)));
  connect(ui->sw_rad,                     SIGNAL(stateChanged(int)),        this, SLOT(switch_rad(int)));
  connect(ui->sw_ml,                      SIGNAL(stateChanged(int)),        this, SLOT(switch_ml(int)));
  connect(ui->sw_cu,                      SIGNAL(stateChanged(int)),        this, SLOT(switch_cu(int)));
  connect(ui->sw_cu_rad,                  SIGNAL(stateChanged(int)),        this, SLOT(switch_curad(int)));
  connect(ui->sw_chem,                    SIGNAL(stateChanged(int)),        this, SLOT(switch_chem(int)));
  connect(ui->sw_chem_constant,           SIGNAL(stateChanged(int)),        this, SLOT(switch_chem_constant(int)));
  connect(ui->sw_species_photolysis,      SIGNAL(stateChanged(int)),        this, SLOT(switch_photolysis(int)));
  connect(ui->sw_surface_advanced,        SIGNAL(stateChanged(int)),        this, SLOT(switch_surface_advanced(int)));
  connect(ui->sw_soil_advanced,           SIGNAL(stateChanged(int)),        this, SLOT(switch_soil_advanced(int)));
  connect(ui->sw_jarvis,                  SIGNAL(currentIndexChanged(int)), this, SLOT(setLandSoil(int)));

  connect(ui->input_reactions_nonebutton, SIGNAL(clicked()),                this, SLOT(setNoReactions()));
  connect(ui->input_reactions_simplebutton, SIGNAL(clicked()),              this, SLOT(setSimpleReactions()));
  connect(ui->input_reactions_complexbutton, SIGNAL(clicked()),             this, SLOT(setComplexReactions()));

  // loadfieldslots();
  //numgraphs = 0;
  readdefaultinput();

  // Setup QTreeWidget with model runs
  QStringList heading;
  heading << "ID" << "Name";
  ui->modelRunTree->setColumnCount(2);
  ui->modelRunTree->setHeaderLabels(heading);
  ui->modelRunTree->setColumnWidth(0,35);
  ui->modelRunTree->hideColumn(0);
  ui->modelRunTree->setSelectionMode(QAbstractItemView::ExtendedSelection);

  // Setup QTreeWidget with chemical species
  QList<int> visible_species;         // Posibility to show only a subset of the species.
  visible_species << 0 << 1 << 3 << 4 << 9 << 5 << 13 << 6 << 7 << 8 << 11 << 12 << 19 << 18 << 20 << 21;

  QStringList chemheading;
  chemheading << "ID" << "Species";
  ui->species_treewidget->setColumnCount(2);
  ui->species_treewidget->setHeaderLabels(chemheading);
  ui->species_treewidget->setColumnWidth(0,35);
  ui->species_treewidget->hideColumn(0);
  ui->species_treewidget->setSelectionMode(QAbstractItemView::SingleSelection);

  modeloutput modelout(0,22);

  for (int i=0; i<visible_species.size(); i++)
  {
    QTreeWidgetItem *point = new QTreeWidgetItem(ui->species_treewidget);
    point->setText(0, QString::number(visible_species.value(i)));
    point->setText(1, QString::fromStdString(modelout.sc[visible_species.value(i)].name));
  }

  activerun = -1;
  activespecies = visible_species[0];
  ui->species_treewidget->setCurrentItem(ui->species_treewidget->topLevelItem(0));
  // End species

  modelrunlist = new QMap<int, modelrun>;
  selectedruns = new QList<int>;

  // BvS, moved from setLandSoil()
  initLandSoil();
  // Initialize surface and soil types
  setLandSoil(0);

  newrun();
  ui->modelRunTree->setCurrentItem(ui->modelRunTree->topLevelItem(0));

  // CvH test blocking events to prevent multiple triggering of identical functions
  blockInput(true);

  ui->surface_advanced_group->setEnabled(false);
  ui->soil_advanced_group->setEnabled(false);

  // CvH test blocking events to prevent multiple triggering of identical functions
  blockInput(false);

  // if all fields are properly assigned, the next line can be removed
  formvalues            = defaultinput;

  defaultname = "default.mxl";
  casename = defaultname;
  this->setWindowTitle("CLASS main | " + casename);

  //std::cout << "githash=" << GITHASH << std::endl;
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  closeCheck = true;
  if(modelrunlist->size() > 0)
    closeWarning();

  if (plotwindowList.size() > 0 && closeCheck == true)
  {
    blockInput(true);
    for (int i = 0; i < plotwindowList.size(); i++)
      plotwindowList.value(i)->close();
  }

  if(!closeCheck)
    event->ignore();
}

void MainWindow::speciesselectionchanged()
{
  storeFormData();
  loadFormData();
  activespecies = ui->species_treewidget->currentItem()->text(0).toInt();
}

void MainWindow::blockInput(bool check)
{
  ui->input_surface_surfacetypes->blockSignals(check);
  ui->input_soil_soiltypes->blockSignals(check);
  ui->modelRunTree->blockSignals(check);
  ui->sw_wtheta->blockSignals(check);
  ui->sw_wq->blockSignals(check);
  ui->sw_wind->blockSignals(check);
  ui->sw_sl->blockSignals(check);
  ui->sw_ls->blockSignals(check);
  ui->sw_rad->blockSignals(check);
  ui->sw_ml->blockSignals(check);
  ui->sw_surface_advanced->blockSignals(check);
  ui->sw_soil_advanced->blockSignals(check);
  if (plotwindowList.size() > 0)
  {
    for (int i = 0; i < plotwindowList.size(); i++)
      plotwindowList.value(i)->blockSignals(check);
  }
}

void MainWindow::tabChanged(int)
{
  storeFormData();
}

void MainWindow::newrun()
{
  // CvH test blocking events to prevent multiple triggering of identical functions
  blockInput(true);

  if(activerun != -1)
    storeFormData();

  modelrun run(&defaultinput);
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

  modelrunlist->find(max+1).value().run->input = defaultinput;
  modelrunlist->find(max+1).value().previnput  = defaultinput;

  QTreeWidgetItem *point = new QTreeWidgetItem(ui->modelRunTree);
  point->setText(0, QString::number(max+1));
  point->setText(1, base);

  QFont font;
  font = point->font(1);
  font.setItalic(true);
  point->setFont(1, font);
  QBrush brush(Qt::lightGray);
  point->setForeground(0, brush);

  ui->modelRunTree->setCurrentItem(point);

  runTreeChanged();
  updateSurfacetype(0);

  // CvH runTreeChanged also triggers loadFormData
  // loadFormData();
  // CvH test blocking events to prevent multiple triggering of identical functions
  blockInput(false);
}

void MainWindow::clonerun()
{
  storeFormData();

  for (int n=0; n<ui->modelRunTree->selectedItems().count(); n++)
  {
    modelrun run(&defaultinput);
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
    QString base = modelrunlist->find(id).value().runname;
    QString append = " (clone)";
    base.append(append);
    run.runname.append(base);
    modelrunlist->insert((max+1),run);

    //modelrunlist->value(max+1).run->input       = modelrunlist->value(id).run->input;
    modelrunlist->find(max+1).value().run->input = modelrunlist->find(id).value().run->input;
    modelrunlist->find(max+1).value().previnput  = modelrunlist->find(id).value().previnput;

    modelrunlist->find(max+1).value().surfacestatus   = modelrunlist->find(id).value().surfacestatus;
    modelrunlist->find(max+1).value().soilstatus      = modelrunlist->find(id).value().soilstatus;
    modelrunlist->find(max+1).value().surfaceadvanced = modelrunlist->find(id).value().surfaceadvanced;
    modelrunlist->find(max+1).value().soiladvanced    = modelrunlist->find(id).value().soiladvanced;

    QTreeWidgetItem *point = new QTreeWidgetItem(ui->modelRunTree);
    point->setText(0, QString::number(max+1));
    point->setText(1, base);

    QFont font;
    font = point->font(1);
    font.setItalic(true);
    point->setFont(1,font);
    QBrush brush(Qt::lightGray);
    point->setForeground(0, brush);

    if(n == (ui->modelRunTree->selectedItems().count() - 1))
      ui->modelRunTree->setCurrentItem(point);
  }

  loadFormData();
}

void MainWindow::runTreePressed(QModelIndex dummy)
{
   runTreeChanged();
}

void MainWindow::runTreeChanged()
{
  // CvH test blocking events to prevent multiple triggering of identical functions
  blockInput(true);

  if(activerun != -1)
    storeFormData();

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
  ui->input_name->setEnabled(inputfields);
  
  updateSelectedRuns();

  if (activerun != -1 && ui->modelRunTree->selectedItems().size() != 0)
  {
    if(modelrunlist->find(activerun).value().previnput != modelrunlist->find(activerun).value().run->input)
    {
      QFont font;
      QString id = QString::number(activerun);
      font = ui->modelRunTree->findItems(id,Qt::MatchExactly)[0]->font(1);
      font.setItalic(true);
      ui->modelRunTree->findItems(id, Qt::MatchExactly)[0]->setFont(1,font);
      QBrush brush(Qt::lightGray);
      ui->modelRunTree->findItems(id,Qt::MatchExactly)[0]->setForeground(0, brush);
    }
    else
    {
      QFont font;
      QString id = QString::number(activerun);
      font = ui->modelRunTree->findItems(id,Qt::MatchExactly)[0]->font(1);
      font.setItalic(false);
      ui->modelRunTree->findItems(id,Qt::MatchExactly)[0]->setFont(1,font);
      QBrush brush(Qt::black);
      ui->modelRunTree->findItems(id,Qt::MatchExactly)[0]->setForeground(0, brush);
    }
  }

  loadFormData();

  // CvH test blocking events to prevent multiple triggering of identical functions
  blockInput(false);
}

void MainWindow::updateSelectedRuns()
{
  selectedruns->clear();
  for (int i = 0; i < ui->modelRunTree->topLevelItemCount(); i++)
  {
    QTreeWidgetItem *item =  ui->modelRunTree->topLevelItem (i);
      if (item->isSelected() && modelrunlist->find(item->text(0).toInt()).value().hasrun)
        selectedruns->append(item->text(0).toInt());
  }
}

void MainWindow::storeFormData()
{
  // TAB 1
  formvalues.dt         = ui->input_timestep->text().toDouble();        // time step [s]
  formvalues.runtime    = ui->input_time->text().toDouble() * 3600;     // total run time [s]
  formvalues.sinperiod  = ui->input_sinperiod->text().toDouble() * 3600;// period for sinusoidal fluxes

  // MIXED-LAYER
  formvalues.sw_ml      = CheckState2bool(ui->sw_ml->checkState());
  formvalues.sw_ftcws   = CheckState2bool(ui->sw_ftcws->checkState());
  formvalues.sw_shearwe = CheckState2bool(ui->sw_shearwe->checkState());
  formvalues.h          = ui->input_ml_h->text().toDouble();             // initial ABL height [m]
  formvalues.Ps         = ui->input_ml_ps->text().toDouble() * 100;      // surface pressure [Pa]
  formvalues.omegas     = ui->input_ml_omegas->text().toDouble();        // large scale vertical velocity [m s-1]
  formvalues.beta       = ui->input_ml_beta->text().toDouble();          // entrainment ratio for virtual heat [-]

  // HEAT
  formvalues.theta      = ui->input_heat_theta->text().toDouble();       // initial mixed-layer potential temperature [K]
  formvalues.dtheta     = ui->input_heat_dtheta->text().toDouble();      // initial temperature jump at h [K]
  formvalues.gammatheta = ui->input_heat_gammatheta->text().toDouble();  // free atmosphere potential temperature lapse rate [K m-1]
  formvalues.advtheta   = ui->input_heat_advtheta->text().toDouble();    // advection of heat [K s-1]
  formvalues.wtheta     = ui->input_heat_wtheta->text().toDouble();        // surface kinematic heat flux [K m s-1]
  formvalues.sw_wtheta  = CheckState2bool(ui->sw_wtheta->checkState());

  // MOISTURE
  formvalues.q          = ui->input_moisture_q->text().toDouble() / 1000;     // initial mixed-layer specific humidity [kg kg-1]
  formvalues.dq         = ui->input_moisture_dq->text().toDouble() / 1000;    // initial specific humidity jump at h [kg kg-1]
  formvalues.gammaq     = ui->input_moisture_gammaq->text().toDouble() / 1000;// free atmosphere specific humidity lapse rate [kg kg-1 m-1]
  formvalues.advq       = ui->input_moisture_advq->text().toDouble() / 1000;  // advection of moisture [kg kg-1 s-1]
  formvalues.wq         = ui->input_moisture_wq->text().toDouble() / 1000;    // surface kinematic moisture flux [kg kg-1 m s-1]
  formvalues.sw_wq      = CheckState2bool(ui->sw_wq->checkState());
  // END TAB1

  // TAB2
  // WIND
  formvalues.sw_wind    = CheckState2bool(ui->sw_wind->checkState());
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

  formvalues.ustar      = ui->input_wind_ustar->text().toDouble();        // surface friction velocity [m s-1]
  formvalues.fc         = ui->input_wind_fc->text().toDouble();           // Coriolis parameter [m s-1]

  formvalues.sw_sl      = CheckState2bool(ui->sw_sl->checkState());

  // SCALAR / CO2
  formvalues.sca        = ui->input_sca_sca->text().toDouble();
  formvalues.dsca       = ui->input_sca_dsca->text().toDouble();
  formvalues.gammasca   = ui->input_sca_gammasca->text().toDouble();
  formvalues.advsca     = ui->input_sca_advsca->text().toDouble();
  formvalues.wsca       = ui->input_sca_wsca->text().toDouble();

  formvalues.CO2        = ui->input_CO2_CO2->text().toDouble();
  formvalues.dCO2       = ui->input_CO2_dCO2->text().toDouble();
  formvalues.gammaCO2   = ui->input_CO2_gammaCO2->text().toDouble();
  formvalues.advCO2     = ui->input_CO2_CO2adv->text().toDouble();
  formvalues.wCO2       = ui->input_CO2_wCO2->text().toDouble() * (28.9/(1.2*46.));  // Conversion mgCO2 m-2 s-1 to ppm

  //if (activetab == 1)
  //{
    formvalues.z0m                = ui->input_surfacelayer_z0m->text().toDouble();
    formvalues.z0h                = ui->input_surfacelayer_z0h->text().toDouble();
  //}
  // END TAB2

  // TAB3
  formvalues.sw_ls      = CheckState2bool(ui->sw_ls->checkState());
  if(ui->sw_jarvis->currentIndex() == 0)
    formvalues.sw_jarvis = true;
  else
    formvalues.sw_jarvis = false;

  if(ui->sw_sea->currentIndex() == 0)
    formvalues.sw_sea   = false;
  else
    formvalues.sw_sea   = true;

  formvalues.C3C4       = surfacetypes[ui->input_surface_surfacetypes->currentIndex()].C3C4;

  // SURFACE
  formvalues.Ts         = ui->input_surface_Ts->text().toDouble();
  formvalues.Wl         = ui->input_surface_Wl->text().toDouble();
  formvalues.LAI        = ui->input_surface_LAI->text().toDouble();
  formvalues.gD         = ui->input_surface_gD->text().toDouble();
  formvalues.rsmin      = ui->input_surface_rsmin->text().toDouble();
  formvalues.alpha      = ui->input_surface_alpha->text().toDouble();
  formvalues.cveg       = ui->input_surface_cveg->text().toDouble();
  formvalues.Lambda     = ui->input_surface_Lambda->text().toDouble();

  //if (activetab == 3)
  //{
  //  formvalues.z0m      = ui->input_surface_z0m->text().toDouble();
  //  formvalues.z0h      = ui->input_surface_z0h->text().toDouble();
  //}
  // END TAB3

  // TAB4
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
  // END TAB4

  // TAB5
  // RADIATION
  formvalues.sw_rad     = CheckState2bool(ui->sw_rad->checkState());
  formvalues.doy        = ui->input_rad_DOY->text().toDouble();
  formvalues.lat        = ui->input_rad_lat->text().toDouble();
  formvalues.lon        = ui->input_rad_lon->text().toDouble() * -1.;
  formvalues.tstart     = ui->input_rad_time->text().toDouble();

  formvalues.Q          = ui->input_rad_Qnet->text().toDouble();
  formvalues.cc         = ui->input_rad_clouds->text().toDouble();

  formvalues.sw_cu      = CheckState2bool(ui->sw_cu->checkState());
  formvalues.sw_curad   = CheckState2bool(ui->sw_cu_rad->checkState());

  formvalues.dFz        = ui->input_rad_dFz->text().toDouble();
  // END TAB5

  // TAB6
  // SPECIES
  formvalues.sw_chem    = CheckState2bool(ui->sw_chem->checkState());
  // SPECIES - PHOTOLYSIS
  formvalues.sw_photo_constant = CheckState2bool(ui->sw_species_photolysis->checkState());
  formvalues.tod_ref    = ui->input_species_photolysis_tref->text().toDouble();

  // CONSTANT CHEMISTRY
  formvalues.sw_chem_constant = CheckState2bool(ui->sw_chem_constant->checkState());
  formvalues.Tcbl_ref   = ui->input_species_ref_Tcbl->text().toDouble();
  formvalues.Tfc_ref    = ui->input_species_ref_Tft->text().toDouble();
  formvalues.qcbl_ref   = ui->input_species_ref_qcbl->text().toDouble() / 1000.;
  formvalues.qfc_ref    = ui->input_species_ref_qft->text().toDouble()  / 1000.;
  formvalues.P_ref      = ui->input_species_ref_pref->text().toDouble() * 100.;

  // SPECIES - PROPERTIES
  formvalues.sc[activespecies]        = ui->input_species_scalar->text().toDouble();
  formvalues.dsc[activespecies]       = ui->input_species_dscalar->text().toDouble();
  formvalues.gammasc[activespecies]   = ui->input_species_gammascalar->text().toDouble();
  formvalues.wsc[activespecies]       = ui->input_species_wscalar->text().toDouble();
  formvalues.advsc[activespecies]     = ui->input_species_advscalar->text().toDouble();
  formvalues.sw_wsc[activespecies]    = ui->selector_species_diurnal->currentIndex();

  // TAB 7
  // EQUATIONS
  formvalues.stocoef              = ui->input_reactions_OHrecycling->value();
  formvalues.sw_reactions[0]      = CheckState2bool(ui->sw_R1->checkState());
  formvalues.sw_reactions[1]      = CheckState2bool(ui->sw_R2->checkState());
  formvalues.sw_reactions[2]      = CheckState2bool(ui->sw_R3->checkState());
  formvalues.sw_reactions[3]      = CheckState2bool(ui->sw_R4->checkState());
  formvalues.sw_reactions[4]      = CheckState2bool(ui->sw_R5->checkState());
  formvalues.sw_reactions[5]      = CheckState2bool(ui->sw_R6->checkState());
  formvalues.sw_reactions[6]      = CheckState2bool(ui->sw_R7->checkState());
  formvalues.sw_reactions[7]      = CheckState2bool(ui->sw_R8->checkState());
  formvalues.sw_reactions[8]      = CheckState2bool(ui->sw_R9->checkState());
  formvalues.sw_reactions[9]      = CheckState2bool(ui->sw_R10->checkState());
  formvalues.sw_reactions[10]     = CheckState2bool(ui->sw_R11->checkState());
  formvalues.sw_reactions[11]     = CheckState2bool(ui->sw_R12->checkState());
  formvalues.sw_reactions[12]     = CheckState2bool(ui->sw_R13->checkState());
  formvalues.sw_reactions[13]     = CheckState2bool(ui->sw_R14->checkState());
  formvalues.sw_reactions[14]     = CheckState2bool(ui->sw_R15->checkState());
  formvalues.sw_reactions[15]     = CheckState2bool(ui->sw_R16->checkState());
  formvalues.sw_reactions[16]     = CheckState2bool(ui->sw_R17->checkState());
  formvalues.sw_reactions[17]     = CheckState2bool(ui->sw_R18->checkState());
  formvalues.sw_reactions[18]     = CheckState2bool(ui->sw_R19->checkState());
  formvalues.sw_reactions[19]     = CheckState2bool(ui->sw_R20->checkState());
  formvalues.sw_reactions[20]     = CheckState2bool(ui->sw_R21->checkState());
  formvalues.sw_reactions[21]     = CheckState2bool(ui->sw_R22->checkState());
  formvalues.sw_reactions[22]     = CheckState2bool(ui->sw_R23->checkState());
  formvalues.sw_reactions[23]     = CheckState2bool(ui->sw_R24->checkState());
  formvalues.sw_reactions[24]     = CheckState2bool(ui->sw_R25->checkState());
  formvalues.sw_reactions[25]     = CheckState2bool(ui->sw_R26->checkState());
  formvalues.sw_reactions[26]     = CheckState2bool(ui->sw_R27->checkState());
  formvalues.sw_reactions[27]     = CheckState2bool(ui->sw_R28->checkState());

  // OTHER
  QString name          = QString::fromStdString(ui->input_name->text().toStdString());

//  if (ui->modelRunTree->selectedItems().size() == 1)                  // Extra check if QTreeWidget has selected item
//  {
//    int id = ui->modelRunTree->currentItem()->text(0).toInt();
//    modelrunlist->value(id).run->input = formvalues;
//    modelrunlist->find(id).value().runname = name;
//
//    updateForm();
//  }

  if (activerun != -1) // && ui->modelRunTree->selectedItems().size() == 1)                  // Extra check if QTreeWidget has selected item
  {
    QMap<int, modelrun>::const_iterator i = modelrunlist->find(activerun);
    if(i != modelrunlist->end())
    {
      modelrunlist->find(activerun).value().run->input = formvalues;
      modelrunlist->find(activerun).value().runname    = name;

      modelrunlist->find(activerun).value().surfacestatus = ui->input_surface_surfacetypes->currentIndex();
      modelrunlist->find(activerun).value().soilstatus    = ui->input_soil_soiltypes->currentIndex();

      modelrunlist->find(activerun).value().surfaceadvanced = CheckState2bool(ui->sw_surface_advanced->checkState());
      modelrunlist->find(activerun).value().soiladvanced    = CheckState2bool(ui->sw_soil_advanced->checkState());
    }
    // updateForm();
  }

  // only change tabindex after storing all data;
  activetab = ui->tabWidget->currentIndex();
}

void MainWindow::loadFormData()
{
  if (ui->modelRunTree->selectedItems().size() == 1)                  // Extra check if QTreeWidget has selected item
  {
    int n = ui->modelRunTree->currentItem()->text(0).toInt();

    activerun = n;

    formvalues = modelrunlist->find(n).value().run->input;

    // set the pull down menus correctly
    ui->input_surface_surfacetypes->setCurrentIndex(modelrunlist->find(n).value().surfacestatus);
    ui->input_soil_soiltypes->setCurrentIndex(modelrunlist->find(n).value().soilstatus);

    ui->input_timestep->setText(QString::number(formvalues.dt));
    ui->input_time->setText(QString::number(formvalues.runtime / 3600.));
    ui->input_sinperiod->setText(QString::number(formvalues.sinperiod / 3600.));

    // SWITCHES
    //Qt::CheckState check;
    ui->sw_wind->setCheckState(Bool2CheckState(formvalues.sw_wind));
      switch_wind(Bool2Int(formvalues.sw_wind));
    ui->sw_ml->setCheckState(Bool2CheckState(formvalues.sw_ml));
    ui->sw_ftcws->setCheckState(Bool2CheckState(formvalues.sw_ftcws));
    ui->sw_shearwe->setCheckState(Bool2CheckState(formvalues.sw_shearwe));
    ui->sw_cu->setCheckState(Bool2CheckState(formvalues.sw_cu));
      switch_cu(Bool2Int(formvalues.sw_cu));
    ui->sw_cu_rad->setCheckState(Bool2CheckState(formvalues.sw_curad));
    ui->sw_rad->setCheckState(Bool2CheckState(formvalues.sw_rad));
      switch_rad(Bool2Int(formvalues.sw_rad));
    ui->sw_sl->setCheckState(Bool2CheckState(formvalues.sw_sl));
      switch_sl(Bool2Int(formvalues.sw_sl));
    ui->sw_ls->setCheckState(Bool2CheckState(formvalues.sw_ls));
      switch_ls(Bool2Int(formvalues.sw_ls));
    ui->sw_wtheta->setCheckState(Bool2CheckState(formvalues.sw_wtheta));
    ui->sw_wq->setCheckState(Bool2CheckState(formvalues.sw_wq));
    ui->sw_chem->setCheckState(Bool2CheckState(formvalues.sw_chem));
      switch_chem(Bool2Int(formvalues.sw_chem));
    ui->sw_chem_constant->setCheckState(Bool2CheckState(formvalues.sw_chem_constant));
    ui->sw_species_photolysis->setCheckState(Bool2CheckState(formvalues.sw_photo_constant));

    ui->sw_R1->setCheckState(Bool2CheckState(formvalues.sw_reactions[0]));
    ui->sw_R2->setCheckState(Bool2CheckState(formvalues.sw_reactions[1]));
    ui->sw_R3->setCheckState(Bool2CheckState(formvalues.sw_reactions[2]));
    ui->sw_R4->setCheckState(Bool2CheckState(formvalues.sw_reactions[3]));
    ui->sw_R5->setCheckState(Bool2CheckState(formvalues.sw_reactions[4]));
    ui->sw_R6->setCheckState(Bool2CheckState(formvalues.sw_reactions[5]));
    ui->sw_R7->setCheckState(Bool2CheckState(formvalues.sw_reactions[6]));
    ui->sw_R8->setCheckState(Bool2CheckState(formvalues.sw_reactions[7]));
    ui->sw_R9->setCheckState(Bool2CheckState(formvalues.sw_reactions[8]));
    ui->sw_R10->setCheckState(Bool2CheckState(formvalues.sw_reactions[9]));
    ui->sw_R11->setCheckState(Bool2CheckState(formvalues.sw_reactions[10]));
    ui->sw_R12->setCheckState(Bool2CheckState(formvalues.sw_reactions[11]));
    ui->sw_R13->setCheckState(Bool2CheckState(formvalues.sw_reactions[12]));
    ui->sw_R14->setCheckState(Bool2CheckState(formvalues.sw_reactions[13]));
    ui->sw_R15->setCheckState(Bool2CheckState(formvalues.sw_reactions[14]));
    ui->sw_R16->setCheckState(Bool2CheckState(formvalues.sw_reactions[15]));
    ui->sw_R17->setCheckState(Bool2CheckState(formvalues.sw_reactions[16]));
    ui->sw_R18->setCheckState(Bool2CheckState(formvalues.sw_reactions[17]));
    ui->sw_R19->setCheckState(Bool2CheckState(formvalues.sw_reactions[18]));
    ui->sw_R20->setCheckState(Bool2CheckState(formvalues.sw_reactions[19]));
    ui->sw_R21->setCheckState(Bool2CheckState(formvalues.sw_reactions[20]));
    ui->sw_R22->setCheckState(Bool2CheckState(formvalues.sw_reactions[21]));
    ui->sw_R23->setCheckState(Bool2CheckState(formvalues.sw_reactions[22]));
    ui->sw_R24->setCheckState(Bool2CheckState(formvalues.sw_reactions[23]));
    ui->sw_R25->setCheckState(Bool2CheckState(formvalues.sw_reactions[24]));
    ui->sw_R26->setCheckState(Bool2CheckState(formvalues.sw_reactions[25]));
    ui->sw_R27->setCheckState(Bool2CheckState(formvalues.sw_reactions[26]));
    ui->sw_R28->setCheckState(Bool2CheckState(formvalues.sw_reactions[27]));

    if(formvalues.sw_sea == true)
      ui->sw_sea->setCurrentIndex(1);
    else
      ui->sw_sea->setCurrentIndex(0);

    if(formvalues.sw_jarvis == true)
      ui->sw_jarvis->setCurrentIndex(0);
    else
      ui->sw_jarvis->setCurrentIndex(1);

    // MIXED-LAYER
    ui->input_ml_h->setText(QString::number(formvalues.h));
    ui->input_ml_ps->setText(QString::number(formvalues.Ps / 100.));
    ui->input_ml_omegas->setText(QString::number(formvalues.omegas));
    ui->input_ml_beta->setText(QString::number(formvalues.beta));

    // HEAT
    ui->input_heat_theta->setText(QString::number(formvalues.theta));
    ui->input_heat_dtheta->setText(QString::number(formvalues.dtheta));
    ui->input_heat_gammatheta->setText(QString::number(formvalues.gammatheta));
    ui->input_heat_advtheta->setText(QString::number(formvalues.advtheta));
    ui->input_heat_wtheta->setText(QString::number(formvalues.wtheta));

    // MOISTURE
    ui->input_moisture_q->setText(QString::number(formvalues.q * 1000.));
    ui->input_moisture_dq->setText(QString::number(formvalues.dq * 1000.));
    ui->input_moisture_gammaq->setText(QString::number(formvalues.gammaq * 1000.));
    ui->input_moisture_advq->setText(QString::number(formvalues.advq * 1000.));
    ui->input_moisture_wq->setText(QString::number(formvalues.wq * 1000.));

    // WIND
    ui->input_wind_u->setText(QString::number(formvalues.u));
    ui->input_wind_ug->setText(QString::number(formvalues.u + formvalues.du));
    ui->input_wind_gammau->setText(QString::number(formvalues.gammau));
    ui->input_wind_advu->setText(QString::number(formvalues.advu));

    ui->input_wind_v->setText(QString::number(formvalues.v));
    ui->input_wind_vg->setText(QString::number(formvalues.v + formvalues.dv));
    ui->input_wind_gammav->setText(QString::number(formvalues.gammav));
    ui->input_wind_advv->setText(QString::number(formvalues.advv));

    ui->input_wind_fc->setText(QString::number(formvalues.fc));
    ui->input_wind_ustar->setText(QString::number(formvalues.ustar));

    ui->input_surfacelayer_z0h->setText(QString::number(formvalues.z0h));
    ui->input_surfacelayer_z0m->setText(QString::number(formvalues.z0m));

    // SCALAR / CO2
    ui->input_sca_sca->setText(QString::number(formvalues.sca));
    ui->input_sca_dsca->setText(QString::number(formvalues.dsca));
    ui->input_sca_gammasca->setText(QString::number(formvalues.gammasca));
    ui->input_sca_advsca->setText(QString::number(formvalues.advsca));
    ui->input_sca_wsca->setText(QString::number(formvalues.wsca));

    ui->input_CO2_CO2->setText(QString::number(formvalues.CO2));
    ui->input_CO2_dCO2->setText(QString::number(formvalues.dCO2));
    ui->input_CO2_gammaCO2->setText(QString::number(formvalues.gammaCO2));
    ui->input_CO2_CO2adv->setText(QString::number(formvalues.advCO2));
    ui->input_CO2_wCO2->setText(QString::number(formvalues.wCO2 / (28.9/(1.2*46.))));  // Conversion ppm to mgCO2 m-2 s-1

    // SOIL
    ui->input_soil_T2->setText(QString::number(formvalues.T2));
    ui->input_soil_Tsoil->setText(QString::number(formvalues.Tsoil));
    ui->input_soil_W2->setText(QString::number(formvalues.w2));
    ui->input_soil_Wg->setText(QString::number(formvalues.wg));

    ui->input_soil_wsat->setText(QString::number(formvalues.wsat));
    ui->input_soil_wfc->setText(QString::number(formvalues.wfc));
    ui->input_soil_wwilt->setText(QString::number(formvalues.wwilt));
    ui->input_soil_c1sat->setText(QString::number(formvalues.C1sat));
    ui->input_soil_c2ref->setText(QString::number(formvalues.C2ref));
    ui->input_soil_a->setText(QString::number(formvalues.a));
    ui->input_soil_b->setText(QString::number(formvalues.b));
    ui->input_soil_p->setText(QString::number(formvalues.p));
    ui->input_soil_CGsat->setText(QString::number(formvalues.CGsat));

    if(!modelrunlist->find(n).value().soiladvanced)
    {
      updateSoiltype(modelrunlist->find(n).value().soilstatus);
      ui->sw_soil_advanced->setCheckState(Qt::Unchecked);
      ui->soil_advanced_group->setEnabled(false);
      ui->input_soil_soiltypes->setEnabled(true);
    }
    else
    {
      ui->sw_soil_advanced->setCheckState(Qt::Checked);
      ui->soil_advanced_group->setEnabled(true);
      ui->input_soil_soiltypes->setEnabled(false);
    }

    // SURFACE
    ui->input_surface_Ts->setText(QString::number(formvalues.Ts));
    ui->input_surface_Wl->setText(QString::number(formvalues.Wl));

    ui->input_surface_LAI->setText(QString::number(formvalues.LAI));
    ui->input_surface_gD->setText(QString::number(formvalues.gD));
    ui->input_surface_rsmin->setText(QString::number(formvalues.rsmin));
    ui->input_surface_alpha->setText(QString::number(formvalues.alpha));
    ui->input_surface_cveg->setText(QString::number(formvalues.cveg));
    ui->input_surface_Lambda->setText(QString::number(formvalues.Lambda));

    if(!modelrunlist->find(n).value().surfaceadvanced)
    {
      ui->sw_surface_advanced->setCheckState(Qt::Unchecked);
      ui->surface_advanced_group->setEnabled(false);
      ui->input_surface_surfacetypes->setEnabled(true);
    }
    else
    {
      ui->sw_surface_advanced->setCheckState(Qt::Checked);
      ui->surface_advanced_group->setEnabled(true);
      ui->input_surface_surfacetypes->setEnabled(false);
    }

    // RADIATION
    ui->input_rad_DOY->setText(QString::number(formvalues.doy));
    ui->input_rad_lat->setText(QString::number(formvalues.lat));
    ui->input_rad_lon->setText(QString::number(formvalues.lon * -1.));
    ui->input_rad_time->setText(QString::number(formvalues.tstart));

    ui->input_rad_Qnet->setText(QString::number(formvalues.Q));
    ui->input_rad_clouds->setText(QString::number(formvalues.cc));

    ui->input_rad_dFz->setText(QString::number(formvalues.dFz));

    // OTHER
    ui->input_name->setText(modelrunlist->find(n).value().runname);

    // CHEMISTRY
    ui->input_species_photolysis_tref->setText(QString::number(formvalues.tod_ref));
    ui->input_species_ref_Tcbl->setText(QString::number(formvalues.Tcbl_ref));
    ui->input_species_ref_Tft->setText(QString::number(formvalues.Tfc_ref));
    ui->input_species_ref_qcbl->setText(QString::number(formvalues.qcbl_ref * 1000.));
    ui->input_species_ref_qft->setText(QString::number(formvalues.qfc_ref * 1000.));
    ui->input_species_ref_pref->setText(QString::number(formvalues.P_ref / 100.));


    ui->input_reactions_OHrecycling->setValue(formvalues.stocoef);

    if (ui->species_treewidget->selectedItems().count() != 0)
    {
      int id = ui->species_treewidget->currentItem()->text(0).toInt();
      ui->input_species_scalar->setText(QString::number(formvalues.sc[id]));
      ui->input_species_dscalar->setText(QString::number(formvalues.dsc[id]));
      ui->input_species_gammascalar->setText(QString::number(formvalues.gammasc[id]));
      ui->input_species_wscalar->setText(QString::number(formvalues.wsc[id]));
      ui->input_species_advscalar->setText(QString::number(formvalues.advsc[id]));
      ui->selector_species_diurnal->setCurrentIndex(formvalues.sw_wsc[id]);
    }

    updateStatusBar();
  }
  else
    activerun = -1;
}

void MainWindow::updateStatusBar()
{
  QString statusmessage =
    "mixed-layer "   + bool2string(CheckState2bool(ui->sw_ml->checkState()))   + " | " +
    "wind "          + bool2string(CheckState2bool(ui->sw_wind->checkState())) + " | " +
    "surface-layer " + bool2string(CheckState2bool(ui->sw_sl->checkState()))   + " | " +
    "surface "       + bool2string(CheckState2bool(ui->sw_ls->checkState()))   + " | " +
    "radiation "     + bool2string(CheckState2bool(ui->sw_rad->checkState()))  + " | " +
    "sCU "           + bool2string(CheckState2bool(ui->sw_cu->checkState()))   + " | " +
    "chemistry "     + bool2string(CheckState2bool(ui->sw_chem->checkState()));
  ui->statusbar->showMessage(statusmessage);
}

QString MainWindow::bool2string(const bool in)
{
  if(in)
    return "ON";
  else
    return "OFF";
}

bool MainWindow::CheckState2bool(Qt::CheckState state)
{
  if(state == Qt::Unchecked)
    return false;
  else
    return true;
}

Qt::CheckState MainWindow::Bool2CheckState(bool state)
{
  if(state == true)
    return Qt::Checked;
  else
    return Qt::Unchecked;
}

int MainWindow::Bool2Int(bool state)
{
  if(state == true)
    return 2;
  else
    return 0;
}

void MainWindow::deleteRun()
{
  blockInput(true);
  if(ui->modelRunTree->selectedItems().count() > 0)
  {
    for (int i=0; i<ui->modelRunTree->selectedItems().count(); i++)
    {

      QString ident = ui->modelRunTree->selectedItems()[i]->text(0);
      int n = ident.toInt(0,10);

      // CvH This is an extremely ugly hack to solve the memory leak that occurs during run deletion
      // What needs to be done is managing the modelrun pointers accurately when inserting modelruns into
      // a list. Now, the memory that is going to be leaked is reduced to a minimum size by rerunning the
      // model with 1 timestep.
      if(modelrunlist->find(n).value().hasrun)
        if(modelrunlist->find(n).value().run->hasoutput)
        {
          modelrunlist->find(n).value().run->output->reset(22);
          modelrunlist->find(n).value().run->output->reload(1,22);
        }
      // end of ugly hack

      modelrunlist->remove(n);
      emit rundeleted(n);
    }

    qDeleteAll(ui->modelRunTree->selectedItems());
    runTreeChanged();
    ui->modelRunTree->setCurrentItem(ui->modelRunTree->topLevelItem(0));

    if(ui->modelRunTree->topLevelItemCount() > 0)
      activerun = ui->modelRunTree->currentItem()->text(0).toInt();
  }
  //updateSelectedRuns();
  //runTreeChanged();
  blockInput(false);
}

void MainWindow::updateRunName(QString dummy)
{
  blockInput(true);
  if(ui->modelRunTree->selectedItems().count() > 0)
    ui->modelRunTree->currentItem()->setText(1,ui->input_name->text());

  //storeFormData();

  if (plotwindowList.size() > 0)
  {
    int id = ui->modelRunTree->currentItem()->text(0).toInt();
    modelrunlist->find(id).value().runname = ui->input_name->text();

    for (int i = 0; i < plotwindowList.size(); i++)
    {
      plotwindowList.value(i)->updateselectedruns();
      if(modelrunlist->find(id).value().hasrun)
        plotwindowList.value(i)->addrun(id);
    }
  }

  //if (numgraphs > 0)
  //{
  //  graph->updateselectedruns();
  //  int id = ui->modelRunTree->currentItem()->text(0).toInt();
  //  modelrunlist->find(id).value().runname = ui->input_name->text();
  //  if(modelrunlist->find(id).value().hasrun)
  //    graph->addrun(id);
  //}

  blockInput(false);
}

void MainWindow::startrun()
{
  if(ui->modelRunTree->selectedItems().count() > 0)
  {
    for (int i=0; i<ui->modelRunTree->selectedItems().count(); i++)
    {
      storeFormData();
      int id = ui->modelRunTree->selectedItems()[i]->text(0).toInt();
      modelrunlist->find(id).value().previnput = modelrunlist->find(id).value().run->input;      // Why commented out?
      modelrunlist->find(id).value().run->runmodel();
      modelrunlist->find(id).value().hasrun = true;

      QFont font;
      font = ui->modelRunTree->selectedItems()[i]->font(1);
      font.setItalic(false);
      ui->modelRunTree->selectedItems()[i]->setFont(1,font);
      QBrush brush(Qt::lightGray);
      ui->modelRunTree->selectedItems()[i]->setForeground(0, brush);
      updateSelectedRuns();
      emit runadded(id);
    }
  }
}

void MainWindow::canceledit()
{
  int id = ui->modelRunTree->currentItem()->text(0).toInt();
  modelrunlist->find(id).value().run->input = modelrunlist->find(id).value().previnput;
  loadFormData();
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
  plotwindowList.prepend(graph);
  connect(this, SIGNAL(rundeleted(int)), graph, SLOT(deleterun(int)));
  connect(this, SIGNAL(runadded(int)), graph, SLOT(addrun(int)));
  connect(graph, SIGNAL(graphclosed(plotwindow*)), this, SLOT(graphClosed(plotwindow*)));
}

void MainWindow::graphClosed(plotwindow* plot){
  plotwindowList.removeAt(plotwindowList.indexOf(plot));
}

void MainWindow::exportRuns()
{
  storeFormData();

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
      int j = 0;
      while (QFile::exists(testfile))
      {
        runname  = runname0 + "_" + istr.setNum(j);
        testfile = dirname + "/" + runname + ".csv";
        j++;
      }

      int n = ident.toInt(0,10);
      if (modelrunlist->find(n).value().hasrun)
        modelrunlist->find(n).value().run->run2file(dirname.toStdString(),runname.toStdString());
    }
  }
}

void MainWindow::saveRuns()
{
  if(modelrunlist->size() == 0)
  {
    QMessageBox msgBox;
    msgBox.setWindowTitle("CLASS - Save session");
    msgBox.setText("There are no runs to save!");
    msgBox.exec();
  }
  else
  {
    storeFormData();

    QString casename_in = casename; // Save case name, in case user aborts the save
    QString filename = QFileDialog::getSaveFileName(this, "CLASS - Save session", casename);

    // Get file name from path, update current casename, set window title
    QStringList splitname = filename.split("/");
    casename = splitname.value(splitname.length()-1);  // BvS: WILL FAIL FOR WINDOWS
    this->setWindowTitle("CLASS main | " + casename);

    QFile file(filename);
    if(!file.open(QFile::WriteOnly | QFile::Truncate))
    {
      std::cout << "Error! Cannot open file! Abort saving..." << std::endl;
      casename = casename_in; // Restore case name
      this->setWindowTitle("CLASS main | " + casename_in); // set window title
      return;
    }
    QTextStream out(&file);   // we will serialize the data into the file

    QMap<int,modelrun>::iterator i;
    modelrun temprun(&defaultinput);

    for (i = modelrunlist->begin(); i != modelrunlist->end(); ++i)
    {
      std::cout << "Saving MXL run..." << std::endl;
      temprun = i.value();
      // NON-INPUT CLASS ELEMENTS
      out << "#MXL# NEWRUN"                << Qt::endl;
      out << temprun.runname               << Qt::endl;
      out << temprun.surfacestatus         << Qt::endl;
      out << temprun.soilstatus            << Qt::endl;

      out << temprun.surfaceadvanced       << Qt::endl;
      out << temprun.soiladvanced          << Qt::endl;

      // TAB 1
      out << temprun.run->input.dt         << Qt::endl;
      out << temprun.run->input.runtime    << Qt::endl;
      out << temprun.run->input.sinperiod  << Qt::endl;

      // MIXED-LAYER
      out << temprun.run->input.sw_ml      << Qt::endl;
      out << temprun.run->input.sw_ftcws   << Qt::endl;
      out << temprun.run->input.sw_shearwe << Qt::endl;
      out << temprun.run->input.h          << Qt::endl;
      out << temprun.run->input.Ps         << Qt::endl;
      out << temprun.run->input.omegas     << Qt::endl;
      out << temprun.run->input.beta       << Qt::endl;

      // HEAT
      out << temprun.run->input.theta      << Qt::endl;
      out << temprun.run->input.dtheta     << Qt::endl;
      out << temprun.run->input.gammatheta << Qt::endl;
      out << temprun.run->input.advtheta   << Qt::endl;
      out << temprun.run->input.wtheta     << Qt::endl;
      out << temprun.run->input.sw_wtheta  << Qt::endl;

      // MOISTURE
      out << temprun.run->input.q          << Qt::endl;
      out << temprun.run->input.dq         << Qt::endl;
      out << temprun.run->input.gammaq     << Qt::endl;
      out << temprun.run->input.advq       << Qt::endl;
      out << temprun.run->input.wq         << Qt::endl;
      out << temprun.run->input.sw_wq      << Qt::endl;
      // END TAB1

      // TAB2
      // WIND
      out << temprun.run->input.sw_wind    << Qt::endl;
      out << temprun.run->input.u          << Qt::endl;
      out << temprun.run->input.du         << Qt::endl;
      out << temprun.run->input.gammau     << Qt::endl;
      out << temprun.run->input.advu       << Qt::endl;
      out << temprun.run->input.v          << Qt::endl;
      out << temprun.run->input.dv         << Qt::endl;
      out << temprun.run->input.gammav     << Qt::endl;
      out << temprun.run->input.advv       << Qt::endl;

      out << temprun.run->input.ustar      << Qt::endl;
      out << temprun.run->input.fc         << Qt::endl;

      out << temprun.run->input.sw_sl      << Qt::endl;
      out << temprun.run->input.z0m        << Qt::endl;
      out << temprun.run->input.z0h        << Qt::endl;

      // SCALAR / CO2
      out << temprun.run->input.sca        << Qt::endl;
      out << temprun.run->input.dsca       << Qt::endl;
      out << temprun.run->input.gammasca   << Qt::endl;
      out << temprun.run->input.advsca     << Qt::endl;
      out << temprun.run->input.wsca       << Qt::endl;

      out << temprun.run->input.CO2        << Qt::endl;
      out << temprun.run->input.dCO2       << Qt::endl;
      out << temprun.run->input.gammaCO2   << Qt::endl;
      out << temprun.run->input.advCO2     << Qt::endl;
      out << temprun.run->input.wCO2       << Qt::endl;
      // END TAB2

      // TAB3
      out << temprun.run->input.sw_ls      << Qt::endl;
      out << temprun.run->input.sw_sea     << Qt::endl;
      out << temprun.run->input.sw_jarvis  << Qt::endl;

      // SURFACE
      out << temprun.run->input.Ts         << Qt::endl;
      out << temprun.run->input.Wl         << Qt::endl;
      out << temprun.run->input.LAI        << Qt::endl;
      out << temprun.run->input.gD         << Qt::endl;
      out << temprun.run->input.rsmin      << Qt::endl;
      out << temprun.run->input.alpha      << Qt::endl;
      out << temprun.run->input.cveg       << Qt::endl;
      out << temprun.run->input.Lambda     << Qt::endl;
      out << temprun.run->input.z0m        << Qt::endl;
      out << temprun.run->input.z0h        << Qt::endl;
      // END TAB3

      // TAB4
      // SOIL
      out << temprun.run->input.T2         << Qt::endl;
      out << temprun.run->input.Tsoil      << Qt::endl;
      out << temprun.run->input.w2         << Qt::endl;
      out << temprun.run->input.wg         << Qt::endl;
      out << temprun.run->input.wsat       << Qt::endl;
      out << temprun.run->input.wfc        << Qt::endl;
      out << temprun.run->input.wwilt      << Qt::endl;
      out << temprun.run->input.T2         << Qt::endl;
      out << temprun.run->input.C1sat      << Qt::endl;
      out << temprun.run->input.C2ref      << Qt::endl;
      out << temprun.run->input.a          << Qt::endl;
      out << temprun.run->input.b          << Qt::endl;
      out << temprun.run->input.p          << Qt::endl;
      out << temprun.run->input.CGsat      << Qt::endl;
      // END TAB4

      // TAB5
      // RADIATION
      out << temprun.run->input.sw_rad     << Qt::endl;
      out << temprun.run->input.doy        << Qt::endl;
      out << temprun.run->input.lat        << Qt::endl;
      out << temprun.run->input.lon        << Qt::endl;
      out << temprun.run->input.tstart     << Qt::endl;

      out << temprun.run->input.Q          << Qt::endl;
      out << temprun.run->input.cc         << Qt::endl;

      out << temprun.run->input.sw_cu      << Qt::endl;
      out << temprun.run->input.sw_curad   << Qt::endl;
      out << temprun.run->input.dFz        << Qt::endl;
      // END TAB5

      // TAB 6 and 7
      // CHEMISTRY
      out << temprun.run->input.sw_chem           << Qt::endl;
      out << temprun.run->input.sw_chem_constant  << Qt::endl;
      out << temprun.run->input.sw_photo_constant << Qt::endl;
      out << temprun.run->input.csize             << Qt::endl;
      out << temprun.run->input.rsize             << Qt::endl;

      // store scalars
      for(int n=0; n<temprun.run->input.csize; n++)
      {
        out << temprun.run->input.sc[n]      << Qt::endl;
        out << temprun.run->input.dsc[n]     << Qt::endl;
        out << temprun.run->input.gammasc[n] << Qt::endl;
        out << temprun.run->input.advsc[n]   << Qt::endl;
        out << temprun.run->input.wsc[n]     << Qt::endl;
        out << temprun.run->input.sw_wsc[n]  << Qt::endl;
      }

      for(int n=0; n<temprun.run->input.rsize; n++)
        out << temprun.run->input.sw_reactions[n] << Qt::endl;

      out << temprun.run->input.P_ref             << Qt::endl;
      out << temprun.run->input.Tcbl_ref          << Qt::endl;
      out << temprun.run->input.Tfc_ref           << Qt::endl;
      out << temprun.run->input.qcbl_ref          << Qt::endl;
      out << temprun.run->input.qfc_ref           << Qt::endl;
      out << temprun.run->input.tod_ref           << Qt::endl;

      out << temprun.run->input.stocoef           << Qt::endl;
    }
    out << "#MXL# EOF" << Qt::endl;
    std::cout << "Saving MXL session COMPLETE!" << std::endl;

    file.close();
  }
}

void MainWindow::loadRuns()
{
  storeFormData();

  QString filename = QFileDialog::getOpenFileName(this, "Load session file", "~");
  QStringList splitname = filename.split("/");

  // BvS: WILL FAIL FOR WINDOWS
  casename = splitname.value(splitname.length()-1);
  this->setWindowTitle("CLASS main | " + casename);

  QFile file(filename);
  file.open(QFile::ReadOnly);
  QTextStream in(&file);   // we will serialize the data into the file

  QString line;
  modelrun temprun(&defaultinput);
  modelinput tempinput;
  tempinput = defaultinput;

  line = in.readLine();
  if(line != "#MXL# NEWRUN")
    std::cout << "Error! Not a MXL session file" << std::endl;
  else
  {
    while(line == "#MXL# NEWRUN")
    {
      temprun.run = new model(&defaultinput);

      std::cout << "Loading MXL run..." << std::endl;
      line = in.readLine();
      temprun.runname               = line;

      line = in.readLine();
      temprun.surfacestatus         = line.toInt();
      line = in.readLine();
      temprun.soilstatus            = line.toInt();

      line = in.readLine();
      temprun.surfaceadvanced       = line.toInt();
      line = in.readLine();
      temprun.soiladvanced          = line.toInt();

      //  // TAB 1
      line = in.readLine();
      tempinput.dt         = line.toDouble();
      line = in.readLine();
      tempinput.runtime    = line.toDouble();
      line = in.readLine();
      tempinput.sinperiod  = line.toDouble();

      //  // MIXED-LAYER
      line = in.readLine();
      tempinput.sw_ml      = line.toInt();
      line = in.readLine();
      tempinput.sw_ftcws   = line.toInt();
      line = in.readLine();
      tempinput.sw_shearwe = line.toInt();
      line = in.readLine();
      tempinput.h          = line.toDouble();
      line = in.readLine();
      tempinput.Ps         = line.toDouble();
      line = in.readLine();
      tempinput.omegas     = line.toDouble();
      line = in.readLine();
      tempinput.beta       = line.toDouble();

      //  // HEAT
      line = in.readLine();
      tempinput.theta      = line.toDouble();
      line = in.readLine();
      tempinput.dtheta     = line.toDouble();
      line = in.readLine();
      tempinput.gammatheta = line.toDouble();
      line = in.readLine();
      tempinput.advtheta   = line.toDouble();
      line = in.readLine();
      tempinput.wtheta     = line.toDouble();
      line = in.readLine();
      tempinput.sw_wtheta  = line.toInt();

      //  // MOISTURE
      line = in.readLine();
      tempinput.q          = line.toDouble();
      line = in.readLine();
      tempinput.dq         = line.toDouble();
      line = in.readLine();
      tempinput.gammaq     = line.toDouble();
      line = in.readLine();
      tempinput.advq       = line.toDouble();
      line = in.readLine();
      tempinput.wq         = line.toDouble();
      line = in.readLine();
      tempinput.sw_wq      = line.toInt();
      // END TAB1

      // TAB2
      // WIND
      line = in.readLine();
      tempinput.sw_wind    = line.toInt();
      line = in.readLine();
      tempinput.u          = line.toDouble();
      line = in.readLine();
      tempinput.du         = line.toDouble();
      line = in.readLine();
      tempinput.gammau     = line.toDouble();
      line = in.readLine();
      tempinput.advu       = line.toDouble();
      line = in.readLine();
      tempinput.v          = line.toDouble();
      line = in.readLine();
      tempinput.dv         = line.toDouble();
      line = in.readLine();
      tempinput.gammav     = line.toDouble();
      line = in.readLine();
      tempinput.advv       = line.toDouble();

      line = in.readLine();
      tempinput.ustar      = line.toDouble();
      line = in.readLine();
      tempinput.fc         = line.toDouble();

      line = in.readLine();
      tempinput.sw_sl      = line.toInt();
      line = in.readLine();
      tempinput.z0m        = line.toDouble();
      line = in.readLine();
      tempinput.z0h        = line.toDouble();

      // SCALAR / CO2
      line = in.readLine();
      tempinput.sca        = line.toDouble();
      line = in.readLine();
      tempinput.dsca       = line.toDouble();
      line = in.readLine();
      tempinput.gammasca   = line.toDouble();
      line = in.readLine();
      tempinput.advsca     = line.toDouble();
      line = in.readLine();
      tempinput.wsca       = line.toDouble();

      line = in.readLine();
      tempinput.CO2        = line.toDouble();
      line = in.readLine();
      tempinput.dCO2       = line.toDouble();
      line = in.readLine();
      tempinput.gammaCO2   = line.toDouble();
      line = in.readLine();
      tempinput.advCO2     = line.toDouble();
      line = in.readLine();
      tempinput.wCO2       = line.toDouble();
      // END TAB2

      // TAB3
      line = in.readLine();
      tempinput.sw_ls      = line.toInt();
      line = in.readLine();
      tempinput.sw_sea     = line.toInt();
      line = in.readLine();
      tempinput.sw_jarvis  = line.toInt();

      // SURFACE
      line = in.readLine();
      tempinput.Ts         = line.toDouble();
      line = in.readLine();
      tempinput.Wl         = line.toDouble();
      line = in.readLine();
      tempinput.LAI        = line.toDouble();
      line = in.readLine();
      tempinput.gD         = line.toDouble();
      line = in.readLine();
      tempinput.rsmin      = line.toDouble();
      line = in.readLine();
      tempinput.alpha      = line.toDouble();
      line = in.readLine();
      tempinput.cveg       = line.toDouble();
      line = in.readLine();
      tempinput.Lambda     = line.toDouble();
      line = in.readLine();
      tempinput.z0m        = line.toDouble();
      line = in.readLine();
      tempinput.z0h        = line.toDouble();
      // END TAB3

      // TAB4
      // SOIL
      line = in.readLine();
      tempinput.T2         = line.toDouble();
      line = in.readLine();
      tempinput.Tsoil      = line.toDouble();
      line = in.readLine();
      tempinput.w2         = line.toDouble();
      line = in.readLine();
      tempinput.wg         = line.toDouble();
      line = in.readLine();
      tempinput.wsat       = line.toDouble();
      line = in.readLine();
      tempinput.wfc        = line.toDouble();
      line = in.readLine();
      tempinput.wwilt      = line.toDouble();
      line = in.readLine();
      tempinput.T2         = line.toDouble();
      line = in.readLine();
      tempinput.C1sat      = line.toDouble();
      line = in.readLine();
      tempinput.C2ref      = line.toDouble();
      line = in.readLine();
      tempinput.a          = line.toDouble();
      line = in.readLine();
      tempinput.b          = line.toDouble();
      line = in.readLine();
      tempinput.p          = line.toDouble();
      line = in.readLine();
      tempinput.CGsat      = line.toDouble();
      // END TAB4

      // TAB5
      // RADIATION
      line = in.readLine();
      tempinput.sw_rad     = line.toInt();
      line = in.readLine();
      tempinput.doy        = line.toDouble();
      line = in.readLine();
      tempinput.lat        = line.toDouble();
      line = in.readLine();
      tempinput.lon        = line.toDouble();
      line = in.readLine();
      tempinput.tstart     = line.toDouble();

      line = in.readLine();
      tempinput.Q          = line.toDouble();
      line = in.readLine();
      tempinput.cc         = line.toDouble();

      line = in.readLine();
      tempinput.sw_cu      = line.toInt();
      line = in.readLine();
      tempinput.sw_curad   = line.toInt();
      line = in.readLine();
      tempinput.dFz        = line.toDouble();

      // END TAB5

      // CHEMISTRY
      line = in.readLine();
      tempinput.sw_chem           = line.toInt();
      line = in.readLine();
      tempinput.sw_chem_constant  = line.toInt();
      line = in.readLine();
      tempinput.sw_photo_constant = line.toInt();
      line = in.readLine();
      tempinput.csize             = line.toInt();
      line = in.readLine();
      tempinput.rsize             = line.toInt();

      // store scalars
      for(int n=0; n<tempinput.csize; n++)
      {
        line = in.readLine();
        tempinput.sc[n]      = line.toDouble();
        line = in.readLine();
        tempinput.dsc[n]     = line.toDouble();
        line = in.readLine();
        tempinput.gammasc[n] = line.toDouble();
        line = in.readLine();
        tempinput.advsc[n]   = line.toDouble();
        line = in.readLine();
        tempinput.wsc[n]     = line.toDouble();
        line = in.readLine();
        tempinput.sw_wsc[n]  = line.toInt();
      }

      for(int n=0; n<tempinput.rsize; n++)
      {
        line = in.readLine();
        tempinput.sw_reactions[n] = line.toInt();
      }

      line = in.readLine();
      tempinput.P_ref       = line.toDouble();      
      line = in.readLine();
      tempinput.Tcbl_ref    = line.toDouble();  
      line = in.readLine();
      tempinput.Tfc_ref     = line.toDouble();
      line = in.readLine();
      tempinput.qcbl_ref    = line.toDouble();
      line = in.readLine();
      tempinput.qfc_ref     = line.toDouble();
      line = in.readLine();
      tempinput.tod_ref     = line.toDouble();

      line = in.readLine();
      tempinput.stocoef     = line.toDouble();

      line = in.readLine();

      if(line == "#MXL# EOF" || line == "#MXL# NEWRUN")
      {
        std::cout << "MXL Run correctly imported!" << std::endl;

        // Add temp run to runtree
        blockInput(true);
        temprun.hasrun = false;

        QMap<int, modelrun>::iterator i = modelrunlist->begin();
        int max=0;
        while (i != modelrunlist->end())
        {
          if (i.key() > max)
            max = i.key();
          ++i;
        }

        modelrunlist->insert((max+1),temprun);
        modelrunlist->find(max+1).value().run->input = tempinput;
        modelrunlist->find(max+1).value().previnput  = tempinput;

        QTreeWidgetItem *point = new QTreeWidgetItem(ui->modelRunTree);
        point->setText(0, QString::number(max+1));
        point->setText(1, temprun.runname);

        QFont font;
        font = point->font(1);
        font.setItalic(true);
        point->setFont(1,font);
        QBrush brush(Qt::red);
        point->setForeground(0, brush);

        ui->modelRunTree->setCurrentItem(point);

        runTreeChanged();
        blockInput(false);
      }
      else
      {
        std::cout << "MXL run corrupted" << std::endl;
        break;
      }
    }
  }
  file.close();
}

void MainWindow::resetInterface()
{
  QMessageBox msgBox;
  msgBox.setWindowTitle("CLASS - Reset session?");
  msgBox.setText("Are you sure you want to reset the interface?");
  msgBox.setInformativeText("This will delete all of your current experiments");

  msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
  msgBox.setDefaultButton(QMessageBox::Cancel);
  int ret = msgBox.exec();

  switch (ret)
  {
    case QMessageBox::Yes:

      blockInput(true);

      for (int i=0; i<ui->modelRunTree->topLevelItemCount(); i++)
      {
        QString ident = ui->modelRunTree->topLevelItem(i)->text(0);
        int n = ident.toInt(0,10);

        // CvH This is an extremely ugly hack to solve the memory leak that occurs during run deletion
        if(modelrunlist->find(n).value().hasrun)
          if(modelrunlist->find(n).value().run->hasoutput)
          {
          modelrunlist->find(n).value().run->output->reset(22);
          modelrunlist->find(n).value().run->output->reload(1,22);
          }
       // end of ugly hack

       modelrunlist->remove(n);
       emit rundeleted(n);
      }

      ui->modelRunTree->clear();
      runTreeChanged();

      newrun();
      casename = defaultname;
      this->setWindowTitle("CLASS main | " + casename);

      blockInput(false);

      break;
    default:
      break;
    }
}

void MainWindow::setLandSoil(int i)
{
  // Check if/which surface and soil type is selected to restore settings
  // Index -1 == no selection.
  int selectedSurfaceType = ui->input_surface_surfacetypes->currentIndex();
  int selectedSoilType = ui->input_soil_soiltypes->currentIndex();

  // Clear list
  ui->input_surface_surfacetypes->clear();
  ui->input_soil_soiltypes->clear();

  // Read surface types into pull down menu
  if(i == 0) // Jarvis-stewart vegetation types
  {
    for (int j = 0;j<4;j++)
      ui->input_surface_surfacetypes->addItem(surfacetypes[j].name, j);
    if(selectedSurfaceType != -1)
    {
      ui->input_surface_surfacetypes->setCurrentIndex(selectedSurfaceType);
      ui->input_surface_surfacetypes->show();
    }
  }
  else // A-Gs vegetation types:
  {
    for (int j = 0;j<2;j++)
      ui->input_surface_surfacetypes->addItem(surfacetypes[j].name, j);
    if(selectedSurfaceType != -1 && selectedSurfaceType < 2) // Prevent selecting non-A-Gs vegetations..
    {
      ui->input_surface_surfacetypes->setCurrentIndex(selectedSurfaceType);
      ui->input_surface_surfacetypes->show();
    }
  }

  // Read soil types into pull down menu
  for(int i=0;i<3;i++)
    ui->input_soil_soiltypes->addItem(soiltypes[i].name, i);
  if(selectedSoilType != -1)
  {
    ui->input_soil_soiltypes->setCurrentIndex(selectedSoilType);
    ui->input_surface_surfacetypes->show();
  }
}

void MainWindow::updateSurfacetype(int i)
{
  if(ui->modelRunTree->selectedItems().count() == 0)
    return;

  // int id = ui->modelRunTree->currentItem()->text(0).toInt();
  // if(modelrunlist->find(id).value().surfaceadvanced)
  //  return;

  // BvS, Uglyest hack ever, have to change this...
  //if(ui->sw_jarvis->currentIndex() == 1 && ui->input_surface_surfacetypes->currentIndex() == 1)
  //  i = 3;

  ui->input_surface_LAI->setText(QString::number(surfacetypes[i].LAI));
  ui->input_surface_gD->setText(QString::number(surfacetypes[i].gD));
  ui->input_surface_rsmin->setText(QString::number(surfacetypes[i].rsmin));
  ui->input_surface_alpha->setText(QString::number(surfacetypes[i].alpha));

  ui->input_surface_cveg->setText(QString::number(surfacetypes[i].cveg));
  ui->input_surface_Lambda->setText(QString::number(surfacetypes[i].Lambda));
  ui->input_surfacelayer_z0m->setText(QString::number(surfacetypes[i].z0m));
  ui->input_surfacelayer_z0h->setText(QString::number(surfacetypes[i].z0h));

//  QMap<int,modelrun>::iterator n = modelrunlist->find(id);
//  while(n != modelrunlist->end())
//  {
//    modelrunlist->find(id).value().surfacestatus   = ui->input_surface_surfacetypes->currentIndex();
//
//    modelinput *tempinput = &modelrunlist->find(id).value().run->input;
//
//    // SURFACE
//    tempinput->LAI        = ui->input_surface_LAI->text().toDouble();
//    tempinput->gD         = ui->input_surface_gD->text().toDouble();
//    tempinput->rsmin      = ui->input_surface_rsmin->text().toDouble();
//    tempinput->alpha      = ui->input_surface_alpha->text().toDouble();
//    tempinput->cveg       = ui->input_surface_cveg->text().toDouble();
//    tempinput->Lambda     = ui->input_surface_Lambda->text().toDouble();
//    tempinput->z0m        = ui->input_surface_z0m->text().toDouble();
//    tempinput->z0h        = ui->input_surface_z0h->text().toDouble();
//
//    n++;
//  }
}

void MainWindow::updateSoiltype(int i)
{
  if(ui->modelRunTree->selectedItems().count() == 0)
    return;

  // int id = ui->modelRunTree->currentItem()->text(0).toInt();

  // if(modelrunlist->find(id).value().soiladvanced)
  //  return;

  ui->input_soil_wsat->setText(QString::number(soiltypes[i].wsat));
  ui->input_soil_wfc->setText(QString::number(soiltypes[i].wfc));
  ui->input_soil_wwilt->setText(QString::number(soiltypes[i].wwilt));
  ui->input_soil_c1sat->setText(QString::number(soiltypes[i].C1sat));
  ui->input_soil_c2ref->setText(QString::number(soiltypes[i].C2ref));

  ui->input_soil_a->setText(QString::number(soiltypes[i].a));
  ui->input_soil_b->setText(QString::number(soiltypes[i].b));
  ui->input_soil_p->setText(QString::number(soiltypes[i].p));
  ui->input_soil_CGsat->setText(QString::number(soiltypes[i].CGsat));

//  QMap<int,modelrun>::iterator n = modelrunlist->find(id);
//  while(n != modelrunlist->end())
//  {
//    modelrunlist->find(id).value().soilstatus   = ui->input_soil_soiltypes->currentIndex();
//
//    modelinput *tempinput = &modelrunlist->find(id).value().run->input;
//
//    tempinput->wsat       = ui->input_soil_wsat->text().toDouble();
//    tempinput->wfc        = ui->input_soil_wfc->text().toDouble();
//    tempinput->wwilt      = ui->input_soil_wwilt->text().toDouble();
//    tempinput->C1sat      = ui->input_soil_c1sat->text().toDouble();
//    tempinput->C2ref      = ui->input_soil_c2ref->text().toDouble();
//    tempinput->a          = ui->input_soil_a->text().toDouble();
//    tempinput->b          = ui->input_soil_b->text().toDouble();
//    tempinput->p          = ui->input_soil_p->text().toDouble();
//    tempinput->CGsat      = ui->input_soil_CGsat->text().toDouble();
//
//    n++;
//  }
}

// ----------------------------------
// Switches

void MainWindow::setNoReactions()
{
  setReactions(0);
}

void MainWindow::setSimpleReactions()
{
  setReactions(1);
}

void MainWindow::setComplexReactions()
{
  setReactions(2);
}

void MainWindow::setReactions(int mode)
{
  // mode 0 = no reactions
  // mode 1 = simple (only R5, R21)
  // mode 2 = complex (all reactions)

  bool active;

  if (mode == 0)
    active = false;
  else if (mode == 1)
    active = false;
  else if (mode == 2)
    active = true;

  ui->sw_R1->setChecked(active);
  ui->sw_R2->setChecked(active);
  ui->sw_R3->setChecked(active);
  ui->sw_R4->setChecked(active);
  if (mode != 1)
    ui->sw_R5->setChecked(active);
  else
    ui->sw_R5->setChecked(true);
  ui->sw_R6->setChecked(active);
  ui->sw_R7->setChecked(active);
  ui->sw_R8->setChecked(active);
  ui->sw_R9->setChecked(active);
  ui->sw_R10->setChecked(active);
  ui->sw_R11->setChecked(active);
  ui->sw_R12->setChecked(active);
  ui->sw_R13->setChecked(active);
  ui->sw_R14->setChecked(active);
  ui->sw_R15->setChecked(active);
  ui->sw_R16->setChecked(active);
  ui->sw_R17->setChecked(active);
  ui->sw_R18->setChecked(active);
  ui->sw_R19->setChecked(active);
  ui->sw_R20->setChecked(active);
  if (mode != 1)
    ui->sw_R21->setChecked(active);
  else
    ui->sw_R21->setChecked(true);
  ui->sw_R22->setChecked(active);
  ui->sw_R23->setChecked(active);
  ui->sw_R24->setChecked(active);
  ui->sw_R25->setChecked(active);
  ui->sw_R26->setChecked(active);
  ui->sw_R27->setChecked(active);
  ui->sw_R28->setChecked(active);
}


void MainWindow::switch_wind(int state)
{
  bool checkstate;
  if (state == Qt::Checked)
    checkstate = true;
  else
    checkstate = false;

  formvalues.sw_wind = checkstate;
  ui->wind_U_group->setEnabled(checkstate);
  ui->wind_V_group->setEnabled(checkstate);
  //ui->wind_general_group->setEnabled(checkstate);
  updateStatusBar();
}

void MainWindow::switch_ls(int state)
{
  bool checkstate;
  if (state == Qt::Checked)
    checkstate = true;
  else
    checkstate = false;

  ui->surface_properties_group->setEnabled(checkstate);
  ui->soil_properties_group->setEnabled(checkstate);
  ui->advanced_surface_switches_group->setEnabled(checkstate);

  if(checkstate == false)
  {
    ui->sw_surface_advanced->setCheckState(Qt::Unchecked);
    ui->sw_soil_advanced->setCheckState(Qt::Unchecked);
  }

  ui->input_surfacelayer_z0h->setToolTip("roughness length for scalars");

  updateStatusBar();
}

void MainWindow::switch_sea(int state)
{
  return;
}

void MainWindow::switch_sl(int state)
{
  bool checkstate;
  if (state == Qt::Checked)
    checkstate = true;
  else
    checkstate = false;

  ui->input_surfacelayer_z0h->setEnabled(checkstate);
  ui->input_surfacelayer_z0m->setEnabled(checkstate);
  ui->label_surfacelayer_z0h->setEnabled(checkstate);
  ui->label_surfacelayer_z0m->setEnabled(checkstate);
  ui->unitlabel_surfacelayer_z0h->setEnabled(checkstate);
  ui->unitlabel_surfacelayer_z0m->setEnabled(checkstate);

  updateStatusBar();
}

void MainWindow::switch_rad(int state)
{
  bool checkstate;
  bool checkstateclouds;
  if (state == Qt::Checked)
    checkstate = true;
  else
    checkstate = false;

  ui->label_rad_Qnet->setEnabled(!checkstate);
  ui->input_rad_Qnet->setEnabled(!checkstate);
  ui->unitlabel_rad_Qnet->setEnabled(!checkstate);

  // Field clouds depends both on state radiation and cumulus switches
  if(checkstate && ui->sw_cu->checkState() == Qt::Unchecked)
    checkstateclouds = true;
  else
    checkstateclouds = false;

  ui->label_rad_clouds->setEnabled(checkstateclouds);
  ui->input_rad_clouds->setEnabled(checkstateclouds);
  ui->unitlabel_rad_clouds->setEnabled(checkstateclouds);

  updateStatusBar();
}

void MainWindow::switch_ml(int state)
{
  bool checkstate;
  if (state == Qt::Checked)
    checkstate = true;
  else
    checkstate = false;

  updateStatusBar();
}

void MainWindow::switch_cu(int state)
{
  bool checkstate;
  bool checkstateclouds;
  if (state == Qt::Checked)
    checkstate = true;
  else
    checkstate = false;

  ui->sw_cu_rad->setEnabled(checkstate);

  // Field clouds depends both on state radiation and cumulus switches
  if(!checkstate && ui->sw_rad->checkState() == Qt::Checked)
    checkstateclouds = true;
  else
    checkstateclouds = false;

  ui->label_rad_clouds->setEnabled(checkstateclouds);
  ui->input_rad_clouds->setEnabled(checkstateclouds);
  ui->unitlabel_rad_clouds->setEnabled(checkstateclouds);


  updateStatusBar();
}

void MainWindow::switch_curad(int state)
{
  bool checkstate;
  if (state == Qt::Checked)
    checkstate = true;
  else
    checkstate = false;
}

void MainWindow::switch_wtheta(int state)
{
  bool checkstate;
  if (state == Qt::Checked)
    checkstate = true;
  else
    checkstate = false;
}

void MainWindow::switch_wq(int state)
{
  bool checkstate;
  if (state == Qt::Checked)
    checkstate = true;
  else
    checkstate = false;
}

void MainWindow::switch_surface_advanced(int state)
{
  bool checkstate;
  if (state == Qt::Checked)
    checkstate = true;
  else
    checkstate = false;

  ui->surface_advanced_group->setEnabled(checkstate);
  ui->input_surface_surfacetypes->setEnabled(!checkstate);

}

void MainWindow::switch_soil_advanced(int state)
{
  bool checkstate;
  if (state == Qt::Checked)
    checkstate = true;
  else
    checkstate = false;  

  ui->soil_advanced_group->setEnabled(checkstate);
  ui->input_soil_soiltypes->setEnabled(!checkstate);
}

void MainWindow::switch_chem(int state)
{
  bool checkstate;
  if (state == Qt::Checked)
    checkstate = true;
  else
    checkstate = false;

  ui->species_photo_group->setEnabled(checkstate);
  ui->species_group_ref->setEnabled(checkstate);
  ui->species_species_group->setEnabled(checkstate);
  ui->reactions_group->setEnabled(checkstate);
  ui->reactions_scrollarea->setEnabled(checkstate);

  updateStatusBar();
}

void MainWindow::switch_chem_constant(int state)
{
  bool checkstate;
  if (state == Qt::Checked)
    checkstate = true;
  else
    checkstate = false;

  ui->label_species_ref_Tcbl->setEnabled(checkstate);
  ui->input_species_ref_Tcbl->setEnabled(checkstate);
  ui->unitlabel_species_ref_Tcbl->setEnabled(checkstate);

  ui->label_species_ref_Tft->setEnabled(checkstate);
  ui->input_species_ref_Tft->setEnabled(checkstate);
  ui->unitlabel_species_ref_Tft->setEnabled(checkstate);

  ui->label_species_ref_qcbl->setEnabled(checkstate);
  ui->input_species_ref_qcbl->setEnabled(checkstate);
  ui->unitlabel_species_ref_qcbl->setEnabled(checkstate);

  ui->label_species_ref_qft->setEnabled(checkstate);
  ui->input_species_ref_qft->setEnabled(checkstate);
  ui->unitlabel_species_ref_qft->setEnabled(checkstate);

  ui->label_species_ref_pref->setEnabled(checkstate);
  ui->input_species_ref_pref->setEnabled(checkstate);
  ui->unitlabel_species_ref_pref->setEnabled(checkstate);
}


void MainWindow::switch_photolysis(int state)
{
  bool checkstate;
  if (state == Qt::Checked)
    checkstate = true;
  else
    checkstate = false;

  ui->input_species_photolysis_tref->setEnabled(checkstate);
}

void MainWindow::showAbout()
{
  QMessageBox msgBox;
  msgBox.setWindowTitle("CLASS - About");
  QSpacerItem* horizontalSpacer = new QSpacerItem(400, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
  msgBox.setText("About the CLASS model");
  QString informativeText = "<html><b>Chemistry Land-surface Atmosphere Soil Slab model</b><br/><br/><b>Meteorology and Air Quality section<br/>Wageningen University and Research centre</b><br/></br><br/><br/>Authors:<br/>Chiel van Heerwaarden<br/>Bart van Stratum<br/>Kees van den Dries<br/>Jordi Vil&#224;-Guerau de Arellano<br/><br/>Contact: jordi.vila@wur.nl<br/><br/>&copy; 2010-2014, GPLv3 licence<br/><br/>Source code available at https://github.com/classmodel/modelgui<br/><br/>git-hash: " + QString::fromStdString(GITHASH) + "</html>";
  msgBox.setInformativeText(informativeText);
  QGridLayout* layout = (QGridLayout*)msgBox.layout();
  layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
  msgBox.exec();
}

void MainWindow::closeWarning()
{
  QMessageBox msgBox;
  msgBox.setWindowTitle("CLASS - Save session?");
  msgBox.setText("Save settings?");
  msgBox.setInformativeText("There are possibly unsaved settings. Do you want to save them?");
  msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
  msgBox.setDefaultButton(QMessageBox::Save);
  int ret = msgBox.exec();

  switch (ret)
  {
    case QMessageBox::Save:
      saveRuns();
      closeCheck = true;     // allowed to close..
      break;
    case QMessageBox::Discard:
      closeCheck = true;     // chose to close..
      break;
    case QMessageBox::Cancel:
      closeCheck = false;
      break;
    default:
      // should never be reached
      break;
  }
}









