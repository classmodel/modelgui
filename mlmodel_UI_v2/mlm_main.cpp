#include "mlm_main.h"
#include <QtGui>
#include <QList>
#include <list>
#include <string>
#include <QString>
#include <sstream>
#include <iostream>    // TIJDELIJK
#include <QAbstractItemView>

void mlm_main::widgetrun(QMap<int, rundata> *thisrun)
{
    widget = new runWidget(thisrun, this);
    widget->setWindowFlags(Qt::Window);
    widget->show();
}

mlm_main::mlm_main(QMainWindow *parent) : QMainWindow(parent)
{
    numruns = 0;
    dummyFrame = new QFrame;
    dummyFrame->setFrameStyle(QFrame::Raised);
    dummyFrame->setFixedSize(300,300);
    setCentralWidget(dummyFrame);
    createMenus();
    createDocks();
    setWindowTitle(tr("MLMODEL"));
    modelruns = new QMap<int, rundata>;
}

mlm_main::~mlm_main()
{
    /*
    QMap<int, rundata>::const_iterator i = modelruns->constBegin();
    while (i != modelruns->constEnd()) {
       std::cout << "Name run: " << qPrintable(modelruns->value(i.key()).name) << std::endl;
       ++i;
    }
    */
}

void mlm_main::createMenus()
{
    viewMenu = menuBar()->addMenu(tr("&View"));
}

void mlm_main::createDocks()
{
    // ==================================================
    //   CREATE MODEL RUN
    // ==================================================

    rightdock = new QDockWidget(tr("Create new run"), this);
    rightdock->setObjectName(QString::fromUtf8("rightdock"));
    rightdock->setAllowedAreas(Qt::RightDockWidgetArea);
    rightdock->setFixedWidth(160);

    // Dummy content -> create new run and show graph buttons.
    QWidget *initcontainer = new QWidget();
    initcontainer->setObjectName(QString::fromUtf8("initcontainer"));
    QWidget *verticalLayoutWidget = new QWidget(initcontainer);
    verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
    QVBoxLayout *verticalLayout = new QVBoxLayout(verticalLayoutWidget);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));

    newrunButton = new QPushButton(initcontainer);
    newrunButton->setText("New run");
        verticalLayout->addWidget(newrunButton);
    showgraphButton = new QPushButton(initcontainer);
    showgraphButton->setText("Show Graph");
        verticalLayout->addWidget(showgraphButton);

    rightdock->setWidget(initcontainer);
    addDockWidget(Qt::RightDockWidgetArea, rightdock);
    viewMenu->addAction(rightdock->toggleViewAction());

    connect(newrunButton, SIGNAL(clicked()), this, SLOT(createRun()));

    // ==================================================
    //   OVERVIEW MODELRUNS
    // ==================================================

    QStringList heading;
    heading << "ID" << "Name";

    runviewList = new QTreeWidget(initcontainer);
    runviewList->setColumnCount(2);
    runviewList->setHeaderLabels(heading);
    runviewList->setColumnWidth(0,30);
    runviewList->hideColumn(0);                    // (DIS)/(EN)ABLE FOR HIDING ID COLUMN
    runviewList->setFixedWidth(150);
    runviewList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    verticalLayout->addWidget(runviewList);

    droprunButton = new QPushButton(initcontainer);
    droprunButton->setText("Delete selected");
    verticalLayout->addWidget(droprunButton);

    connect(droprunButton, SIGNAL(clicked()), this, SLOT(dropRun()));

    // ==================================================

    rightdock->setWidget(initcontainer);
    addDockWidget(Qt::RightDockWidgetArea, rightdock);
    viewMenu->addAction(rightdock->toggleViewAction());
}

void mlm_main::updaterunlist()
{
    runviewList->clear();
    QMap<int, rundata>::const_iterator i = modelruns->constBegin();
    while (i != modelruns->constEnd()) {
        QTreeWidgetItem *point = new QTreeWidgetItem(runviewList);
        point->setText(0, QString::number(i.key()));
        point->setText(1, modelruns->value(i.key()).name);
        ++i;
    }
}

void mlm_main::dropRun()
{
    if(runviewList->selectedItems().count() > 0)
    {
        for (int i=0; i<runviewList->selectedItems().count(); i++)
        {
            QString ident = runviewList->selectedItems()[i]->text(0);
            int n = ident.toInt(0,10);
            modelruns->remove(n);
        }
    updaterunlist();
    }
}

void mlm_main::createRun()
{
    widgetrun(modelruns);
    numruns++;
}
