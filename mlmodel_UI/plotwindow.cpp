#include "plotwindow.h"
#include "ui_plotwindow.h"

plotwindow::plotwindow(QTabWidget *parent)
    : QTabWidget(parent), ui(new Ui::plotwindow)
{
    ui->setupUi(this);
}

plotwindow::~plotwindow()
{
    delete ui;
}
