#include "mlm_main.h"
#include <QtGui>

mlm_main::mlm_main()
{
    // Dummy-frame -> now used as central Widget
    dummyFrame = new QFrame;
    dummyFrame->setFrameStyle(QFrame::Raised);
    setCentralWidget(dummyFrame);

    createDocks();
    setWindowTitle(tr("MLMODEL"));
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
}
