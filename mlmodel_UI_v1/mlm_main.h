#ifndef MLM_MAIN_H
#define MLM_MAIN_H

#include <QtGui/QMainWindow>

QT_BEGIN_NAMESPACE
class QAction;
class QListWidget;
class QMenu;
class QTextEdit;
class QPushButton;
class QFrame;
class QVBoxLayout;
QT_END_NAMESPACE

class mlm_main : public QMainWindow
{
    Q_OBJECT

public:
    mlm_main();

private:
    void createDocks();

    QFrame *dummyFrame;
    QPushButton *newrunButton;
    QPushButton *showgraphButton;

};

#endif // MLM_MAIN_H
