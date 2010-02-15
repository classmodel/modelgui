#ifndef MLM_MAIN_H
#define MLM_MAIN_H

#include <QtGui/QMainWindow>
#include "runwidget.h"
#include <QString>

QT_BEGIN_NAMESPACE
class QAction;
class QTreeWidget;
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
    QString name;
    double *t;
    double *h;
};

class mlm_main : public QMainWindow
{
    Q_OBJECT

public:
    mlm_main(QMainWindow *parent = 0);
    ~mlm_main();
    int numruns;                        // Counter for number of model-runs.
    void widgetrun(QMap<int, rundata> *);
    runWidget *widget;

    QMap<int, rundata> *modelruns;

public slots:
    void createRun();                   // Function to create new model-run object
    void updaterunlist();
    void dropRun();

private:
    void createDocks();
    void createMenus();
    void createToolBars();

    QDockWidget *rightdock;

    QFrame *dummyFrame;
    QPushButton *newrunButton;
    QPushButton *showgraphButton;
    QPushButton *droprunButton;
    QMenu *viewMenu;
    QTreeWidget *runviewList;
};

#endif // MLM_MAIN_H
