/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Thu Nov 3 04:58:17 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *ConnectAsReceiver;
    QPushButton *OpenAsSrc;
    QLineEdit *filePath;
    QLineEdit *IPAddress;
    QTextEdit *DebugOutput;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *PortNr;
    QLabel *label_3;
    QPushButton *browse_button;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(600, 400);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        ConnectAsReceiver = new QPushButton(centralWidget);
        ConnectAsReceiver->setObjectName(QString::fromUtf8("ConnectAsReceiver"));
        ConnectAsReceiver->setGeometry(QRect(50, 40, 141, 27));
        OpenAsSrc = new QPushButton(centralWidget);
        OpenAsSrc->setObjectName(QString::fromUtf8("OpenAsSrc"));
        OpenAsSrc->setGeometry(QRect(50, 90, 93, 27));
        filePath = new QLineEdit(centralWidget);
        filePath->setObjectName(QString::fromUtf8("filePath"));
        filePath->setGeometry(QRect(210, 90, 113, 27));
        IPAddress = new QLineEdit(centralWidget);
        IPAddress->setObjectName(QString::fromUtf8("IPAddress"));
        IPAddress->setGeometry(QRect(210, 40, 113, 27));
        DebugOutput = new QTextEdit(centralWidget);
        DebugOutput->setObjectName(QString::fromUtf8("DebugOutput"));
        DebugOutput->setGeometry(QRect(50, 140, 341, 181));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(220, 70, 62, 17));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(220, 20, 81, 20));
        PortNr = new QLineEdit(centralWidget);
        PortNr->setObjectName(QString::fromUtf8("PortNr"));
        PortNr->setGeometry(QRect(350, 40, 113, 27));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(350, 20, 62, 17));
        browse_button = new QPushButton(centralWidget);
        browse_button->setObjectName(QString::fromUtf8("browse_button"));
        browse_button->setGeometry(QRect(350, 90, 93, 27));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 23));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        ConnectAsReceiver->setText(QApplication::translate("MainWindow", "ConnectAsReceiver", 0, QApplication::UnicodeUTF8));
        OpenAsSrc->setText(QApplication::translate("MainWindow", "OpenAsSrc", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "IP Address", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "Port", 0, QApplication::UnicodeUTF8));
        browse_button->setText(QApplication::translate("MainWindow", "Browse", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
