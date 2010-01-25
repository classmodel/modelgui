#include "widget.h"
#include "window.h"
#include "ui_widget.h"
#include <iostream>
#include <QLineEdit>
#include <cmath>

    Widget::Widget(QWidget *parent)
        : QWidget(parent), ui(new Ui::Widget)
    {
        ui->setupUi(this);

        xminInput = new QLineEdit;
        xminInput = ui->xminInput;
        xminInput->setReadOnly(false);

        xmaxInput = new QLineEdit;
        xmaxInput = ui->xmaxInput;
        xmaxInput->setReadOnly(false);

        xintInput = new QLineEdit;
        xintInput = ui->xintInput;
        xintInput->setReadOnly(false);

        showButton = new QPushButton;
        showButton = ui->showButton;

        connect(showButton, SIGNAL(clicked()), this,
                    SLOT(showData()));

        setWindowTitle(tr("Initiate ML-model"));
    }

    Widget::~Widget()
    {
        delete ui;
    }

    void Widget::showData()
    {
        double xmin = xminInput->text().toDouble();
        double xmax = xmaxInput->text().toDouble();
        double xint = xintInput->text().toDouble();
        int steps = ceil((xmax - xmin) / xint)+1;
        x = new double[steps];
        y = new double[steps];
        int i;
        for (i=0; i<(steps); i++)
        {
            x[i] = xmin + (i * xint);
            y[i] = cos(x[i]);
            std::cout << "X: " << x[i] << " Y: " << y[i] << std::endl;
        }

        return;
    }
