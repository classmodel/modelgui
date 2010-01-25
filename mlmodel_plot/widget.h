#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui/QWidget>
#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>

namespace Ui
{
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
    double *x;
    double *y;

public slots:
    void showData();

private:
    Ui::Widget *ui;
    QPushButton *showButton;
    QLineEdit *xminInput;
    QLineEdit *xmaxInput;
    QLineEdit *xintInput;
};

#endif // WIDGET_H
