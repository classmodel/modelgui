#include "mlm_main.h"
#include <QtGui>
#include <list>
#include <string>
#include <sstream>
#include <iostream>    // TIJDELIJK

mlm_main::mlm_main()
{
    // Dummy-frame -> now used as central Widget
    dummyFrame = new QFrame;
    dummyFrame->setFrameStyle(QFrame::Raised);
    setCentralWidget(dummyFrame);
    createMenus();
    createDocks();
    setWindowTitle(tr("MLMODEL"));

    numruns = 0;
}

void mlm_main::createMenus()
{
    viewMenu = menuBar()->addMenu(tr("&View"));
}

void mlm_main::createDocks()
{
    // Create dock at right side of main window.
    QDockWidget *rightdock = new QDockWidget(tr("Create new run"), this);
    rightdock->setObjectName(QString::fromUtf8("rightdock"));
    rightdock->setAllowedAreas(Qt::RightDockWidgetArea);
    rightdock->setFixedWidth(130);

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
}

void mlm_main::createRun()
{
    // Rather complicated method to get names "run1", "run2", etc??
    std::string basename("run");
    std::stringstream run_name;
    run_name << basename << numruns;
    std::cout << run_name.str() << std::endl;  // temporary; output on terminal
    // Put name in list
    modelruns.push_back(run_name.str());
    // Create objects
    // ???????

    numruns++;
}
