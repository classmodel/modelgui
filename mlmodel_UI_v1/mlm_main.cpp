#include "mlm_main.h"
#include <QtGui>
#include <list>
#include <string>
#include <sstream>
#include <iostream>    // TIJDELIJK

void rundata::widgetrun()
{
    widget = new runWidget();
    widget->show();
}

void rundata::widgetdestroy()
{
    widget->close();
}

mlm_main::mlm_main(QMainWindow *parent) : QMainWindow(parent)
{
    dummyFrame = new QFrame;
    dummyFrame->setFrameStyle(QFrame::Raised);
    setCentralWidget(dummyFrame);
    createMenus();
    createDocks();
    setWindowTitle(tr("MLMODEL"));
    numruns = 0;
}

void mlm_main::closeEvent(QCloseEvent *)
 {
    std::cout << "mainwin closed" << std::endl;
    modelruns[numruns]->widgetdestroy();
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
    rightdock->setFixedSize(130,200);

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

    if (numruns == 0)
    {
        showgraphButton->setDisabled(true);
    }

    rightdock->setWidget(initcontainer);
    addDockWidget(Qt::RightDockWidgetArea, rightdock);
    viewMenu->addAction(rightdock->toggleViewAction());

    connect(newrunButton, SIGNAL(clicked()), this, SLOT(createRun()));
}

void mlm_main::createRun()
{
    std::string basename("run");
    std::stringstream run_name;
    run_name << basename << numruns;

    modelruns[numruns] = new rundata();
    modelruns[numruns]->widgetrun();

    numruns++;

    if (numruns > 0)
    {
        showgraphButton->setDisabled(false);
    }

    if (numruns > 9)
    {
        newrunButton->setDisabled(true);
    }
}


