#include "plotwindow.h"
#include "ui_plotwindow.h"

plotwindow::plotwindow(QMap<int, modelrun> *givenrun, QMainWindow *parent) : QTabWidget(parent), ui(new Ui::plotwindow)
{
    ui->setupUi(this);
}

plotwindow::~plotwindow()
{
    delete ui;
}
