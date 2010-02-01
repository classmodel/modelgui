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

class rundata
{
public:
    double *t;
    double *h;
    std::string name;
};


class mlm_main : public QMainWindow
{
    Q_OBJECT

public:
    mlm_main();
    //std::list<rundata> modelruns;     // List containing model-run objects
    rundata *modelruns[20];

public slots:
    void createRun();                     // Function to create new model-run object

private:
    void createDocks();
    void createMenus();
    void createToolBars();

    QFrame *dummyFrame;
    QPushButton *newrunButton;
    QPushButton *showgraphButton;
    QMenu *viewMenu;

    int numruns;                          // Counter for number of model-runs.
};



#endif // MLM_MAIN_H
