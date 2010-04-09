#include "plotwindow.h"
#include "plotarea.h"
#include "ui_plotwindow.h"
#include <iostream>

plotwindow::plotwindow(QMap<int, modelrun> *runs, QMainWindow *parent) : QTabWidget(parent), ui(new Ui::plotwindow)
{
  ui->setupUi(this);
  selectedruns = new QList<int>;
  runlist = runs;

  connect(ui->modelruntree, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(updateselectedruns()));
  connect(ui->plotvar, SIGNAL(currentIndexChanged(int)), this, SLOT(changeplotvar()));

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
    if (runlist->value(i.key()).hasrun)
    {
      QTreeWidgetItem *point = new QTreeWidgetItem(ui->modelruntree);
      point->setCheckState(1,Qt::Unchecked);
      point->setDisabled(false);
      point->setText(0, QString::number(i.key()));
      point->setText(1, runlist->value(i.key()).runname);
    }
  ++i;
  }

  // Dropdown with plot variables
  QStringList varnames;
  varnames << "CBL-height" << "Potential temperature" << "Potential temperature jump" << "Sensible heat flux"
          << "Specific humidity" << "Specific humidity jump" << "Latent heat flux";
  outputnames << "h" << "theta" << "dtheta" << "wtheta" << "q" << "dq" << "wq";
  ui->plotvar->addItems(varnames);
}

plotwindow::~plotwindow()
{
  delete ui;
}

void plotwindow::updateselectedruns()               // create QList containing ID's of selected runs
{
  int id = ui->modelruntree->currentItem()->text(0).toInt();
  if (ui->modelruntree->currentItem()->checkState(1) == 2)
    selectedruns->append(id);
  else if(ui->modelruntree->currentItem()->checkState(1) == 0)
    selectedruns->removeAt(selectedruns->indexOf(id));

  plot->update();


/*
  selectedruns->clear();
  int i=0;
  while(QTreeWidgetItem *item = ui->modelruntree->topLevelItem(i))
  {
    if (item->checkState(1) == 2)
    {
      QString ident = item->text(0);
      int n = ident.toInt(0,10);
      selectedruns->append(n);
    }
    i++;
  }
  plot->update();
*/
}

void plotwindow::changeplotvar()
{
  plot->plotvar = outputnames[ui->plotvar->currentIndex()];
  plot->update();
}

void plotwindow::deleterun(int num)
{
  QString id = QString::number(num,10);
  QTreeWidgetItem *del = ui->modelruntree->findItems(id,Qt::MatchExactly,0).value(0);
  delete del;
  selectedruns->removeAt(selectedruns->indexOf(num));
  plot->update();
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
    point->setText(1, runlist->value(num).runname);
  }
  plot->update();
}
