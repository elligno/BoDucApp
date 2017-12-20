/********************************************************************************
** Form generated from reading UI file 'BoDucAppGui.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BODUCAPPGUI_H
#define UI_BODUCAPPGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BoDucAppGuiClass
{
public:

    void setupUi(QWidget *BoDucAppGuiClass)
    {
        if (BoDucAppGuiClass->objectName().isEmpty())
            BoDucAppGuiClass->setObjectName(QStringLiteral("BoDucAppGuiClass"));
        BoDucAppGuiClass->resize(600, 400);

        retranslateUi(BoDucAppGuiClass);

        QMetaObject::connectSlotsByName(BoDucAppGuiClass);
    } // setupUi

    void retranslateUi(QWidget *BoDucAppGuiClass)
    {
        BoDucAppGuiClass->setWindowTitle(QApplication::translate("BoDucAppGuiClass", "BoDucAppGui", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class BoDucAppGuiClass: public Ui_BoDucAppGuiClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BODUCAPPGUI_H
