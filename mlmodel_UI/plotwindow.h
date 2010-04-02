#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include <QtGui/QWidget>
#include <QtGui/QTabWidget>

namespace Ui
{
    class plotwindow;
}

class plotwindow : public QTabWidget
{
    Q_OBJECT

public:
    plotwindow(QTabWidget *parent = 0);
    ~plotwindow();

private:
    Ui::plotwindow *ui;
};

#endif // PLOTWINDOW_H
