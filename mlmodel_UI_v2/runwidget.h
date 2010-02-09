#ifndef RUNWIDGET_H
#define RUNWIDGET_H

#include <QWidget>
#include "mlm_main.h"
#include <qmap.h>
#include <qstring.h>

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
    runWidget(QMap<QString, rundata> *, QWidget *);

public slots:
    void createData();

private:
    QPushButton *runButton;
    QLineEdit *nameInput;
    QLabel *nameLabel;
    QLineEdit *tminInput;
    QLabel *tminLabel;
    QLineEdit *tmaxInput;
    QLabel *tmaxLabel;
    QLineEdit *ampInput;
    QLabel *ampLabel;

    QMap<QString, rundata> *thisrun;
    
 };
#endif
