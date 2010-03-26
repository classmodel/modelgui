#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include <QtGui/QWidget>

namespace Ui
{
    class plotwindow;
}

class plotwindow : public QWidget
{
    Q_OBJECT

public:
    plotwindow(QWidget *parent = 0);
    ~plotwindow();

private:
    Ui::plotwindow *ui;
};

#endif // PLOTWINDOW_H
