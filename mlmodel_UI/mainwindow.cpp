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
  modelrunlist = new QMap<int, modelrun>;
  updateRunList();
}

MainWindow::~MainWindow()
{
  delete ui;
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
