#include "mlm_main.h"
#include <QtGui>
#include <list>
#include <string>
#include <sstream>
#include <iostream>    // TIJDELIJK

void mlm_main::widgetrun(QMap<QString, rundata> *thisrun)
{
    widget = new runWidget(thisrun, this);
    widget->setWindowFlags(Qt::Window);
    widget->show();
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

    modelruns = new QMap<QString, rundata>;
}

mlm_main::~mlm_main()
{

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
    widgetrun(modelruns);
    numruns++;
}


