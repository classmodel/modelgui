#include "plotwindow.h"
#include "plotarea.h"
#include "ui_plotwindow.h"
#include <iostream>

plotwindow::plotwindow(QMap<int, modelrun> *runlist, QMainWindow *parent) : QTabWidget(parent), ui(new Ui::plotwindow)
{
  ui->setupUi(this);

  selectedruns = new QList<int>;

  // Remove dummy-plot-widget, add and setup new plot widget
  ui->verticalLayout->removeWidget(ui->plotarea);
  plot = new plotarea(runlist, selectedruns, this);
  QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  sizePolicy.setHorizontalStretch(0);
  sizePolicy.setVerticalStretch(0);
  sizePolicy.setHeightForWidth(plot->sizePolicy().hasHeightForWidth());
  plot->setSizePolicy(sizePolicy);
  plot->setMinimumSize(QSize(400, 400));
  ui->verticalLayout->addWidget(plot);

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


