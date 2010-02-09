#include <QtGui>
#include <QString>
#include "runwidget.h"
#include "mlm_main.h"
#include <iostream>
#include <cstdlib>
#include <math.h>

const int IdRole = Qt::UserRole;

runWidget::runWidget(QMap<QString, rundata> *givenrun, QWidget *parent) : QWidget(parent)
{
    runButton = new QPushButton(tr("run"));
    tminInput = new QLineEdit(tr("7"));
    tminLabel = new QLabel(tr("t-min:"));
    tminLabel->setBuddy(tminInput);
    tmaxInput = new QLineEdit(tr("14"));
    tmaxLabel = new QLabel(tr("t-max:"));
    tmaxLabel->setBuddy(tmaxInput);
    ampInput = new QLineEdit(tr("1"));
    ampLabel = new QLabel(tr("Amplitude:"));
    ampLabel->setBuddy(ampInput);

    connect(runButton, SIGNAL(clicked()), this, SLOT(createData()));

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
    thisrun = givenrun;
}

void runWidget::createData()
{
    double tmin = tminInput->text().toDouble();
    double tmax = tmaxInput->text().toDouble();
    double amp = ampInput->text().toDouble();
    int tsteps = (tmax-tmin)*60;

    rundata *run1;
    run1 = new rundata;
    run1->t = new double [tsteps];
    run1->h = new double [tsteps];

    for (int i=0; i<tsteps; i++)
    {
        run1->t[i] = tmin + (i*tsteps);
        run1->h[i] = amp*cos(run1->t[i]);
    }

    QString nn;
    nn.setNum(rand());
    thisrun->insert(nn,*run1);
}


