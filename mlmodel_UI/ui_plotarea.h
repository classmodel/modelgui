/********************************************************************************
** Form generated from reading UI file 'plotarea.ui'
**
** Created
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLOTAREA_H
#define UI_PLOTAREA_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_plotarea
{
public:
    QVBoxLayout *verticalLayout;
    QVBoxLayout *plotLayout;
    QGridLayout *plotLayoutGrid;
    QHBoxLayout *layout_xmin;
    QLabel *xminLabel;
    QLineEdit *xminInput;
    QHBoxLayout *layout_ymin;
    QLabel *yminLabel;
    QLineEdit *yminInput;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *layout_xmax;
    QLabel *xmaxLabel;
    QLineEdit *xmaxInput;
    QHBoxLayout *layout_ymax;
    QLabel *ymaxLabel;
    QLineEdit *ymaxInput;
    QComboBox *plotvar;
    QPushButton *saveButton;
    QCheckBox *autoscaleaxis;
    QWidget *plotarea_2;

    void setupUi(QWidget *plotarea)
    {
        if (plotarea->objectName().isEmpty())
            plotarea->setObjectName(QString::fromUtf8("plotarea"));
        plotarea->resize(460, 434);
        verticalLayout = new QVBoxLayout(plotarea);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        plotLayout = new QVBoxLayout();
        plotLayout->setSpacing(6);
        plotLayout->setObjectName(QString::fromUtf8("plotLayout"));
        plotLayoutGrid = new QGridLayout();
        plotLayoutGrid->setSpacing(6);
        plotLayoutGrid->setObjectName(QString::fromUtf8("plotLayoutGrid"));
        layout_xmin = new QHBoxLayout();
        layout_xmin->setSpacing(6);
        layout_xmin->setObjectName(QString::fromUtf8("layout_xmin"));
        xminLabel = new QLabel(plotarea);
        xminLabel->setObjectName(QString::fromUtf8("xminLabel"));
        xminLabel->setMinimumSize(QSize(45, 0));
        xminLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        layout_xmin->addWidget(xminLabel);

        xminInput = new QLineEdit(plotarea);
        xminInput->setObjectName(QString::fromUtf8("xminInput"));
        xminInput->setMinimumSize(QSize(60, 0));
        xminInput->setMaximumSize(QSize(100, 1000));

        layout_xmin->addWidget(xminInput);


        plotLayoutGrid->addLayout(layout_xmin, 1, 0, 1, 1);

        layout_ymin = new QHBoxLayout();
        layout_ymin->setSpacing(6);
        layout_ymin->setObjectName(QString::fromUtf8("layout_ymin"));
        layout_ymin->setSizeConstraint(QLayout::SetFixedSize);
        yminLabel = new QLabel(plotarea);
        yminLabel->setObjectName(QString::fromUtf8("yminLabel"));
        yminLabel->setMinimumSize(QSize(45, 0));
        yminLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        layout_ymin->addWidget(yminLabel);

        yminInput = new QLineEdit(plotarea);
        yminInput->setObjectName(QString::fromUtf8("yminInput"));
        yminInput->setMinimumSize(QSize(60, 0));
        yminInput->setMaximumSize(QSize(100, 1000));

        layout_ymin->addWidget(yminInput);


        plotLayoutGrid->addLayout(layout_ymin, 1, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(10000, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        plotLayoutGrid->addItem(horizontalSpacer, 1, 4, 1, 1);

        layout_xmax = new QHBoxLayout();
        layout_xmax->setSpacing(6);
        layout_xmax->setObjectName(QString::fromUtf8("layout_xmax"));
        xmaxLabel = new QLabel(plotarea);
        xmaxLabel->setObjectName(QString::fromUtf8("xmaxLabel"));
        xmaxLabel->setMinimumSize(QSize(45, 0));
        xmaxLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        layout_xmax->addWidget(xmaxLabel);

        xmaxInput = new QLineEdit(plotarea);
        xmaxInput->setObjectName(QString::fromUtf8("xmaxInput"));
        xmaxInput->setMinimumSize(QSize(60, 0));
        xmaxInput->setMaximumSize(QSize(100, 1000));

        layout_xmax->addWidget(xmaxInput);


        plotLayoutGrid->addLayout(layout_xmax, 2, 0, 1, 1);

        layout_ymax = new QHBoxLayout();
        layout_ymax->setSpacing(6);
        layout_ymax->setObjectName(QString::fromUtf8("layout_ymax"));
        ymaxLabel = new QLabel(plotarea);
        ymaxLabel->setObjectName(QString::fromUtf8("ymaxLabel"));
        ymaxLabel->setMinimumSize(QSize(45, 0));
        ymaxLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        layout_ymax->addWidget(ymaxLabel);

        ymaxInput = new QLineEdit(plotarea);
        ymaxInput->setObjectName(QString::fromUtf8("ymaxInput"));
        ymaxInput->setMinimumSize(QSize(60, 0));
        ymaxInput->setMaximumSize(QSize(100, 1000));

        layout_ymax->addWidget(ymaxInput);


        plotLayoutGrid->addLayout(layout_ymax, 2, 2, 1, 1);

        plotvar = new QComboBox(plotarea);
        plotvar->setObjectName(QString::fromUtf8("plotvar"));
        plotvar->setMaximumSize(QSize(2000, 2000));

        plotLayoutGrid->addWidget(plotvar, 0, 0, 1, 5);

        saveButton = new QPushButton(plotarea);
        saveButton->setObjectName(QString::fromUtf8("saveButton"));
        saveButton->setMinimumSize(QSize(130, 25));

        plotLayoutGrid->addWidget(saveButton, 1, 3, 1, 1);

        autoscaleaxis = new QCheckBox(plotarea);
        autoscaleaxis->setObjectName(QString::fromUtf8("autoscaleaxis"));

        plotLayoutGrid->addWidget(autoscaleaxis, 2, 3, 1, 2);


        plotLayout->addLayout(plotLayoutGrid);

        plotarea_2 = new QWidget(plotarea);
        plotarea_2->setObjectName(QString::fromUtf8("plotarea_2"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(plotarea_2->sizePolicy().hasHeightForWidth());
        plotarea_2->setSizePolicy(sizePolicy);
        plotarea_2->setMinimumSize(QSize(300, 300));
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Active, QPalette::Window, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush);
        plotarea_2->setPalette(palette);
        plotarea_2->setAutoFillBackground(false);

        plotLayout->addWidget(plotarea_2);


        verticalLayout->addLayout(plotLayout);


        retranslateUi(plotarea);

        QMetaObject::connectSlotsByName(plotarea);
    } // setupUi

    void retranslateUi(QWidget *plotarea)
    {
        plotarea->setWindowTitle(QApplication::translate("plotarea", "plotarea", 0, QApplication::UnicodeUTF8));
        xminLabel->setText(QApplication::translate("plotarea", "X-min:", 0, QApplication::UnicodeUTF8));
        yminLabel->setText(QApplication::translate("plotarea", "Y-min:", 0, QApplication::UnicodeUTF8));
        xmaxLabel->setText(QApplication::translate("plotarea", "X-max:", 0, QApplication::UnicodeUTF8));
        ymaxLabel->setText(QApplication::translate("plotarea", "Y-max:", 0, QApplication::UnicodeUTF8));
        saveButton->setText(QApplication::translate("plotarea", "Save figure", 0, QApplication::UnicodeUTF8));
        autoscaleaxis->setText(QApplication::translate("plotarea", "Auto scale axis", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class plotarea: public Ui_plotarea {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLOTAREA_H
