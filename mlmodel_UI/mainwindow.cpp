#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent), ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  connect(ui->switch_wind, SIGNAL(stateChanged(int)), this, SLOT(wind_switch(int)));
  ui->wind_U_group->setDisabled(true);              // wind disabled by default
  ui->wind_V_group->setDisabled(true);
  // QMaps (lists) containing model in- and output
  modelinputlist = new QMap<int, modelinput>;
  modeloutputlist = new QMap<int, modeloutput>;

  updateRunList();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::updateRunList()
{
  // Setup list
  QStringList heading;
  heading << "ID" << "Name";
  ui->modelRunsList->setColumnCount(2);
  ui->modelRunsList->setHeaderLabels(heading);
  ui->modelRunsList->hideColumn(0);
  ui->modelRunsList->setSelectionMode(QAbstractItemView::ExtendedSelection);
  // Update list
  //ui->modelRunsList->clear();
  //QMap<int, modeloutput>::const_iterator i = modeloutputlist->constBegin();
  //  while (i != modeloutputlist->constEnd()) {
  //      QTreeWidgetItem *point = new QTreeWidgetItem(ui->modelRunsList);
  //      point->setText(0, QString::number(i.key()));
  //      point->setText(1, modeloutputlist->value(i.key()).name);
  //      ++i;
  //}
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
