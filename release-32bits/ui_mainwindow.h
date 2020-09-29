/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QPushButton *pushButton_9;
    QPushButton *pushButton_10;
    QPushButton *pushButton_1;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QPushButton *pushButton_7;
    QPushButton *pushButton_8;
    QPushButton *pushButton_11;
    QPushButton *pushButton_12;
    QPushButton *pushButton_13;
    QPushButton *pushButton_14;
    QPushButton *pushButton_15;
    QPushButton *pushButton_16;
    QPushButton *pushButton_17;
    QPushButton *pushButton_18;
    QPushButton *pushButton_19;
    QPushButton *pushButton_20;
    QMenuBar *menuBar;
    QStatusBar *statusBar;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1586, 775);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        scrollArea = new QScrollArea(centralWidget);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setGeometry(QRect(200, 100, 1200, 600));
        scrollArea->setMouseTracking(true);
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 1198, 598));
        scrollArea->setWidget(scrollAreaWidgetContents);
        pushButton_9 = new QPushButton(centralWidget);
        pushButton_9->setObjectName(QStringLiteral("pushButton_9"));
        pushButton_9->setGeometry(QRect(30, 560, 93, 28));
        QFont font;
        font.setPointSize(11);
        font.setBold(true);
        font.setWeight(75);
        pushButton_9->setFont(font);
        pushButton_9->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_9->setCheckable(true);
        pushButton_9->setChecked(false);
        pushButton_10 = new QPushButton(centralWidget);
        pushButton_10->setObjectName(QStringLiteral("pushButton_10"));
        pushButton_10->setGeometry(QRect(30, 610, 93, 28));
        pushButton_10->setFont(font);
        pushButton_10->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_10->setCheckable(true);
        pushButton_10->setChecked(false);
        pushButton_1 = new QPushButton(centralWidget);
        pushButton_1->setObjectName(QStringLiteral("pushButton_1"));
        pushButton_1->setGeometry(QRect(30, 160, 93, 28));
        QFont font1;
        font1.setPointSize(11);
        font1.setBold(true);
        font1.setItalic(false);
        font1.setWeight(75);
        font1.setStrikeOut(false);
        pushButton_1->setFont(font1);
        pushButton_1->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_1->setCheckable(true);
        pushButton_1->setChecked(false);
        pushButton_1->setFlat(false);
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(30, 210, 93, 28));
        pushButton_2->setFont(font);
        pushButton_2->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_2->setCheckable(true);
        pushButton_2->setChecked(false);
        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(30, 260, 93, 28));
        pushButton_3->setFont(font);
        pushButton_3->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_3->setCheckable(true);
        pushButton_3->setChecked(false);
        pushButton_4 = new QPushButton(centralWidget);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(30, 310, 93, 28));
        pushButton_4->setFont(font);
        pushButton_4->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_4->setCheckable(true);
        pushButton_4->setChecked(false);
        pushButton_5 = new QPushButton(centralWidget);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setGeometry(QRect(30, 360, 93, 28));
        pushButton_5->setFont(font);
        pushButton_5->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_5->setCheckable(true);
        pushButton_5->setChecked(false);
        pushButton_6 = new QPushButton(centralWidget);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));
        pushButton_6->setGeometry(QRect(30, 410, 93, 28));
        pushButton_6->setFont(font);
        pushButton_6->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_6->setCheckable(true);
        pushButton_6->setChecked(false);
        pushButton_7 = new QPushButton(centralWidget);
        pushButton_7->setObjectName(QStringLiteral("pushButton_7"));
        pushButton_7->setGeometry(QRect(30, 460, 93, 28));
        pushButton_7->setFont(font);
        pushButton_7->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_7->setCheckable(true);
        pushButton_7->setChecked(false);
        pushButton_8 = new QPushButton(centralWidget);
        pushButton_8->setObjectName(QStringLiteral("pushButton_8"));
        pushButton_8->setGeometry(QRect(30, 510, 93, 28));
        pushButton_8->setFont(font);
        pushButton_8->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_8->setCheckable(true);
        pushButton_8->setChecked(false);
        pushButton_11 = new QPushButton(centralWidget);
        pushButton_11->setObjectName(QStringLiteral("pushButton_11"));
        pushButton_11->setGeometry(QRect(1450, 160, 93, 28));
        pushButton_11->setFont(font);
        pushButton_11->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_11->setCheckable(true);
        pushButton_11->setChecked(false);
        pushButton_12 = new QPushButton(centralWidget);
        pushButton_12->setObjectName(QStringLiteral("pushButton_12"));
        pushButton_12->setGeometry(QRect(1450, 210, 93, 28));
        pushButton_12->setFont(font);
        pushButton_12->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_12->setCheckable(true);
        pushButton_12->setChecked(false);
        pushButton_13 = new QPushButton(centralWidget);
        pushButton_13->setObjectName(QStringLiteral("pushButton_13"));
        pushButton_13->setGeometry(QRect(1450, 260, 93, 28));
        pushButton_13->setFont(font);
        pushButton_13->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_13->setCheckable(true);
        pushButton_14 = new QPushButton(centralWidget);
        pushButton_14->setObjectName(QStringLiteral("pushButton_14"));
        pushButton_14->setGeometry(QRect(1450, 310, 93, 28));
        pushButton_14->setFont(font);
        pushButton_14->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_14->setCheckable(true);
        pushButton_15 = new QPushButton(centralWidget);
        pushButton_15->setObjectName(QStringLiteral("pushButton_15"));
        pushButton_15->setGeometry(QRect(1450, 360, 93, 28));
        pushButton_15->setFont(font);
        pushButton_15->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_15->setCheckable(true);
        pushButton_16 = new QPushButton(centralWidget);
        pushButton_16->setObjectName(QStringLiteral("pushButton_16"));
        pushButton_16->setGeometry(QRect(1450, 410, 93, 28));
        pushButton_16->setFont(font);
        pushButton_16->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_16->setCheckable(true);
        pushButton_17 = new QPushButton(centralWidget);
        pushButton_17->setObjectName(QStringLiteral("pushButton_17"));
        pushButton_17->setGeometry(QRect(1450, 460, 93, 28));
        pushButton_17->setFont(font);
        pushButton_17->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_17->setCheckable(true);
        pushButton_18 = new QPushButton(centralWidget);
        pushButton_18->setObjectName(QStringLiteral("pushButton_18"));
        pushButton_18->setGeometry(QRect(1450, 510, 93, 28));
        pushButton_18->setFont(font);
        pushButton_18->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_18->setCheckable(true);
        pushButton_19 = new QPushButton(centralWidget);
        pushButton_19->setObjectName(QStringLiteral("pushButton_19"));
        pushButton_19->setGeometry(QRect(1450, 560, 93, 28));
        pushButton_19->setFont(font);
        pushButton_19->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_19->setCheckable(true);
        pushButton_20 = new QPushButton(centralWidget);
        pushButton_20->setObjectName(QStringLiteral("pushButton_20"));
        pushButton_20->setGeometry(QRect(1450, 610, 93, 28));
        pushButton_20->setFont(font);
        pushButton_20->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_20->setCheckable(true);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1586, 26));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        pushButton_9->setText(QApplication::translate("MainWindow", "\345\234\260\351\223\2019\345\217\267\347\272\277", Q_NULLPTR));
        pushButton_10->setText(QApplication::translate("MainWindow", "\345\234\260\351\223\20110\345\217\267\347\272\277", Q_NULLPTR));
        pushButton_1->setText(QApplication::translate("MainWindow", "\345\234\260\351\223\2011\345\217\267\347\272\277", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("MainWindow", "\345\234\260\351\223\2012\345\217\267\347\272\277", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("MainWindow", "\345\234\260\351\223\2013\345\217\267\347\272\277", Q_NULLPTR));
        pushButton_4->setText(QApplication::translate("MainWindow", "\345\234\260\351\223\2014\345\217\267\347\272\277", Q_NULLPTR));
        pushButton_5->setText(QApplication::translate("MainWindow", "\345\234\260\351\223\2015\345\217\267\347\272\277", Q_NULLPTR));
        pushButton_6->setText(QApplication::translate("MainWindow", "\345\234\260\351\223\2016\345\217\267\347\272\277", Q_NULLPTR));
        pushButton_7->setText(QApplication::translate("MainWindow", "\345\234\260\351\223\2017\345\217\267\347\272\277", Q_NULLPTR));
        pushButton_8->setText(QApplication::translate("MainWindow", "\345\234\260\351\223\2018\345\217\267\347\272\277", Q_NULLPTR));
        pushButton_11->setText(QApplication::translate("MainWindow", "\345\234\260\351\223\20111\345\217\267\347\272\277", Q_NULLPTR));
        pushButton_12->setText(QApplication::translate("MainWindow", "\345\234\260\351\223\20112\345\217\267\347\272\277", Q_NULLPTR));
        pushButton_13->setText(QApplication::translate("MainWindow", "\345\234\260\351\223\20113\345\217\267\347\272\277", Q_NULLPTR));
        pushButton_14->setText(QApplication::translate("MainWindow", "\345\234\260\351\223\20114\345\217\267\347\272\277", Q_NULLPTR));
        pushButton_15->setText(QApplication::translate("MainWindow", "\345\234\260\351\223\20115\345\217\267\347\272\277", Q_NULLPTR));
        pushButton_16->setText(QApplication::translate("MainWindow", "\345\234\260\351\223\20116\345\217\267\347\272\277", Q_NULLPTR));
        pushButton_17->setText(QApplication::translate("MainWindow", "\345\234\260\351\223\20117\345\217\267\347\272\277", Q_NULLPTR));
        pushButton_18->setText(QApplication::translate("MainWindow", "\345\234\260\351\223\20118\345\217\267\347\272\277", Q_NULLPTR));
        pushButton_19->setText(QApplication::translate("MainWindow", "\345\234\260\351\223\20119\345\217\267\347\272\277", Q_NULLPTR));
        pushButton_20->setText(QApplication::translate("MainWindow", "\345\234\260\351\223\20120\345\217\267\347\272\277", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
