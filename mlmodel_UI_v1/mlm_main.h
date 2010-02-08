#ifndef MLM_MAIN_H
#define MLM_MAIN_H

#include <QtGui/QMainWindow>
#include "runwidget.h"

QT_BEGIN_NAMESPACE
class QAction;
class QListWidget;
class QMenu;
class QTextEdit;
class QPushButton;
class QFrame;
class QVBoxLayout;
class QCloseEvent;
QT_END_NAMESPACE

// External classes
class runWidget;

class rundata
{
public:
    double *t;
    double *h;
};

class mlm_main : public QMainWindow
{
    Q_OBJECT

public:
    mlm_main(QMainWindow *parent = 0);
    ~mlm_main();
    rundata *modelruns[20];
    int numruns;                        // Counter for number of model-runs.
    void widgetrun(rundata *);
    runWidget *widget;

public slots:
    void createRun();                   // Function to create new model-run object

private:
    void createDocks();
    void createMenus();
    void createToolBars();
    QFrame *dummyFrame;
    QPushButton *newrunButton;
    QPushButton *showgraphButton;
    QMenu *viewMenu;
};

#endif // MLM_MAIN_H
