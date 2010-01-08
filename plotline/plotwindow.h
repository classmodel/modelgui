#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include <QtGui/QWidget>

namespace Ui
{
    class PlotWindow;
}

class PlotWindow : public QWidget
{
    Q_OBJECT

public:
    PlotWindow(QWidget *parent = 0);
    ~PlotWindow();
    double transfx(double xreal, double xscale, double xmin);    // Function real-coordinate to Widget-coordinate
    double transfy(double yreal, double yscale, double ymin);    // Function real-coordinate to Widget-coordinate

private:
    Ui::PlotWindow *ui;
    static const int plotmargin = 50;        // Margin left and below of axis

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
};

#endif // PLOTWINDOW_H
