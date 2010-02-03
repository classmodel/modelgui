#ifndef RUNWIDGET_H
#define RUNWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QPushButton;
QT_END_NAMESPACE

class runWidget : public QWidget
{
    Q_OBJECT

public:
    runWidget();

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
