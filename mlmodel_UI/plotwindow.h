#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include <QtGui/QWidget>
#include <QtGui/QTabWidget>
#include "mainwindow.h"

class modelrun;

namespace Ui
{
  class plotwindow;
}

class plotwindow : public QTabWidget
{
  Q_OBJECT

public:
  plotwindow(QMap<int,modelrun> *, QMainWindow *);
  ~plotwindow();

private:
  Ui::plotwindow *ui;
};

#endif // PLOTWINDOW_H
