#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QPushButton;
QT_END_NAMESPACE
class RenderArea;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();

private slots:
    void axisChanged();

private:
    RenderArea *renderArea;
    QPushButton *drawButton;
    QLineEdit *xminInput;
    QLabel *xminLabel;
    QLineEdit *xmaxInput;
    QLabel *xmaxLabel;
    QLineEdit *yminInput;
    QLabel *yminLabel;
    QLineEdit *ymaxInput;
    QLabel *ymaxLabel;
 };
#endif
