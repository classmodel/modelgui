#include "plotwindow.h"
#include "ui_plotwindow.h"

plotwindow::plotwindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::plotwindow)
{
    ui->setupUi(this);
}

plotwindow::~plotwindow()
{
    delete ui;
}
