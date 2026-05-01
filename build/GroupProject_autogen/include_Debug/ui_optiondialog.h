/********************************************************************************
** Form generated from reading UI file 'optiondialog.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPTIONDIALOG_H
#define UI_OPTIONDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_OptionDialog
{
public:
    QGridLayout *gridLayout;
    QSpinBox *spinBoxG;
    QLabel *ColourRBG;
    QSpinBox *spinBoxR;
    QLabel *Visible;
    QCheckBox *checkBoxShrink;
    QCheckBox *checkBoxClip;
    QLabel *Name;
    QSpinBox *spinBoxB;
    QCheckBox *checkBoxVisible;
    QDialogButtonBox *buttonBox;
    QLineEdit *lineEditName;

    void setupUi(QDialog *OptionDialog)
    {
        if (OptionDialog->objectName().isEmpty())
            OptionDialog->setObjectName("OptionDialog");
        OptionDialog->resize(400, 300);
        gridLayout = new QGridLayout(OptionDialog);
        gridLayout->setObjectName("gridLayout");
        spinBoxG = new QSpinBox(OptionDialog);
        spinBoxG->setObjectName("spinBoxG");

        gridLayout->addWidget(spinBoxG, 6, 3, 1, 1);

        ColourRBG = new QLabel(OptionDialog);
        ColourRBG->setObjectName("ColourRBG");

        gridLayout->addWidget(ColourRBG, 2, 1, 1, 2);

        spinBoxR = new QSpinBox(OptionDialog);
        spinBoxR->setObjectName("spinBoxR");

        gridLayout->addWidget(spinBoxR, 6, 1, 1, 1);

        Visible = new QLabel(OptionDialog);
        Visible->setObjectName("Visible");

        gridLayout->addWidget(Visible, 2, 4, 1, 1);

        checkBoxShrink = new QCheckBox(OptionDialog);
        checkBoxShrink->setObjectName("checkBoxShrink");

        gridLayout->addWidget(checkBoxShrink, 3, 5, 1, 1);

        checkBoxClip = new QCheckBox(OptionDialog);
        checkBoxClip->setObjectName("checkBoxClip");

        gridLayout->addWidget(checkBoxClip, 4, 5, 1, 1);

        Name = new QLabel(OptionDialog);
        Name->setObjectName("Name");

        gridLayout->addWidget(Name, 0, 2, 1, 1);

        spinBoxB = new QSpinBox(OptionDialog);
        spinBoxB->setObjectName("spinBoxB");

        gridLayout->addWidget(spinBoxB, 6, 2, 1, 1);

        checkBoxVisible = new QCheckBox(OptionDialog);
        checkBoxVisible->setObjectName("checkBoxVisible");

        gridLayout->addWidget(checkBoxVisible, 2, 5, 1, 1);

        buttonBox = new QDialogButtonBox(OptionDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        gridLayout->addWidget(buttonBox, 5, 0, 2, 6);

        lineEditName = new QLineEdit(OptionDialog);
        lineEditName->setObjectName("lineEditName");

        gridLayout->addWidget(lineEditName, 1, 3, 1, 2);


        retranslateUi(OptionDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, OptionDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, OptionDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(OptionDialog);
    } // setupUi

    void retranslateUi(QDialog *OptionDialog)
    {
        OptionDialog->setWindowTitle(QCoreApplication::translate("OptionDialog", "Dialog", nullptr));
        ColourRBG->setText(QCoreApplication::translate("OptionDialog", "Colour (R,G,B)", nullptr));
        Visible->setText(QCoreApplication::translate("OptionDialog", "Visible", nullptr));
        checkBoxShrink->setText(QCoreApplication::translate("OptionDialog", "Apply Shrink Filter", nullptr));
        checkBoxClip->setText(QCoreApplication::translate("OptionDialog", "Apply Clip Filter", nullptr));
        Name->setText(QCoreApplication::translate("OptionDialog", "Name", nullptr));
        checkBoxVisible->setText(QCoreApplication::translate("OptionDialog", "Visible", nullptr));
    } // retranslateUi

};

namespace Ui {
    class OptionDialog: public Ui_OptionDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPTIONDIALOG_H
