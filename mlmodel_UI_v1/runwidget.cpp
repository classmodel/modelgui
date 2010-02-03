#include <QtGui>
#include "runwidget.h"
#include <iostream>

const int IdRole = Qt::UserRole;

runWidget::runWidget()
{
    runButton = new QPushButton(tr("run"));
    tminInput = new QLineEdit(tr(""));
    tminLabel = new QLabel(tr("t-min:"));
    tminLabel->setBuddy(tminInput);
    tmaxInput = new QLineEdit(tr(""));
    tmaxLabel = new QLabel(tr("t-max:"));
    tmaxLabel->setBuddy(tmaxInput);
    ampInput = new QLineEdit(tr("1"));
    ampLabel = new QLabel(tr("Amplitude:"));
    ampLabel->setBuddy(ampInput);

    //connect(drawButton, SIGNAL(clicked()), this, SLOT(axisChanged()));

    QGridLayout *mainLayout = new QGridLayout;

    mainLayout->addWidget(tminLabel, 1,0, Qt::AlignRight);
    mainLayout->addWidget(tminInput, 1,1);
    mainLayout->addWidget(tmaxLabel, 2,0, Qt::AlignRight);
    mainLayout->addWidget(tmaxInput, 2,1);

    mainLayout->addWidget(ampLabel, 3,0, Qt::AlignRight);
    mainLayout->addWidget(ampInput, 3,1);

    mainLayout->addWidget(runButton, 4,1);
    setLayout(mainLayout);

    setWindowTitle(tr("Initiate model-run"));
}


