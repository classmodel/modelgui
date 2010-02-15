#include "mlm_main.h"
#include <QtGui>
#include <list>
#include <string>
#include <sstream>
#include <iostream>    // TIJDELIJK

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
    setCentralWidget(dummyFrame);
    createMenus();
    createDocks();
    setWindowTitle(tr("MLMODEL"));
    modelruns = new QMap<int, rundata>;
}

mlm_main::~mlm_main()
{
    QMap<int, rundata>::const_iterator i = modelruns->constBegin();
    while (i != modelruns->constEnd()) {
       std::cout << "Name run: " << qPrintable(modelruns->value(i.key()).name) << std::endl;
       ++i;
    }
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
    rightdock->setFixedSize(200,200);

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

    runviewList = new QTreeWidget(initcontainer);
    runviewList->setColumnCount(2);
    runviewList->setColumnWidth(0,30);

    // Method to add one item... Just for testing..
    QTreeWidgetItem *point = new QTreeWidgetItem(runviewList);
    point->setText(0, "1");
    point->setText(1, "run1");


    verticalLayout->addWidget(runviewList);

    // ==================================================

    rightdock->setWidget(initcontainer);
    addDockWidget(Qt::RightDockWidgetArea, rightdock);
    viewMenu->addAction(rightdock->toggleViewAction());
}

void mlm_main::updaterunlist()
{
    // Update runlist....
}

void mlm_main::createRun()
{
    widgetrun(modelruns);
    numruns++;
}


