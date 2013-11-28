/*
 * CLASS
 * Copyright (c) 2010-2013 Meteorology and Air Quality section, Wageningen University and Research centre
 * Copyright (c) 2011-2013 Jordi Vila-Guerau de Arellano
 * Copyright (c) 2011-2013 Chiel van Heerwaarden
 * Copyright (c) 2011-2013 Bart van Stratum
 * Copyright (c) 2011-2013 Kees van den Dries
 *
 * This file is part of CLASS
 *
 * CLASS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * CLASS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with CLASS.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include "mainwindow.h"
#include "subplot.h"
#include "QPaintEvent"
#include "QPainter"
#include "QTreeWidget"

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
  #include <QtGui/QWidget>
  #include <QtGui/QTabWidget>
#else
  #include <QtWidgets/QWidget>
  #include <QtWidgets/QTabWidget>
#endif

class modelrun;
class subplot;

namespace Ui
{
  class plotwindow;
}

class plotwindow : public QMainWindow
{
  Q_OBJECT

public:
  plotwindow(QMap<int,modelrun> *, QList<int> *, QMainWindow *);
  ~plotwindow();
  plotarea *plotar;

  QList<int> *selectedruns;
  QList<int> *initialselected;
  QMap<int, modelrun> *runlist;
  QString plotvarx, plotvary;

public slots:
  void addrun(int num);
  void deleterun(int num);
  void updateselectedruns();
  void changeplotvar();
  void changeaxis();
  void zoomebymouse();
  void cursormoved();
  void switchtobasicplotting();
  void switchtoadvancedplotting();
  void viewmenutriggered();
  void togglebasicsettings(bool);
  void toggleadvancedsettings(bool);
  void toggleaxissettings(bool);
  void togglemodelruns(bool);
  void selectadvanceddata(QTreeWidgetItem*,int);
  void changeplottype();

signals:
  void graphclosed(plotwindow* plot);

protected:
  void mouseMoveEvent(QMouseEvent *);

private:
  bool MacOS;
  Ui::plotwindow *ui;
  void closeEvent(QCloseEvent *event);
  QStringList outputnames;
  void updateplotdata();
  void getdata(outputvar*, modelrun, QString);
  void setplotvar(const QString, QString*);
};

#endif // PLOTWINDOW_H
