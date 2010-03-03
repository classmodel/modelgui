#include "widget.h"
#include "ui_widget.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <QLineEdit>


using namespace std;

    Widget::Widget(QWidget *parent)
        : QWidget(parent), ui(new Ui::Widget)
    {
        ui->setupUi(this);

        h0Line = new QLineEdit;
        h0Line = ui->h0Line;
        h0Line->setReadOnly(false);

        thetaLine = new QLineEdit;
        thetaLine = ui->thetaLine;
        thetaLine->setReadOnly(false);

        readButton = new QPushButton;
        readButton = ui->readButton;

        calcButton = new QPushButton;
        calcButton = ui->calcButton;

        dthetaLine = new QLineEdit;
        dthetaLine = ui->dthetaLine;
        dthetaLine->setReadOnly(false);

        labthetaLine = new QLineEdit;
        labthetaLine = ui->labthetaLine;
        labthetaLine->setReadOnly(false);

        //timeCalced = new QLabel("n/a");
        //timeCalced = ui->timeCalced;
        //timeCalced->setText("n/a");

        connect(readButton, SIGNAL(clicked()), this,
                    SLOT(readData()));
        connect(calcButton, SIGNAL(clicked()), this,
                    SLOT(calcData()));

        setWindowTitle(tr("MXL made simple :)"));
    }

    Widget::~Widget()
    {
        delete ui;
    }

    void Widget::readData()
    {
        double h0, theta, dtheta, labtheta;
        ifstream fin("vars.ini");
        fin >> h0 >> theta >> dtheta >> labtheta;
        h0Line->setText(QString::number(h0));
        thetaLine->setText(QString::number(theta));
        dthetaLine->setText(QString::number(dtheta));
        labthetaLine->setText(QString::number(labtheta));
        return;
    }
    
    void Widget::calcData()
    {
        double h0 = h0Line->text().toDouble();
        double theta = thetaLine->text().toDouble();
        if (h0 != 0) {
            h0 = h0 + 100.;
        }
        if (theta != 0) {
            theta = theta +1.5;
        }
        h0Line->setText(QString::number(h0));
        thetaLine->setText(QString::number(theta));
        return;
    }
