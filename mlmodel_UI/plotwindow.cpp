#include "plotwindow.h"
#include "ui_plotwindow.h"
#include <iostream>

plotwindow::plotwindow(QMap<int, modelrun> *runlist, QMainWindow *parent) : QTabWidget(parent), ui(new Ui::plotwindow)
{
  ui->setupUi(this);

  // Setup QTreeWidget
  QStringList heading;
  heading << "ID" << "Name";
  ui->modelruntree->setColumnCount(2);
  ui->modelruntree->setHeaderLabels(heading);
  ui->modelruntree->setColumnWidth(0,35);
  ui->modelruntree->hideColumn(0);
  ui->modelruntree->setSelectionMode(QAbstractItemView::ExtendedSelection);

  QMap<int, modelrun>::const_iterator i = runlist->constBegin();
  while (i != runlist->constEnd()) {
    QTreeWidgetItem *point = new QTreeWidgetItem(ui->modelruntree);
    point->setCheckState(1,Qt::Unchecked);
    point->setFlags(Qt::ItemIsUserCheckable);
    point->setDisabled(false);
    point->setText(0, QString::number(i.key()));
    point->setText(1, runlist->value(i.key()).runname);
    ++i;
    }
}

plotwindow::~plotwindow()
{
  delete ui;
}


