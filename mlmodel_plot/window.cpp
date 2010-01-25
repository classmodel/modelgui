#include <QtGui>
#include "renderarea.h"
#include "window.h"
#include "widget.h"

#include <iostream>

const int IdRole = Qt::UserRole;

Window::Window()
{
    renderArea = new RenderArea;

    drawButton = new QPushButton(tr("draw"));
    xminInput = new QLineEdit(tr(""));
    xminLabel = new QLabel(tr("X-min:"));
    xminLabel->setBuddy(xminInput);
    xmaxInput = new QLineEdit(tr(""));
    xmaxLabel = new QLabel(tr("X-max:"));
    xmaxLabel->setBuddy(xmaxInput);
    yminInput = new QLineEdit(tr(""));
    yminLabel = new QLabel(tr("Y-min:"));
    yminLabel->setBuddy(yminInput);
    ymaxInput = new QLineEdit(tr(""));
    ymaxLabel = new QLabel(tr("Y-max:"));
    ymaxLabel->setBuddy(ymaxInput);

    connect(drawButton, SIGNAL(clicked()), this, SLOT(axisChanged()));

    QGridLayout *mainLayout = new QGridLayout;

    mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnStretch(6, 1);
    mainLayout->addWidget(renderArea, 0, 0, 1, 7);
    mainLayout->setRowMinimumHeight(1, 6);

    mainLayout->addWidget(xminLabel, 1,1, Qt::AlignRight);
    mainLayout->addWidget(xminInput, 1,2);
    mainLayout->addWidget(xmaxLabel, 1,3, Qt::AlignRight);
    mainLayout->addWidget(xmaxInput, 1,4);

    mainLayout->addWidget(yminLabel, 2,1, Qt::AlignRight);
    mainLayout->addWidget(yminInput, 2,2);
    mainLayout->addWidget(ymaxLabel, 2,3, Qt::AlignRight);
    mainLayout->addWidget(ymaxInput, 2,4);

    mainLayout->addWidget(drawButton, 1,5);
    setLayout(mainLayout);

    setWindowTitle(tr("ML-model plot"));
}

void Window::axisChanged()
{
    double xmin = xminInput->text().toDouble();
    double xmax = xmaxInput->text().toDouble();
    double ymin = yminInput->text().toDouble();
    double ymax = ymaxInput->text().toDouble();
    renderArea->setAxis(xmin,xmax,ymin,ymax);
}
