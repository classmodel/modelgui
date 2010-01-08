#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui/QWidget>
#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>
#include <QtGui/QTextEdit>
#include <QtGui/QMessageBox>

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

public slots:
    void readData();
    void calcData();

private:
    Ui::Widget *ui;
    QPushButton *readButton;
    QPushButton *calcButton;
    QLineEdit *h0Line;
    QLineEdit *thetaLine;
    QLineEdit *dthetaLine;
    QLineEdit *labthetaLine;
    //QLabel *thetaCalced;
    //QLabel *timeCalced;
};

#endif // WIDGET_H
