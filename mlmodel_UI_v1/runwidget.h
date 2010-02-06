#ifndef RUNWIDGET_H
#define RUNWIDGET_H

#include <QWidget>
#include "mlm_main.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QPushButton;
QT_END_NAMESPACE

class rundata;

class runWidget : public QWidget
{
    Q_OBJECT

public:
    runWidget(rundata *, QWidget *);
    int widgetnum;

public slots:
    void createData(rundata *);

private:
    QPushButton *runButton;
    QLineEdit *tminInput;
    QLabel *tminLabel;
    QLineEdit *tmaxInput;
    QLabel *tmaxLabel;
    QLineEdit *ampInput;
    QLabel *ampLabel;
    
 };
#endif
