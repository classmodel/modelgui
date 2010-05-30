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

  connect(ui->switch_wind,    SIGNAL(stateChanged(int)),        this, SLOT(wind_switch(int)));
  connect(ui->newRunButton,   SIGNAL(clicked()),                this, SLOT(newrun()));
  connect(ui->cloneRunButton, SIGNAL(clicked()),                this, SLOT(clonerun()));
  connect(ui->modelRunTree,   SIGNAL(itemSelectionChanged()),   this, SLOT(runTreeChanged()));
  connect(ui->deleteButton,   SIGNAL(clicked()),                this, SLOT(deleteRun()));
  connect(ui->graphButton,    SIGNAL(clicked()),                this, SLOT(startGraph()));
  connect(ui->input_name,     SIGNAL(editingFinished()),        this, SLOT(updateRunName()));
  connect(ui->exportButton,   SIGNAL(clicked()),                this, SLOT(exportRuns()));

  loadfieldslots();

  // Setup QTreeWidget
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
  updateSelectedRuns();
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
