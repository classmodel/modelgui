#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>

class RenderArea : public QWidget
{
    Q_OBJECT

public:
    enum Shape { Line, Points, Polyline, Polygon, Rect, RoundedRect, Ellipse, Arc,
                 Chord, Pie, Path, Text, Pixmap };

    RenderArea(QWidget *parent = 0);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    double transfx(double xreal, double xscale, double xmin);    // Function real-coordinate to Widget-coordinate
    double transfy(double yreal, double yscale, double ymin);    // Function real-coordinate to Widget-coordinate

public slots:
    void setAxis(double xmin, double xmax, double ymin, double ymax);

protected:
    void paintEvent(QPaintEvent *event);

private:
    QPen pen;
    QBrush brush;
    QPixmap pixmap;
    static const int plotmargin = 50;
    double xmin, xmax, ymin, ymax;
    bool axisset;
};

#endif
